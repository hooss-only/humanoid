#include<Servo.h>

Servo eyesVerticalServo;
Servo eyesHorizontalServo;
int eyesVertical = 0;
int eyesHorizontal = 135; // 90 ~ 180

void setup() {
	eyesVerticalServo.attach(4);
	eyesHorizontalServo.attach(5);
	Serial.begin(9600);
}

bool a = true;
void loop() {
	if (a) {
		eyesHorizontal = 90;	
	} else {
		eyesHorizontal = 180;	
	}
	a = !a;

	Serial.println(eyesHorizontal);
	Serial.println(eyesVertical);
	Serial.println("");
	
	moveEyes(eyesHorizontal, eyesVertical);

	delay(1000);
}

void moveEyes(int horizontal, int vertical) {
	eyesHorizontalServo.write(horizontal);
	eyesVerticalServo.write(vertical);
}
