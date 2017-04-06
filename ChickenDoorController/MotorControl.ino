//Wind the Door Up
void raiseDoor() {
	digitalWrite(doorUpPin, HIGH);
	digitalWrite(doorDownPin, LOW);
	if (debug) {
		Serial.println("Door Raising");
	}
}

//Wind The Door Down
void lowerDoor() {
	digitalWrite(doorDownPin, HIGH);
	digitalWrite(doorUpPin, LOW);
	if (debug) {
		Serial.println("Door Lowering");
	}
}

//Stop the Door
void stopDoor() {
	digitalWrite(doorUpPin, LOW);
	digitalWrite(doorDownPin, LOW);
	if (debug) {
		Serial.println("Door Stop");
	}
}