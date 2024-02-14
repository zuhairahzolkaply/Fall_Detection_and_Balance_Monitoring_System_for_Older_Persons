#include <Wire.h>
#include <MPU6050.h>
#define buzzer 6
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
char msg;
char call;

MPU6050 mpu;
int LED = 9;

void setup() 
{
  pinMode(buzzer , OUTPUT);
  digitalWrite(buzzer , LOW);
  mySerial.begin(115200);
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  checkSettings();
}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
            
  Serial.println();
  
  Serial.print(" * Gyroscope:         ");
  switch(mpu.getScale())
  {
    case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
    case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
    case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
    case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  } 
  
  Serial.print(" * Gyroscope offsets: ");
  Serial.print(mpu.getGyroOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getGyroOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getGyroOffsetZ());
  
  Serial.println();
}

void loop()
{
  
  Vector normAccel = mpu.readNormalizeAccel();
  Vector rawGyro = mpu.readRawGyro();

  

  Serial.print(" Xaccel = ");
  Serial.print(normAccel.XAxis);
  Serial.print(" Yaccel = ");
  Serial.print(normAccel.YAxis);
  Serial.print(" Zaccel = ");
  Serial.println(normAccel.ZAxis);
  ("m/s^2 ");

  Serial.print(" Xgyro = ");
  Serial.print(rawGyro.XAxis);
  Serial.print(" Ygyro = ");
  Serial.print(rawGyro.YAxis);
  Serial.print(" Zgyro = ");
  Serial.println(rawGyro.ZAxis);
  ("deg/sec ");
  
  delay(10);

  if(normAccel.XAxis>15 ^ normAccel.YAxis>15 ^ normAccel.ZAxis>15)
   
   {
      Serial.println("fall detected."); 
      digitalWrite (buzzer , LOW); MakeCall();
      delay(500);
      
   }
    else if (normAccel.XAxis<15 ^ normAccel.YAxis<15 ^ normAccel.ZAxis<15  ) 
   {
     digitalWrite(buzzer , LOW); 
   }

   
   
   if (rawGyro.XAxis>2500 ^ rawGyro.YAxis>2500 ^ rawGyro.ZAxis>2500  ) 
  {
     Serial.println("not balanced"); 
      digitalWrite (buzzer , HIGH); 
      delay(500);    
   }
   else if (rawGyro.XAxis<2500 ^ rawGyro.YAxis<2500 ^ rawGyro.ZAxis<2500  ) 
   {
     digitalWrite(buzzer , LOW); 
   } 
    if (mySerial.available()>0)
 Serial.write(mySerial.read());
   delay(500);
}

void MakeCall()
{
  mySerial.println("ATD+60182894938;"); 
  Serial.println("Calling caretakers "); // print response over serial port
  delay(1000);
}
void message(void)
{
mySerial.println("AT+CMGF=1");
  delay(100);
  mySerial.println("AT+CMGS=\"+60182894938\"\r");
  delay(100);
  mySerial.println("I am SMS from GSM module. Fall is detected");
  delay(100);
  mySerial.println((char)26);
  delay(100);
}
