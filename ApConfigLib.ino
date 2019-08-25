// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       ApConfigLib.ino
    Created:	24/08/2019 18:35:47
    Author:     MANU-PC\ManuF
*/

// Define User Types below here or use a .h file
//
#include <Arduino.h>
//#include <user_interface.h>
#include "ApConfig.h"
ApConfig apconfig;


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(115200);
	while (!Serial) {delay(500);}
	delay(3000);
	apconfig.Begin(5, 2, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0), RootHandle);
}

// Add the main program code into the continuous loop() function
void loop()
{
	apconfig.Handle();

}

void RootHandle()
{
	//Serve a very basic page on the root url
	webServer.send(200, "text/plain", "http://www.blue-ocean-sailing.com/");
}
