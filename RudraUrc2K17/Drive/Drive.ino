float a = 0.5;
int left,right,l,r,f_auto[2] = {};
String s,s1,s2;
int ch[3] = {A1, A3, A2}; //fb, rl ,shift
int mapper[3] = {}, f_map[2] = {};
int rcmax[3] = { 1048, 1050, 1048 };
int rcmin[3] = { 1890, 1885, 1887 };
int sensi = 70; // sensitivity
  
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  
  for (int i = 0; i < 3; i++)
    pinMode(ch[i], INPUT);
  delay(100);
}

void loop() {
  rc();
  if(mapper[2] < 0) 
    autobot_run();
  else  
    alg();
}

void rc() {
    for (int i = 0 ; i < 3; i++) 
    mapper[i] = pulseIn(ch[i], HIGH);
  
  mapper[0] = map( mapper[0], 1035, 1883, -sensi, sensi);
  mapper[0] = safe(mapper[0]);

  mapper[1] = map( mapper[1], 1038, 1883, -sensi, sensi);
  mapper[1] = safe(mapper[1]);

  mapper[2] = map( mapper[2], 1034, 1881, -100, 100);
}


int safe(int y) {
  return (y < 10 && y > -10) ? 0 : y;
}
void autobot_run() {
  if (Serial.available()>0) {
    s = Serial.readString();
    for(int i=0;i<s.length();i++)
      if(s[i]=='L') l=i;
      else if(s[i]=='R')  r=i;
      else  continue;
    s1 = s.substring(l+1,r);
    s2 = s.substring(r+1,s.length());
    left = parseToInt(s1);
    right = parseToInt(s2);
    
    Serial3.print(left);
    Serial3.print(" ");
    Serial3.println(right);
    
    Left(left);
    Right(right);
  }
}
void alg() {
  f_map[0] = f_map[0] * a + (1-a) * mapper[0];
  f_map[1] = f_map[1] * a + (1-a) * mapper[1];
  Left( f_map[0] + f_map[1] );
  Right( f_map[0] - f_map[1] );
}

void Left(int t)
{
  
  int x = map(-t, -100, 100, 1, 127); // forward
  int y = map(t, -100, 100, 128, 256); // backward

  Serial3.print(" L ");
  Serial3.print(x);
  Serial3.print(" ");
  Serial3.print(y);
  Serial3.print(" ");

  Serial1.write(x); // right back
  delay(5);
  Serial1.write(y); // right front
  delay(5);
}
void Right(int t)
{
  int x = map(-t, -100, 100, 1, 127); // forward
  int y = map(t , -100, 100, 128, 256); // backward

  Serial3.print(" R ");
  Serial3.print(x);
  Serial3.print(" ");
  Serial3.print(y);
  Serial3.println(" ");

  Serial2.write(x); // left front
  delay(5);
  Serial2.write(y); // left back
  delay(5);
}

int parseToInt(String s)
{
   int x[] = {0,0,0,0};   
   int i,j = 0,sum = 0;
   if(s[0] == '-') {
       for (i = (s.length()-1); i > 0; i--,j++) 
         x[j] = s[i] - '0';
         
       sum = x[0] + x[1]*10 +x[2]*100;
       return -sum;
    }
    else {
       for (i = (s.length()-1); i >= 0; i--,j++) 
         x[j] = s[i] - '0';
         
      sum = x[0] + x[1]*10 +x[2]*100;
      return sum;   
  }
}
