#include <Servo.h>
#include "SnapFilter.h"

SnapFilter s(0.007,8,255);   

int GRP1[] = { 22, 24, 2};
int GRP2[] = { 27, 25, 3};                        // A, B, PWM Griper pins
int LA1[] =  { 34, 35, 8};                        // A, B, PWM long 0-1 up
int LA2[] =  { 36, 37, 9};                        // A, B, PWM short 1-0 up
int PR1[] =  { 49, 48, 10};                       // A, B, PWM Pitch and Roll 
int PR2[] =  { 33, 34, 7};                        // A, B, PWM Pitch and Roll

int CH[] = { A11, A10, A9, A8, A5, A6, A7 ,A0};   //Long LA,Short LA,Pitch,TT,Yaw,Roll,TTFb
float a = 0.5, py;
int pulse[7] = {};
int mapper[8] = {};

Servo yaw;

int i = 0;

void setup() {
  for (i = 0; i < 7; i++)   // RC INPUT
    pinMode(CH[i], INPUT);
  pinMode(CH[8],OUTPUT);
  for (i = 0; i < 3; i++) {  // LA outputs
    pinMode(LA1[i], OUTPUT);
    pinMode(LA2[i], OUTPUT);
    pinMode(GRP1[i], OUTPUT);
    pinMode(GRP2[i], OUTPUT);
    pinMode(PR1[i], OUTPUT);
    pinMode(PR2[i], OUTPUT);
  }
  yaw.attach(12);
  Serial3.begin(9600);  // Turn Table
}

void loop() {  
  rc();
    
  control(mapper[1], 25, LA1, abs(mapper[1]));          //Long LA
  control(mapper[0], 25, LA2, abs(mapper[0]));          //Short LA
  
  control(mapper[6], 25, GRP1, 100);                    //Gripper
  control(mapper[6], 25, GRP2, 100);               

  control(mapper[2], 25, PR1, 100);                      //Pitch 
  control(mapper[2], 25, PR2, 100);

  control(-mapper[5],25, PR1, 100);                      //Roll 
  control(mapper[5], 25, PR2, 100);
  
  feedback(mapper[3],mapper[7]);                        //Feedback with rc
  
  yaw.writeMicroseconds(mapper[4]);
}
void rc() {
  for (i = 0; i < 7; i++) {
    pulse[i] = pulseIn(CH[i], HIGH);
  }
  mapper[0] = map(pulse[0], 1133, 1967, 200, -200);   // long LA
  mapper[1] = map(pulse[1], 1968, 1126, 200, -200);   // short LA
  
  mapper[3] = map(pulse[3], 1969, 1126, 0, 1024);     // TT
  mapper[3] = s.Smooth(mapper[3]);
  mapper[7] = analogRead(CH[7]);                      // TT Fb 
  
  mapper[2] = map(pulse[2], 1917, 1076, 200, -200);   // pitch
  mapper[4] = map(pulse[4], 1050, 1871, 1500, 1200);  // yaw
  mapper[5] = map(pulse[5], 1046, 1883, 200, -200);   // roll
  
  mapper[6] = map(pulse[6], 1034, 1890, -100, 100);   // grp
  
}

int safe(int y) {
  return (y < 10 && y > -10) ? 0 : y;
}

void feedback(int rc,int fb){
  if(rc == fb)   turntable(0);
  else if(rc>fb) turntable(50);
  else if(rc<fb) turntable(-50);
  else turntable(0);
}
void turntable(int x) {
  int y = map(x, -100, 100, 128, 256); 
  py = py * a + (1 - a) * y;
  Serial.println(y);
  Serial3.write((int)py);
  delay(10);
}

void control(int inp, int lim, int M[], int pwm) {
  if (abs(inp) <= lim)  act(M, 0, 0, 0);
  else if (inp >  lim)  act(M, 0, 1, pwm);
  else if (inp < -lim)  act(M, 1, 0, pwm);
  else act(M, 0, 0, 0);
}

void act(int arr[], boolean A, boolean B, int pwm) {
  digitalWrite(arr[0], A);
  digitalWrite(arr[1], B);
  analogWrite(arr[2], pwm);
}
