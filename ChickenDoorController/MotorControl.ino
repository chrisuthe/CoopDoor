//Wind the Door Up
void raiseDoor() {
	digitalWrite(DoorUpMotorPin, HIGH);
	digitalWrite(doorDownMotorPin, LOW);
	if (debug) {
		Serial.println("Door Raising");
	}
}

//Wind The Door Down
void lowerDoor() {
	digitalWrite(doorDownMotorPin, HIGH);
	digitalWrite(DoorUpMotorPin, LOW);
	if (debug) {
		Serial.println("Door Lowering");
	}
}

//Stop the Door
void stopDoor() {
	digitalWrite(DoorUpMotorPin, LOW);
	digitalWrite(doorDownMotorPin, LOW);
	if (debug) {
		Serial.println("Door Stop");
	}
}