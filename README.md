# ApConfigLib
Access Point with admin page for ESP32

This is a refactoring work and port to esp32 from msraynsford APCONFIG (https://github.com/msraynsford/APConfig).

A simple framework for creating an standalone access point for an ESP8266. Once connected to the device it is possible to change the SSID and Password from within a browser, it is also possible to reset the device should the password be forgotten.

# Example

```
#include "ApConfig.h"
ApConfig apconfig;

void setup()
{
	Serial.begin(115200);
	while (!Serial) {delay(500);}
	delay(3000);
	apconfig.Begin(5, 2, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0), RootHandle);
}
```

then in your loop:
```
void loop()
{
	apconfig.Handle();

}
```

now you just need to add the handle for the root webserver as you want:

```
void RootHandle()
{
	//Serve a very basic page on the root url
	webServer.send(200, "text/plain", "http://www.blue-ocean-sailing.com/");
}
```
# Know Issue

For now the reset windows where you can reset the AP SSID and Password not working (something not working as expected when EN button is pushed)
As soon as I time I will explore this problem.


