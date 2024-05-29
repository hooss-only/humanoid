#include <Servo.h>
#include <IRremote.h>

Servo eyesVerticalServo;
Servo eyesHorizontalServo;
Servo headSpinServo;
Servo jawOpenServo;

int eyesVertical = 120;
int eyesHorizontal = 150; // 90 ~ 180
const int EYES_SENSITIVE = 10;

int headSpin = 90; // 90 = stop 
int jawOpen = 90; // 90 = stop
// int eyesHorizontal = 150; // 90 ~ 180
const int HEAD_SPIN_SENSITIVE = 10;
const int JAW_OPEN_SENSITIVE = 10;

const int SPIN_MOTOR_MINIMUM = 0;
const int SPIN_MOTOR_MAXIMUM = 180;


const long EYES_VERTICAL_PIN = 4;
const long EYES_HORIZONTAL_PIN = 5;
const long RECV_PIN = 7;
const long HEAD_SPIN_PIN = 8;
const long JAW_OPEN_PIN = 9;

const int EYES_VERTICAL_MINIMUM = 60;
const int EYES_VERTICAL_MAXIMUM = 160;

const int EYES_HORIZONTAL_MINIMUM = 110;
const int EYES_HORIZONTAL_MAXIMUM = 190;

IRrecv irrecv(RECV_PIN);

void setup() {
	eyesVerticalServo.attach(EYES_VERTICAL_PIN);
	eyesHorizontalServo.attach(EYES_HORIZONTAL_PIN);
	headSpinServo.attach(HEAD_SPIN_PIN);
	jawOpenServo.attach(JAW_OPEN_PIN);
	Serial.begin(9600);
	IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
	moveEyes();
}

bool controlMode = false;
bool eyesMode = false;
bool headMode = false;
void loop() {
	RemoteControlHandler();
}

void RemoteControlHandler() {
	if (IrReceiver.decode()) {
		String irNum = String(IrReceiver.decodedIRData.command, HEX);
		Serial.println(irNum);
		
		if (!controlMode) {
			if (irNum == "c") {
				eyesMode = true;
				controlMode = true;
				Serial.println("Eyes mode enabled");
			} else if (irNum == "18") {
				headMode = true;
				controlMode = true;
				Serial.println("Head mode enabled");
			}
		} else {
			if (eyesMode) {
				eyesModeHandler(irNum);
			} else if (headMode) {
				headModeHandler(irNum);
			}
		}
		IrReceiver.resume();
	}
}

void eyesModeHandler(String irNum) {
	if (irNum == "46") {
		eyesVertical -= EYES_SENSITIVE;
	} else if (irNum == "44") {
		eyesHorizontal += EYES_SENSITIVE;
	} else if (irNum == "43") {
		eyesHorizontal -= EYES_SENSITIVE;
	} else if (irNum == "15") {
		eyesVertical += EYES_SENSITIVE;
	} else if (irNum == "16") {
		eyesMode = false;
		controlMode = false;
		Serial.println("Eyes mode disabled");
	} 

	if (eyesVertical < EYES_VERTICAL_MINIMUM) {
		eyesVertical = EYES_VERTICAL_MINIMUM;
	} else if (eyesVertical > EYES_VERTICAL_MAXIMUM) {
		eyesVertical = EYES_VERTICAL_MAXIMUM;
	}

	if (eyesHorizontal < EYES_HORIZONTAL_MINIMUM) {
		eyesHorizontal = EYES_HORIZONTAL_MINIMUM;
	} else if (eyesHorizontal > EYES_HORIZONTAL_MAXIMUM) {
		eyesHorizontal = EYES_HORIZONTAL_MAXIMUM; }

	Serial.println(eyesHorizontal);
	Serial.println(eyesVertical);
	moveEyes();
}

void headModeHandler(String irNum) {
	if (irNum == "46") {
		jawOpen += JAW_OPEN_SENSITIVE;
	} else if (irNum == "44") {
		headSpin += HEAD_SPIN_SENSITIVE;
	} else if (irNum == "43") {
		headSpin -= HEAD_SPIN_SENSITIVE;
	} else if (irNum == "15") {
		jawOpen -= JAW_OPEN_SENSITIVE;
	} else if (irNum == "16") {
		headMode = false;
		controlMode = false;
		Serial.println("Head mode disabled");
		headSpin = 90;
		jawOpen = 90;
	} else if (irNum == "40") {
		headSpin = 90;
		jawOpen = 90;
	}

	if (headSpin < SPIN_MOTOR_MINIMUM) {
		headSpin = SPIN_MOTOR_MINIMUM;
	} else if (headSpin > SPIN_MOTOR_MAXIMUM) {
		headSpin = SPIN_MOTOR_MAXIMUM;
	}
	
	Serial.println(headSpin);
	Serial.println(jawOpen);

	moveHead();
}

void moveHead() {
	headSpinServo.write(headSpin);
	jawOpenServo.write(jawOpen);
}

void moveEyes() {
	eyesHorizontalServo.write(eyesHorizontal);
	eyesVerticalServo.write(eyesVertical);
}
