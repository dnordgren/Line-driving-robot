#include <Servo.h>
#include <boolean.h>

#define LED_RED 2
#define LED_GREEN 3
#define LED_BLUE 4

#define BUTTON 7

// servos defined while looking from power cord
#define SERVO_LEFT 9
#define SERVO_RIGHT 10

// sensors defined while looking from power cord
#define SENSOR_LEFT A0
#define SENSOR_RIGHT A1

Servo leftServo, rightServo;
// TODO : make these local to setup()

void setup () {
	Serial.begin(9600);
	
	pinMode(BUTTON, INPUT_PULLUP);
	
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	
	leftServo.attach(SERVO_LEFT);
	rightServo.attach(SERVO_RIGHT);
	
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, HIGH);
    delay(250);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    delay(250);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, HIGH);
    delay(250);
    digitalWrite(LED_BLUE, LOW);
		
	run();
}

void loop () {
	// printSensors();
}

void run() {
	// par down speed
	float speedScalar = 0.05;
	// define servo rotation speed
	float speed = 90 * speedScalar;
	// set starting speed
	// adjust for slow left servo
	float leftSpeed = 90+speed;//+(0.5*(1/speedScalar));
	float rightSpeed = 90-speed;
	
	// define max speed based on input speed
	// float leftMaxSpeed = leftSpeed*1.2;
// 	if (leftMaxSpeed > 180) {
// 		leftMaxSpeed = 180;
// 	}
// 	float rightMaxSpeed = rightSpeed*0.8;
	
	while (true) {
		// stop 
		if (buttonPressed()) {
			stop();
			break;
		}
		
		// TODO : friction coefficient
	
		// TODO : will need to account for changing values as sensor gets closer to edge of line; turn delay will need to be less
		// left is near black
		if (readLeftSensor() > 875) {
			// slow left; speed right
			leftSpeed = leftSpeed * 0.8;
			rightSpeed = rightSpeed * 0.8;
		}
		// right is near black
		else if (readRightSensor() > 805) {
			// slow right; speed left
			rightSpeed = rightSpeed * 1.2;
			leftSpeed = leftSpeed * 1.2;
		}// else {
// 			// on the correct course; speed up
// 			rightSpeed = rightSpeed * 0.8;	
// 			leftSpeed = rightSpeed * 1.2;
// 		}
		if (leftSpeed > 180) {
			leftSpeed = 180;
		}
		if (leftSpeed < 90) {
			leftSpeed = 90;
		}
		if (rightSpeed > 90) {
			rightSpeed = 90;
		}
		rightServo.write(rightSpeed);
		leftServo.write(leftSpeed);
	}
	
	// TODO : if both sensors are on black, switch modes
}

void stop() {
	leftServo.write(90);
	rightServo.write(90);
}

int readLeftSensor() {
	int sensorLeft = analogRead(SENSOR_LEFT);
	return sensorLeft;
}

int readRightSensor() {
	int sensorRight = analogRead(SENSOR_RIGHT);
	return sensorRight;
}

int readButton() {
	int buttonVal = digitalRead(BUTTON);
	return buttonVal;
}

bool buttonPressed() {
	int buttonVal = readButton();
	bool out = (buttonVal==0) ? true : false;
	return out;
}

// left (port)
void blinkRed() {
	digitalWrite(LED_RED, HIGH);
	//delay(turnDelay);
	digitalWrite(LED_RED, LOW);
}

// right (starboard)
void blinkGreen() {
	digitalWrite(LED_GREEN, HIGH);
	//delay(turnDelay);
	digitalWrite(LED_GREEN, LOW);
}

// mode switched
void blinkBlue() {
	digitalWrite(LED_BLUE, HIGH);
	delay(100);
	digitalWrite(LED_BLUE, LOW);
}

void printSensors() {
	int sensorLeft = readLeftSensor();
    int sensorRight = readRightSensor();
    Serial.print("Left sensor: ");
    Serial.println(sensorLeft);
    Serial.print("Right sensor: ");
    Serial.println(sensorRight);

    delay(1500);
}