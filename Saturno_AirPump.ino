// Air Pump Project V1.0
// Developed by: Enrique Saturno
// Contact: esaturno2@csus.edu

// Purpose:
// The purpose of this project is to control a DC air pump with a L293D motor driver, and display 
// the temperature of a fish tank in a LCD display.




///////LCD VARIABLE/LIBRARY SETUP////////////////////////////////////////////////////////
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
// set the LCD address to 0x27 for a 16 chars and 2 line display 
LiquidCrystal_PCF8574 lcd(0x27);
int show;
/////////////////////////////////////////////////////////////////////////////////////////
//3,5,4
///////CONNECTIONS FOR L293D DRIVER//////////////////////////////////////////////////////
int enA = 6; //ENABLE PIN
int in1 = 8; //INPUT PIN 1
int in2 = 7; //INPUT PIN 2
int speed_up = 11;
int speed_down = 10;
int motor_speed = 120; //INITIAL MOTOR SPEED
////////////////////////////////////////////////////////////////////////////////////////

///////POTENTIOMETER VARIABLES//////////////////////////////////////////////////////////
int potPin = 2;    // select the input pin for the potentiometer //PIN A2
int val = 0;       // variable to store the value coming from the sensor
////////////////////////////////////////////////////////////////////////////////////////

/////////TEMPERATURE VARIABLES//////////////////////////////////////////////////////////
int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
///////LCD INITIALIZATION SETUP////////////////////////////////////////////////////

///SERIALIZATION FOR DEBUGGING/////
  Serial.begin(9600);
///////////////////////////////////
  int error;
  Serial.println("LCD...");
  Wire.begin();
  Wire.beginTransmission(0x27); //LCD Address 
  error = Wire.endTransmission(); 
  Serial.print("Error: ");
  Serial.print(error);
  if (error == 0) 
  {
    Serial.println(": LCD found.");
  } else 
  {
    Serial.println(": LCD not found.");
  } // if
  lcd.begin(16, 2); // initialize the lcd
  show = 0;
//////////////////////////////////////////////////////////////////////////////////////// 
 
/////MOTOR PIN SETUPS///////////////////////////////////////////////////////////////////
  digitalWrite(speed_up,HIGH);
  digitalWrite(speed_down,HIGH);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

//MOTOR INITIAL STATE = OFF
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  

}

void loop() 
{
  getTemp();
  speedControl();
  Display();

  
  delay(1000);
}

//////////////////////////////////////////////////////////////////////////////////////// 
//SPEED CONTROL FUNCTION////////////////////////////////////////////////////////////////
void speedControl() 
// THE PURPOSE OF THIS FUNCTIONS IS TO CONTROL THE SPEED OF THE MOTOR
{
    //WE READ ANALOG VALUE FROM POTENTIOMETER TWICE WITH A SMALL DELAY FOR MORE ACCURARY
    val = analogRead(potPin);    // read the value from the sensor
    delay(10);
    val = analogRead(potPin);    // read the value from the sensor
    
// THE POTENTIOMETER CONTROLS THE SPEED, WHEN A VALUE FROM THE ANALOG PIN IS READ, THIS IS TRANSLATED
// BY THE IF STATEMENTS INTO A VALUE FOR THE SPEED OF THE MOTOR. THE MAX VALUE FOR MOTOR IS 255
  if (val == 0)
  {
    motor_speed = 0;
    
    }
    else if ((val  > 0)&&(val <= 102))
    {
      motor_speed = 0;
      }
      else if ((val  > 102)&&(val <= 204))
    {
      motor_speed = 120;
      }
       else if ((val  > 204)&&(val <= 307))
    {
      motor_speed = 130;
      }
      else if ((val  > 307)&&(val <= 409))
    {
      motor_speed = 140;
      }
      else if ((val  > 409)&&(val <= 511))
    {
      motor_speed = 150;
      }
      else if ((val  > 511)&&(val <= 613))
    {
      motor_speed = 160;
      }
      else if ((val  > 613)&&(val <= 715))
    {
      motor_speed = 170;
      }
      else if ((val  > 715)&&(val <= 817))
    {
      motor_speed = 180;
      }
      else if ((val  > 817)&&(val <= 919))
    {
      motor_speed = 200;
      }
      else if ((val  > 919)&&(val <= 950))
    {
      motor_speed = 220;
      }
      else if ((val  > 950)&&(val <= 1000))
    {
      motor_speed = 240;
      }
      else if ((val  > 1000)&&(val <= 1023))
    {
      motor_speed = 255; //MAX SPEED
      }// END IF'S

  // For PWM maximum possible values are 0 to 255
  analogWrite(enA, motor_speed);
  
  // Turn on motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  //MOTOR SPEED DISPLAY DOR DEBUGGING IN ARDUINO IDE
  Serial.println(motor_speed);
}
////////////////////////////////////////////////////////////////////////////////////////
////TEMPERATURE FUNCTION//////////////////////////////////////////////////////////////// 

void getTemp() 
{
// THE PURPOSE OF THIS FUNCTION IS TO CALCULATE THE TEMPERATURE FROM THERMISTOR  

//WE READ ANALOG VALUE FROM THERMISTOR TWICE WITH A SMALL DELAY FOR MORE ACCURACY
  Vo = analogRead(ThermistorPin);
  delay(50);
  Vo = analogRead(ThermistorPin);

  //CALCULATE TEMPERATURES
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 

  //TEMPERATURE DISPLAY FOR DEBUGGING IN Arduino IDE
  Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.println(" F"); 


}
////////////////////////////////////////////////////////////////////////////////////////
////LCD DISPLAY FUNCTION//////////////////////////////////////////////////////////////// 

void Display()
{
// THE PURPOSE OF THIS FUNCTION IS TO DISPLAY DATA IN THE LCD

    lcd.setBacklight(255); //LCD BACKLIGHT
    lcd.home();
    lcd.clear();
    
    // DISPLAY STRING Temp IN THE FIRST LINE AT POSITION 0
    lcd.setCursor(0,0);
    lcd.print("Temp  F:");
    // DISPLAY degree symbol IN THE FIRST LINE AT POSITION 5
    lcd.setCursor(5,0);
    lcd.print((char)223);
    // DISPLAY VALUE TEMPERATURE IN THE FIRST LINE AT POSITION 12
    lcd.setCursor(12,0);
    lcd.print(T);
    
    // DISPLAY STRING SPEED IN THE SECOND LINE AT POSITION 0
    lcd.setCursor(0,2);
    lcd.print("Speed: ");

    // DISPLAY VALUE SPEED IN THE SECOND LINE AT POSITION 12
    lcd.setCursor(12,2);
    lcd.print(motor_speed);
} 
