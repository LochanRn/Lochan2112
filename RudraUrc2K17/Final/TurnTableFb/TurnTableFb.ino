#include <SnapFilter.h>
SnapFilter s(0.007,8,255);   

int CH[2] = {A0, A1};
int mapper[2] = {};
int pulse;

void setup() {
  pinMode(CH[0],OUTPUT);
  pinMode(CH[1],OUTPUT);
  Serial3.begin(9600);  // Turn Table
}

void loop() {
  rc();
  feedback(mapper[0],mapper[1]);
}
void rc(){
  pulse = pulseIn(CH[0],HIGH);
  mapper[0] = map(pulse, 1969, 1126, 0, 1024);        // TT
  mapper[0] = s.Smooth(mapper[0]);  
  mapper[1] = analogRead(CH[1]);                      // TT Fb 
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
