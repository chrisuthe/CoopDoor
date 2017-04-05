//Wind the Door Up
void raiseDoor() {
	digitalWrite(doorUpPin, HIGH);
	digitalWrite(doorDownPin, LOW);
	if (debug) {
		Serial.println("Door Up");
	}
	delay(10000);
	digitalWrite(doorUpPin, LOW);
	if (debug) {
		Serial.println("Stop Up");
	}
}

//Wind The Door Down
void lowerDoor() {
	digitalWrite(doorDownPin, HIGH);
	digitalWrite(doorUpPin, LOW);
	if (debug) {
		Serial.println("Door Down");
	}
	delay(10000);
	digitalWrite(doorDownPin, LOW);
	if (debug) {
		Serial.println("Stop Down");
	}
}
