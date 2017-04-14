#include <Servo.h>

int GRP1[] = { 22, 24, 2};
int GRP2[] = { 27, 25, 3};                        // A, B, PWM Griper pins
int LA1[] =  { 34, 35, 8};                        // A, B, PWM long 0-1 up
int LA2[] =  { 36, 37, 9};                        // A, B, PWM short 1-0 up

int CH[] = { A11, A10, A9, A8, A5, A6, A7 };     
float a = 0.5, py;
int pulse[7] = {};
int mapper[7] = {};

Servo yaw, pitch, roll;

int i = 0;

void setup() {
  for (i = 0; i < 7; i++)   // RC INPUT
    pinMode(CH[i], INPUT);

  for (i = 0; i < 3; i++) {  // LA outputs
    pinMode(LA1[i], OUTPUT);
    pinMode(LA2[i], OUTPUT);
    pinMode(GRP1[i], OUTPUT);
    pinMode(GRP2[i], OUTPUT);
  }
  yaw.attach(12);
  pitch.attach(10);
  roll.attach(11);

  Serial.begin(9600);   //Debug
  Serial3.begin(9600);  // Turn Table
}

void loop() {
  rc();
    
  control(mapper[1], 25, LA1, abs(mapper[1]));
  control(mapper[0], 25, LA2, abs(mapper[0]));
  
  control(mapper[6], 25, GRP1, 100);
  control(mapper[6], 25, GRP2, 100);

  turntable(mapper[3]);

  pitch.writeMicroseconds(mapper[2]);
  yaw.writeMicroseconds(mapper[4]);
  roll.writeMicroseconds(mapper[5]);
}
void rc() {
  for (i = 0; i < 7; i++) {
    pulse[i] = pulseIn(CH[i], HIGH);
  }
  mapper[0] = map(pulse[0], 1133, 1967, 200, -200);   // long LA
  mapper[1] = map(pulse[1], 1968, 1126, 200, -200);   // short LA
  mapper[2] = map(pulse[2], 1917, 1076, 1400, 1600);  // pitch
  mapper[3] = map(pulse[3], 1969, 1126, 40, -40);     // TT
  mapper[4] = map(pulse[4], 1050, 1871, 1500, 1200);   // yaw
  mapper[5] = map(pulse[5], 1046, 1883, 1000, 2000);  // roll
  mapper[6] = map(pulse[6], 1034, 1890, -100, 100);   // grp
  mapper[3] = safe(mapper[3]);
}



int safe(int y) {
  return (y < 10 && y > -10) ? 0 : y;
}

void turntable(int x) {
  int y = map(x, -100, 100, 128, 256); 
  py = py * a + (1 - a) * y;
  delay(10);
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
