/*
 Name:		ChickenDoorController.ino
 Created:	4/3/2017 9:16:16 PM
 Author:	Chris Uthe
 GitHub:    https://github.com/chrisuthe/CoopDoor 
*/
#include <math.h>
#include <Dusk2Dawn.h>
#include <eeprom.h>
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

//Define RTC
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

//Are We Debugging?
const bool debug = true;

//define the location / timezone for dusk2dawn (this is Colton, SD)
Dusk2Dawn colton(43.789369, -96.927453, -6);

//Temporarily define how long to run the motor 
//TODO: Substitute a better way of starting/stopping the opening and closing [[Sensors??]]
int runTime = 5000;

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
	Serial.println("Checking RTC is Running");
	if (rtc.lostPower()) {
		Serial.println("RTC lost power, lets set the time!");
		// following line sets the RTC to the date & time this sketch was compiled
		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
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
	if (debug){
		Serial.println("LoopHead");
	}
	//Get the Date/Time from the RTC
	DateTime now = rtc.now();
	//Get Sunrise/Sunset for the current year/month/day (THE TRUE is passing in Daylight Savings Time!
	/*TODO: Check to see if we're in Daylight savings time!*/
	int sunrise = colton.sunrise(now.year(), now.month(), now.day(), true);
	int sunset = colton.sunset(now.year(), now.month(), now.day(), true);
	//the int's sunrise and sunset now contain seconds from midnight 
	char riseTime[6];
	char setTime[6];
	//Dusk2Dawn can spit out character arrays of "readable" times, cool for debugging, not useful in code.
	Dusk2Dawn::min2str(riseTime, sunrise);
	Dusk2Dawn::min2str(setTime, sunset);
	if (debug) {
		Serial.println(riseTime); 
		Serial.println(setTime);
	}
	/* Loop ToDo: 
	 * Figure out how many seconds from midnight we are right now using the RTC
	 * Subtract the number of seconds to sunrise and sunset from that result to see if we should be up or down
	 * Compare door state to what it should be
	 * If states don't match raise or lower the door until the switches indicate it's closed or open
	 * Limit motor run time to ??? (this is probably going to be in the open / close subroutines?
	 * Add in an Over-ride: If they push the button over ride to the opposite of the current state: Record the override (overRideOpen or overRideClose) and open/close the door.
	 * Check the override when considering if we open or close time based. If the door is closed and we overRideOpen we ignore all close requests but as soon as the time conidtion matches the override (overRideOpen && DoorShouldBeOpen) we clear the override. The door shuts normally and opens normally next time cycle
	 * Add a temperature sensor
	 * If the temp sensor is below 30*F the door does not open unless override is used. Do not clear override if temp is below (causing door to not open)
	 */
	/* This is just temporary RTC Testing Code*/
	Serial.print(now.year(), DEC);
	Serial.print('/');
	Serial.print(now.month(), DEC);
	Serial.print('/');
	Serial.print(now.day(), DEC);
	Serial.print(" (");
	Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
	Serial.print(") ");
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.println();
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