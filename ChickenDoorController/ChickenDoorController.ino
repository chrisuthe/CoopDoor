/*
 Name:		ChickenDoorController.ino
 Created:	4/3/2017 9:16:16 PM
 Author:	Chris Uthe
 GitHub:    https://github.com/chrisuthe/CoopDoor 
*/
#include <math.h>
#include <Dusk2Dawn.h>
#include <EEPROM.h>
#include <Wire.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include "RTClib.h"


//Define what pins things are hooked up to
//this is the reedswitch at the top of the door. It's a normally open switch which will read LOW when the door is all the way up
const int doorUpSensorPin = 2;
//this is the reedswitch at the bottom of the door. It's a normally open switch which will read LOW when the door is all the way down
//const int doorDownSensorPin = 7;
const int doorDownMotorPin = 5;
const int DoorUpMotorPin = 3;
const int builtInLED = 13;
int currentMins;
int sunrise;
int sunset;
DateTime now;

//Are We Debugging?
const bool debug = true;

//Define RTC
RTC_DS3231 rtc;

//define the location / timezone for dusk2dawn (this is Colton, SD)
Dusk2Dawn colton(43.789369, -96.927453, -6);

// the setup function runs once when you press reset or power the board
void setup() {
	//Set the mode the pins will operate in. In this case on/off as outputs
	pinMode(DoorUpMotorPin, OUTPUT);
	pinMode(doorDownMotorPin, OUTPUT);
	//Set Serial for Debugging
	if (debug) {
		Serial.begin(9600);
	}
	Serial.println("Checking for RTC");
	if (!rtc.begin()) {
		Serial.println("Couldn't find RTC");
		while (1);
	}
	rtc.begin();
	// Lets just set the date/time every time we upload a debug build, since we know the RTC module is good with a good battery.
	if (debug)
	{
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}
	Serial.println("Checking RTC is Running");
	if (rtc.lostPower()) {
		Serial.println("RTC lost power, lets set the time!");
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		if (debug) {
			Serial.println("Set Date/Time");
			rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		}
	}
	//switches setup
	pinMode(doorUpSensorPin, INPUT_PULLUP);
	pinMode(builtInLED, OUTPUT);
	Serial.println("FinishedSetup");
	if (digitalRead(doorUpSensorPin) == LOW)
	{
		//door is open all the way

	}
}


// the loop function runs over and over again until power down or reset
void loop() {
	delay(2000);
	if (debug){
		Serial.println();
		Serial.println("LoopHead");
	}
	//Get the Date/Time from the RTC
	now = rtc.now();
	//Get Sunrise/Sunset for the current year/month/day as INT's that equate to minutes from midnight that he sunrise/sunset will occur (THE TRUE is passing in Daylight Savings Time!)
	sunrise = colton.sunrise(now.year(), now.month(), now.day(), true);
	sunset = colton.sunset(now.year(), now.month(), now.day(), true);
	if (debug) {
		Serial.println();
		Serial.print(now.year());
		Serial.print('/');
		Serial.print(now.month());
		Serial.print('/');
		Serial.print(now.day());
		Serial.print(" - ");
		Serial.print(now.hour());
		Serial.print(':');
		Serial.print(now.minute());
		Serial.print(':');
		Serial.print(now.second());
	}
	
	//Lets get add the "now" Minutes and "now" hours*60 to see how many minutes from midnight we are
	currentMins = ((now.hour()) * 60) + (now.minute());
	Serial.println(currentMins);

	//lets start comparisons, if the door should be up....
	if (sunrise < currentMins && currentMins < sunset)
	{
		Serial.println("Door should be up");
	}

	else
	{
		Serial.println("Door should be down");
	}

	/* Loop ToDo: 
	 * Compare door state to what it should be
	 * If states don't match raise or lower the door until the switches indicate it's closed or open
	 * Limit motor run time to ??? (this is probably going to be in the open / close subroutines?
	 * Add in an Over-ride: If they push the button over ride to the opposite of the current state: Record the override (overRideOpen or overRideClose) and open/close the door.
	 * Check the override when considering if we open or close time based. If the door is closed and we overRideOpen we ignore all close requests but as soon as the time conidtion matches the override (overRideOpen && DoorShouldBeOpen) we clear the override. The door shuts normally and opens normally next time cycle
	 * Add a temperature sensor
	 * If the temp sensor is below 30*F the door does not open unless override is used. Do not clear override if temp is below (causing door to not open)
	 */



	int DoorUp = digitalRead(doorUpSensorPin);
	/* This is just temporary switch debug code*/
	if (DoorUp == LOW)
	{
		Serial.println("Door Opened All The Way");
		stopDoor();
		digitalWrite(builtInLED, HIGH);
	}
	else
	{
		digitalWrite(builtInLED, LOW);
		("Door Not Open all the way");
		raiseDoor();
	}
	
}
