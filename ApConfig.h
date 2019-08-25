//#pragma once
#ifndef _APCONFIGLIB
#define _APCONFIGLIB

#include <Arduino.h>
#include "AdminPage.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <Ticker.h>

//#define USESERIAL
#define USELED

#define LED_BUILTIN 2
#define FLAGSET     0x55555555
#define FLAGCLEAR   0xAAAAAAAA
#define FLAGADDRESS 00
#define SETTIMEOUT 3
#define CLEARTIMEOUT 1
#define MIN_STR_LEN 8
#define MAX_STR_LEN 17

#define SSID_DEFAULT "APConfig"
#define PASS_DEFAULT "password"

extern WebServer webServer;

struct config_t {
	char ssid[MAX_STR_LEN] = SSID_DEFAULT;
	char pass[MAX_STR_LEN] = PASS_DEFAULT;
};

typedef void(*myHandle) ();

	class ApConfig
	{
	public:
		ApConfig();
		~ApConfig();

		static bool Begin(int channel, int max_connection, IPAddress IP, IPAddress mask, myHandle myhandle);
		static void Handle();

	private:
		static void WifiConfig(int channel, int max_connection, IPAddress IP, IPAddress mask, char* ssid, char* password);
		static void ResetConfig();
		static bool ValidateString(char* value);
		static void LoadConfig();
		static void SaveConfig();
		static void PrintConfig();
		static void InitFirmwareReset();
		static void BuiltInLED(bool on);
		static void finishBoot();
		static void clearFlag();
		static void setFlag();
		static bool checkResetFlag();
		static void serveAdmin(WebServer *webServer);
	};
#endif