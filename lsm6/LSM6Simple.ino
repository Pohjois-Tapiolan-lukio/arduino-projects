/*
The sensor outputs provided by the library are the raw
16-bit values obtained by concatenating the 8-bit high and
low accelerometer and gyro data registers. They can be
converted to units of g and dps (degrees per second) using
the conversion factors specified in the datasheet for your
particular device and full scale setting (gain).

Example: An LSM6DS33 gives an accelerometer Z axis reading
of 16276 with its default full scale setting of +/- 2 g. The
LA_So specification in the LSM6DS33 datasheet (page 11)
states a conversion factor of 0.061 mg/LSB (least
significant bit) at this FS setting, so the raw reading of
16276 corresponds to 16276 * 0.061 = 992.8 mg = 0.9928 g.
*/

#include <Wire.h>
#include <LSM6.h>

LSM6 imu;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();
}

void loop()
{
  imu.read();
  double roll, pitch;

  convertAngle(roll, imu.a.y, imu.a.z);
  convertAngle(pitch, imu.a.x, imu.a.z);

  Serial.print("Roll ");
  Serial.print(roll*180.0/M_PI);
  Serial.print("\tPitch ");
  Serial.println(pitch*180.0/M_PI);

  delay(100);
}

void convertAngle(double& loc, int val1, int val2)
{
  loc = atan((double)val1 / (double)val2);
}
