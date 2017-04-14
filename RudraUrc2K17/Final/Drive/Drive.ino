float a = 0.5;
int ch[2] = {A1, A3}; //fb, rl
int mapper[2] = {}, f_map[2] = {};
int rcmax[2] = { 1883, 1883 };
int rcmin[2] = { 1033, 1033 };
int sensi = 50; // sensitivity

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  for (int i = 0; i < 2; i++)
    pinMode(ch[i], INPUT);
  delay(100);
}

void loop()
{
  rc();
  alg();
  delay(10);
}

void rc()
{
  for (int i = 0 ; i < 2; i++) {
    mapper[i] = pulseIn(ch[i], HIGH);    
}
  mapper[0] = map( mapper[0], rcmin[0], rcmax[0], sensi, -sensi);
  mapper[0] = safe(mapper[0]);

  mapper[1] = map( mapper[1], rcmin[1], rcmax[1], sensi, -sensi);
  mapper[1] = safe(mapper[1]);
}

int safe(int y) {
  return (y < 10 && y > -10) ? 0 : y;
}

void alg() {

  f_map[0] = f_map[0] * a + (1 - a) * mapper[0];
  f_map[1] = f_map[1] * a + (1 - a) * mapper[1];
  Left( f_map[0] + f_map[1] );
  Right( f_map[0] - f_map[1] );
  Serial.println();

}
void Left(int t)
{
  int x = map(t, -100, 100, 1, 127); // forward
  int y = map(t, -100, 100, 128, 256); // backward
  Serial.print(" L ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");

  Serial1.write(x); 
  delay(5);
  Serial1.write(y); 
  delay(5);
}
void Right(int t)
{
  int x = map(t, -100, 100, 1, 127); 
  int y = map(t , -100, 100, 128, 256); 

  Serial.print(" R ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");

  Serial2.write(x); // left front
  delay(5);
  Serial2.write(y); // left back
  delay(5);
}
