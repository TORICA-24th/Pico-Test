#include <Wire.h>
// #include <Adafruit_Sensor.h>

#include <Adafruit_DPS310.h>
Adafruit_DPS310 dps;
sensors_event_t temp_event, pressure_event;

volatile float data_main_dps_pressure_hPa = 0;
volatile float data_main_dps_temperature_deg = 0;
volatile float data_main_dps_altitude_m = 0;

char DPS[256];

volatile uint8_t last_send_time;

void led()
{
  digitalWrite(25, HIGH);
  delay(100);
  digitalWrite(25, LOW);
  delay(100);
}

void read_main_dps()
{
  if (!(dps.temperatureAvailable() && dps.pressureAvailable()))
  {
    return;
  }
  dps.getEvents(&temp_event, &pressure_event);
  data_main_dps_pressure_hPa = pressure_event.pressure;
  data_main_dps_temperature_deg = temp_event.temperature;
  data_main_dps_altitude_m = (powf(1013.25 / data_main_dps_pressure_hPa, 1 / 5.257) - 1) * (data_main_dps_temperature_deg + 273.15) / 0.0065;
  sprintf(DPS, "%+06.2f", data_main_dps_pressure_hPa);
  Serial.println(DPS);
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(25, OUTPUT);

  Serial.begin(115200);
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.setClock(400000);
  // Wire.begin();

  if (!dps.begin_I2C(0x77, &Wire))
  { // Can pass in I2C address here
    // if (! dps.begin_SPI(DPS310_CS)) {  // If you want to use SPI
    Serial.println("Failed to find DPS");
    while (1)
    {
      led();
    }
  }
  dps.configurePressure(DPS310_32HZ, DPS310_16SAMPLES);
  dps.configureTemperature(DPS310_32HZ, DPS310_2SAMPLES);
  Serial.println("DPS OK!");

  for (int i = 0; i < 5; ++i)
  {
    led();
  }
  last_send_time = millis();
}

void loop()
{
  // put your main code here, to run repeatedly:
  read_main_dps();
  delay(200);
}
