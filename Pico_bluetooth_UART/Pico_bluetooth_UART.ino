#include <SerialBT.h>

void setup()
{
  Serial.begin(115200);
  SerialBT.begin(115200);
}

void loop()
{

  while(SerialBT)
  {
    /*//UARTにデータがあれば読み出して、BluetoothUARTに送信
    if(Serial.available())
    {
      SerialBT.write(Serial.read());
    }
    */
    //BluetoothUARTにデータがあれば読み出して、UARTに送信
    SerialBT.println("My name is raspberry pi pico");
    delay(1000);
  }
}
