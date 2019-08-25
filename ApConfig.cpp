#include "ApConfig.h"

Preferences prefApConfig;
WebServer webServer(80);
Ticker ticker;
bool booted = false;
struct config_t config;


ApConfig::ApConfig()
{
	
}


ApConfig::~ApConfig()
{
	//prefApConfig.end();
}

bool ApConfig::Begin(int channel, int max_connection, IPAddress IP, IPAddress mask, myHandle myhandle)
{
	bool v_ResetFlag;
	prefApConfig.begin("ApConfig", false);
	//Check to see if the flag is still set from the previous boot
	if (checkResetFlag())
	{
		v_ResetFlag = true;
		//Do the firmware reset here
		#ifdef USESERIAL
		Serial.printf("Reset Firmware\n");
		#endif
		//Set the ssid to default value and turn off the password
		//WiFi.mode(WIFI_AP);
		//delay(100);
		//WiFi.softAP("APConfig", "", channel, false, max_connection);
		//delay(100);
		//WiFi.softAPConfig(IP, IP, mask);
		WifiConfig(channel, max_connection, IP, mask, "APConfig", "");
		delay(500);

	}
	else {
		//Start the wifi with the required username and password
		v_ResetFlag = false;
		delay(3000);
		LoadConfig();
		PrintConfig();

		//WiFi.mode(WIFI_AP);
		//delay(100);
		//WiFi.softAP(config.ssid, config.pass, channel, false, max_connection);
		//delay(100);
		//WiFi.softAPConfig(IP, IP, mask);
		WifiConfig(channel, max_connection, IP, mask, config.ssid, config.pass);
		delay(500);
	}

	//Serve a very basic page on the root url
	//webServer.on("/", []() {
		//webServer.send(200, "text/plain", "http://www.blue-ocean-sailing.com/");

	webServer.on("/", myhandle);

	// Set up the admin page
	webServer.on("/admin", std::bind(serveAdmin, &webServer));
	webServer.begin();

	return v_ResetFlag;
}

void ApConfig::WifiConfig(int channel, int max_connection, IPAddress IP, IPAddress mask, char* ssid, char* password)
{
	WiFi.mode(WIFI_AP);
	delay(100);
	WiFi.softAP(config.ssid, config.pass, channel, false, max_connection);
	delay(100);
	WiFi.softAPConfig(IP, IP, mask);
}

void ApConfig::Handle()
{
	webServer.handleClient();
}

void ApConfig::SaveConfig() {
#ifdef USESERIAL
	Serial.printf("Save Config\r\n");
	PrintConfig();
#endif

	prefApConfig.putBytes("ssid", config.ssid, MAX_STR_LEN);
	prefApConfig.putBytes("pass", config.pass, MAX_STR_LEN);
}

void ApConfig::LoadConfig() {
	bool eepromValid = true;

	// Load the config variables from EEPROM
	config_t eepromConfig;
	//EEPROM_readAnything(0, eepromConfig);
	prefApConfig.getBytes("ssid", eepromConfig.ssid, MAX_STR_LEN);
	prefApConfig.getBytes("pass", eepromConfig.pass, MAX_STR_LEN);

	//Check to see if the config variables loaded from eeprom are valid
	eepromValid &= ValidateString(eepromConfig.ssid);
	eepromValid &= ValidateString(eepromConfig.pass);

	if (eepromValid) {
		strcpy(config.ssid, eepromConfig.ssid);
		strcpy(config.pass, eepromConfig.pass);
	}
	else {
		// If the EEROM isn't valid then create a unique name for the wifi
		ResetConfig();
		SaveConfig();
	}
}

void ApConfig::ResetConfig() {
#ifdef USESERIAL
	Serial.printf("Reset Config\n");
#endif

#ifdef ESP8266
	uint32_t chipID = ESP.getChipId();
#endif

#ifdef ESP32
	uint64_t macAddress = ESP.getEfuseMac();
	uint64_t macAddressTrunc = macAddress << 40;
	uint32_t chipID = macAddressTrunc >> 40;
#endif

	// If the EEPROM isn't valid then create a unique name for the wifi
	sprintf(config.ssid, "%s %06X", SSID_DEFAULT, chipID);
	sprintf(config.pass, PASS_DEFAULT);

	SaveConfig();
}

