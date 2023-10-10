#include <TORICA_SD.h>
int cs_SD = 18;
TORICA_SD sd(cs_SD);

void setup() {
  // put your setup code here, to run once:
  sd.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  sd.add_str("Hello");
  sd.flash();
  delay(1000);
}
