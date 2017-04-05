/*
 Name:		ChickenDoorController.ino
 Created:	4/3/2017 9:16:16 PM
 Author:	Chris Uthe
*/
#include <math.h>
#include <Dusk2Dawn.h>
#include <eeprom.h>
#include <Wire.h>
#include <eBtn.h>


//Define the pins connecting to the motor driver
const int doorUpPin = 3;
const int doorDownPin = 5;
const int btnPin = 2;
eBtn btn = eBtn(btnPin);


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
	pinMode(doorUpPin, OUTPUT);
	pinMode(doorDownPin, OUTPUT);
	//Set Serial for Debugging
	if (debug) {
		Serial.begin(9600);
		Serial.println("FinishedSetup");
	}

}


// the loop function runs over and over again until power down or reset
void loop() {
	if (debug){
		Serial.println("LoopHead");
	}
	int sunrise = colton.sunrise(2017, 4, 4, true);
	int sunset = colton.sunset(2017, 4, 4, true);
	if (debug) {
		Serial.println(sunrise);
		Serial.println(sunset);
	}
	char riseTime[6];
	char setTime[6];
	Dusk2Dawn::min2str(riseTime, sunrise);
	Dusk2Dawn::min2str(setTime, sunset);
	if (debug) {
		Serial.println(riseTime); 
		Serial.println(setTime);
	}
	raiseDoor();
	lowerDoor();
	delay(runTime);
}