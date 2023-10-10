#include <Geometry.h>
using namespace Geometry;
using namespace BLA;

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);

volatile float data_main_bno_accx_mss = 0;
volatile float data_main_bno_accy_mss = 0;
volatile float data_main_bno_accz_mss = 0;
volatile float data_main_bno_qw = 0;
volatile float data_main_bno_qx = 0;
volatile float data_main_bno_qy = 0;
volatile float data_main_bno_qz = 0;
volatile float data_main_bno_roll = 0;
volatile float data_main_bno_pitch = 0;
volatile float data_main_bno_yaw = 0;

char data[512];

void setup() {
  // put your setup code here, to run once:
  pinMode(25,OUTPUT);
  Serial.begin(115200);
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.setClock(400000);
  for(int i=0;i<50;++i){
    led();
  }
  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1) {
      out_led();
    }
  }
  for(int i=0;i<100;++i){
    led();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t current_time = millis();
  static uint32_t last_send_time = 0;
  read_main_bno();
  if(current_time - last_send_time >= 1000){
    sprintf(data,"roll(left+)   pitch   yaw\n%+06.2f        %+06.2f  %+06.2f\n", data_main_bno_roll, data_main_bno_pitch, data_main_bno_yaw);
    Serial.println(data);
    last_send_time = millis();
  } 
}

void led(){
  digitalWrite(25,HIGH);
  delay(10);
  digitalWrite(25,LOW);
  delay(10);
}

void out_led(){
  digitalWrite(25,HIGH);
  delay(500);
  digitalWrite(25,LOW);
  delay(500);
}

void read_main_bno() {
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Quaternion quat = bno.getQuat();
  //imu::Vector<3> magnet = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);    magnet.x()
  //imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);         gyro.x()
  //imu::Vector<3> ground_acc = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL); ground_acc.x()
  data_main_bno_accx_mss = accel.x();
  data_main_bno_accy_mss = accel.y();
  data_main_bno_accz_mss = accel.z();
  data_main_bno_qw = quat.w();
  data_main_bno_qx = quat.x();
  data_main_bno_qy = quat.y();
  data_main_bno_qz = quat.z();
  Quaternion qua(data_main_bno_qx, data_main_bno_qy, data_main_bno_qz, data_main_bno_qw);
  EulerAngles euler(qua.to_rotation_matrix());
  data_main_bno_roll = -(euler.first() * 180 / 3.1415);
  data_main_bno_pitch = euler.second() * 180 / 3.1415;
  data_main_bno_yaw = euler.third() * 180 / 3.1415;
}
