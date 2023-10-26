void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  pinMode(25,OUTPUT);

} 

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available()){
    digitalWrite(25,HIGH);
    String data = Serial1.readStringUntil('\n');
    Serial1.print(data);
    digitalWrite(25,LOW);
  }
}
