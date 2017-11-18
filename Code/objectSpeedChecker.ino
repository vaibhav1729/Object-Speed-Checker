/*
  Object Speed Checker

  Calculates Speed of an object and displays it on LCD.

  modified 18 Nov 2017
  by Vaibhav Agrawal

*/

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Initialise the present val and prev val of the IR pair
int present_state=0;
int prev_state=1;

//Initialize the IR sensor pins
int sensor1 = A0;
int sensor2 = A1;

//Initialize the final speed
float sped = 0.0;

// Initialize the IR pair values
int ldr1_val = 850;
int ldr2_val = 850;

int val = 1;                   //used for controlling unnecessary running of loop.
int buzz = 13;                 //piezo buzzer to digital pin 13
//We used LED instead of buzzer
float sped_limit = 1.00;       //Threshold speed of an obstacle i.e. 1 kmph.
int limit = 800;               //Threshold range of IR Pair when obstacle crosses it

//Initialize variables for calculating time

long int time3 = 0;
long int time1, time2;
void setup()
{
  lcd.begin(16, 2);
  //Serial.begin(300);         //setting serial monitor at a baud rate of 300 (for slow communication).

  //define pin mode for input pins
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  pinMode(buzz, OUTPUT);       //define pin mode for buzzer
  digitalWrite(buzz, LOW);     //set the buzzer in off mode (initial condition)

  lcd.print("Speed: ");
}

void loop()
{
  ldr1_val = analogRead(sensor1);
  ldr2_val = analogRead(sensor2);
  if (ldr1_val <= limit && val == 1)
  {
    lcd.clear();
    lcd.print("Speed: ");
    present_state=ldr1_val;
    val = 0;
    time1 = millis();               //Stores time at the moment when object crosses IR Pair 1.
    delay(200);
    
  }

  if (ldr2_val <= limit && prev_state!=present_state)
  {
    delay(200);
    time2 = millis();               //Stores time at the moment when object crosses IR Pair 2.
    time3 = (time2 - time1);
    sped = 954.0 / time3;           //As speed = distance/time. Note: Distance b/w IR Pairs is taken 26 cm.
    lcd.clear();                    //Clears the LCD screen.
    lcd.print("Speed: ");
    lcd.print(sped);
    lcd.println(" kmph");           //Displays "Speed: sped kmph"

    if (sped <= sped_limit)         //If obstacle is nearer than the Threshold range
    {
      digitalWrite(buzz, LOW);      //LED will be in OFF state
      delay(20);
    }
    else if (sped > sped_limit)     //If obstacle is not in Threshold range
    {
      lcd.setCursor(0,1);
      lcd.print("ALERT!!!");        //Displays alert!!! when object is overspeed.
      digitalWrite(buzz, HIGH);     //LED will be in ON state
      delay(1000);
      digitalWrite(buzz, LOW);      //LED will be in OFF state after a delay of 1 second
    }
    prev_state=present_state;
  }
  val = 1;
}

