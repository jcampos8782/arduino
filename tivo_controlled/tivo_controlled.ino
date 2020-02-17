//www.elegoo.com

#include <IRremote.h>

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define RECV_PIN  12        //Infrared signal receiving pin
#define LED       13        //define LED pin

#define L         2701977607
#define R         2701961223
#define F         2701928455
#define B         2701944839
#define A         2701927435
#define D         2701935627
#define LED_OFF   2701924359
#define LED_ON    2701940743  
#define S 

unsigned long val; 
unsigned long speed = 0;
unsigned long lastDirection = F;

IRrecv irrecv(RECV_PIN);   //initialization
decode_results results;   //Define structure type

void accelerate() {
  speed += 75;

  if (speed > 255) {
    speed = 255;
  }
  
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void deccelerate() {
  speed -= 75;
  
  if (speed < 0) {
    speed = 0;
  }
  
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void leftWheelsForward(){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
}

void leftWheelsBack(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
}

void rightWheelsForward(){
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void rightWheelsBack(){
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void moveCarForward(){
  lastDirection = F;
  rightWheelsForward();
  leftWheelsForward();
}

void moveCarBack(){
  lastDirection = B;
  rightWheelsBack();
  leftWheelsBack();
}

void turnCarLeft(){
  rightWheelsForward();
  leftWheelsBack();
  delay(500); 
  if (lastDirection == F) {
    moveCarForward();
  } else {
    moveCarBack();
  }
}

void turnCarRight(){
  rightWheelsBack();
  leftWheelsForward();
  delay(500); 
  if (lastDirection == F) {
    moveCarForward();
  } else {
    moveCarBack();
  }
}

void setup() {
  pinMode(LED, OUTPUT); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Start the car in a resting position
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  moveCarForward();
  
  Serial.begin(9600);  // debug output at 9600 baud
  irrecv.enableIRIn();  // Start receiving
}

void loop() {
  if (irrecv.decode(&results)) { 
    Serial.println(val);
    
    switch (results.value) {
      case A: 
        accelerate();
        break;
      case D:
        deccelerate();
        break;
      case L:
        turnCarLeft();
        break;
      case R:
        turnCarRight();
        break;
      case F:
        moveCarForward();
        break;
      case B:
        moveCarBack();
        break;
      case LED_ON:
        digitalWrite(LED,HIGH);
        break;
      case LED_OFF:
        digitalWrite(LED,LOW);
        break;
    }
    
    irrecv.resume();
    delay(150);
  }
}
