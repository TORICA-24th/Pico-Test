uint32_t last_send_time = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(25,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() - last_send_time >= 4000){
    Serial1.print("Hello");
    last_send_time = millis();
  }
  if(Serial1.available()){
    String data = Serial1.readStringUntil('\n');
    if(data == "Hello"){
      for(int i = 0;i < 5;++i){
        digitalWrite(25,HIGH);
        delay(50);
        digitalWrite(25,LOW);
        delay(50);
      }
    }
   /* for(int i = 0;i < 5;++i){
      digitalWrite(25,HIGH);
      delay(50);
      digitalWrite(25,LOW);
      delay(50);
    }*/
  }
}
