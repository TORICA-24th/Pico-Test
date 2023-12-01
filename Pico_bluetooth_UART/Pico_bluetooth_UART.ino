#include <SerialBT.h>

void setup()
{
  Serial.begin(115200);
  SerialBT.begin(115200);

  pinMode(5,OUTPUT);
}

void loop()
{
  if(SerialBT.available()){
    digitalWrite(5,HIGH);
    SerialBT.println("Hello");
  }
}