bool ApConfig::ValidateString(char* value) {
	bool valid = true;
	#ifdef USESERIAL
	Serial.println("Enter Validate string method!");
	#endif
	//Check to see if the string is an acceptable length
	if ((strlen(value) < MIN_STR_LEN) && (strlen(value) >= MAX_STR_LEN)) {
		#ifdef USESERIAL
		Serial.println("Lenght is bad !");
		#endif
		valid = false;
	}
	else {
		#ifdef USESERIAL
		Serial.println("Lenght is good !");
		#endif
		//Check each character in the string to make sure it is alphanumeric or space
		for (uint8_t i = 0; i < strlen(value); i++)
		{
			//Serial.printf("%c ", value[i]);
			if (!isAlphaNumeric(value[i]))
			{
				#ifdef USESERIAL
				Serial.println("is not alphanumeric !");
				#endif
				if (value[i] != ' ' && value[i] != '_')
				{
					#ifdef USESERIAL
					Serial.println("is not a space or underscore !");
					#endif			
					valid = false;
				}
			}
		}
		//Serial.println("");
	}

	return valid;
}

void ApConfig::PrintConfig() {
#ifdef USESERIAL
	Serial.printf("SSID: '%s'\n", config.ssid);
	Serial.printf("Pass: '%s'\n", config.pass);
#endif
}

void ApConfig::InitFirmwareReset()
{
	prefApConfig.begin("Reset", false);
}

// Allows shared usage of the built in led for as long as the firmware reset needs it
void ApConfig::BuiltInLED(bool on) {
#ifdef USELED
	if (booted)
		digitalWrite(LED_BUILTIN, on);
#endif
}

// End the firmware reset process, at this point the software can run normally without checking for user resets
void ApConfig::finishBoot() {
	booted = true;

#ifdef USESERIAL
	Serial.printf("Booted\n");
#endif

	ticker.detach();
}

// Clear the reset flag in memory, this ends the window for a possible reset
void ApConfig::clearFlag() {
	uint32_t value = FLAGCLEAR;
	//ESP.rtcUserMemoryWrite(FLAGADDRESS, &value, sizeof(value));
	//Flag.put(0, value);
	prefApConfig.putUInt("FirmReset", value);

#ifdef USELED
	digitalWrite(LED_BUILTIN, HIGH);
#endif

#ifdef USESERIAL
	Serial.printf("Clear Flag\n");
#endif

	ticker.attach(CLEARTIMEOUT, finishBoot);
}

void ApConfig::setFlag() {
	uint32_t value = FLAGSET;
	//ESP.rtcUserMemoryWrite(FLAGADDRESS, &value, sizeof(value));
	//Flag.put(0, value);
	prefApConfig.putUInt("FirmReset", value);

#ifdef USELED
	for (uint16_t i = 0; i < 10; i++)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(100);
		digitalWrite(LED_BUILTIN, LOW);
		delay(100);
	}
#endif

#ifdef USESERIAL
	Serial.printf("Set Flag\n");
#endif

	ticker.attach(CLEARTIMEOUT, clearFlag);
}

// Checks to see if the user has rebooted the processor within the reset time frame
bool ApConfig::checkResetFlag() {
	uint32_t value = FLAGCLEAR;
	bool result;

#ifdef USELED
	// Set up the LED as a status indicator
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
#endif

	// Check to see the state of the flag in memory
	//ESP.rtcUserMemoryRead(FLAGADDRESS, &value, sizeof(value));
	//Flag.get(0, value);
	prefApConfig.getUInt("FirmReset", value);
	result = (value == FLAGSET);

	// If the user previously performed a reset then begin the reset process
	if (result)
		clearFlag();

	// If the flag was not previously set, then set up a new reset window
	else
		ticker.attach(SETTIMEOUT, setFlag);
	return result;
}

//Creates a webpage that allows the user to change the SSID and Password from the browser
void ApConfig::serveAdmin(WebServer *webServer) {
	String message;

	// Check to see if we've been sent any arguments and instantly return if not
	if (webServer->args() == 0) {
		webServer->sendHeader("Content-Length", String(strlen(adminPage)));
		webServer->send(200, "text/html", adminPage);
	}
	else {
		// Create a string containing all the arguments, send them out to the serial port
		// Check to see if there are new values (also doubles to check the length of the new value is long enough)
		#ifdef USESERIAL
		Serial.println(webServer->arg("newssid"));
		#endif

		if ((webServer->arg("newssid").length() >= MIN_STR_LEN) &&
			(webServer->arg("newssid").length() < MAX_STR_LEN))
			webServer->arg("newssid").toCharArray(config.ssid, sizeof(config.ssid));

		if (webServer->arg("newpassword").length() < MAX_STR_LEN)
			webServer->arg("newpassword").toCharArray(config.pass, sizeof(config.pass));

		// Store the new settings to EEPROM
		SaveConfig();
		PrintConfig();

		// Construct a message to tell the user that the change worked
		message = "New settings will take effect after restart";

		// Reply with a web page to indicate success or failure
		message = "<html><head><meta http-equiv='refresh' content='5;url=/' /></head><body>" + message;
		message += "<br/>Redirecting in 5 seconds...</body></html>";
		webServer->sendHeader("Content-Length", String(message.length()));
		webServer->send(200, "text/html", message);
	}
  }
