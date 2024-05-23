#include <Servo.h>
#include <IRremote.h>

Servo eyesVerticalServo;
Servo eyesHorizontalServo;
Servo headSpinServo;
int eyesVertical = 0;
int eyesHorizontal = 130; // 90 ~ 180
const int EYES_SENSITIVE = 10;

const long EYES_VERTICAL_PIN = 4;
const long EYES_HORIZONTAL_PIN = 5;
const long RECV_PIN = 7;
const long HEAD_SPIN_PIN = 8;

const int EYES_HORIZONTAL_MINIMUM = 110;
const int EYES_HORIZONTAL_MAXIMUM = 180;

IRrecv irrecv(RECV_PIN);

void setup() {
	eyesVerticalServo.attach(EYES_VERTICAL_PIN);
	eyesHorizontalServo.attach(EYES_HORIZONTAL_PIN);
	headSpinServo.attach(HEAD_SPIN_PIN);
	Serial.begin(9600);
	IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
}

bool eyesMode = false;
int test = 0;
void loop() {
	RemoteControlHandler();
}

void RemoteControlHandler() {
	if (IrReceiver.decode()) {
		String irNum = String(IrReceiver.decodedIRData.command, HEX);
		Serial.println(irNum);

		if (irNum == "c") {
			eyesMode = true;
			Serial.println("Eyes mode enabled");
		} else if (eyesMode) {
			if (irNum == "46") {
				eyesVertical += EYES_SENSITIVE;
			} else if (irNum == "44") {
				eyesHorizontal += EYES_SENSITIVE;
			} else if (irNum == "43") {
				eyesHorizontal -= EYES_SENSITIVE;
			} else if (irNum == "15") {
				eyesVertical -= EYES_SENSITIVE;
			} else if (irNum == "16") {
				eyesMode = false;
				Serial.println("Eyes mode disabled");
			}

			if (eyesVertical < 0) {
				eyesVertical = 0;
			}
			
			if (eyesHorizontal < EYES_HORIZONTAL_MINIMUM) {
				eyesHorizontal = EYES_HORIZONTAL_MINIMUM;
			} else if (eyesHorizontal > EYES_HORIZONTAL_MAXIMUM) {
				eyesHorizontal = EYES_HORIZONTAL_MAXIMUM;
			}

			Serial.println(eyesHorizontal);
			moveEyes();
		}

		IrReceiver.resume();
	}
}

void moveEyes() {
	eyesHorizontalServo.write(eyesHorizontal);
	eyesVerticalServo.write(eyesVertical);
}
