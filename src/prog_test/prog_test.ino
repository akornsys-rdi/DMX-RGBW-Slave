void setup() {
  pinMode(12,INPUT);
  pinMode(13,OUTPUT);
  Serial.begin(115200);
  Serial3.begin(9600);
}

void loop() {
  byte yBuffer=0;
  
  delay(5000);
  pinMode(12,OUTPUT);
  Serial.println("init");
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  Serial.println("prog mode");
  digitalWrite(12,LOW);
  delay(1);
  Serial.println("sign");
  digitalWrite(13,HIGH);
  Serial3.write(0xFD);
  delay(2);
  digitalWrite(13,LOW);
  delay(10);
  Serial.println("read");
  digitalWrite(13,HIGH);
  Serial3.write(0xFA);
  delay(2);
  digitalWrite(13,LOW);
  delay(10);
//  Serial.println("write");
//  digitalWrite(13,HIGH);
//  Serial3.write(0xFB);
//  delay(2);
//  digitalWrite(13,LOW);
//  delay(10);
//  Serial.println("add");
//  digitalWrite(13,HIGH);
//  Serial3.write(0x01);
//  delay(2);
//  digitalWrite(13,LOW);
//  delay(10);
  do {
    while(Serial3.available() >= 1) {
      yBuffer=Serial3.read();
      Serial.println(yBuffer,HEX);
    }
  } while(1);
}
