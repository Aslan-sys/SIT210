#include <BH1750.h>
#include <Wire.h>

// pir motion pin 
#define PIR_PIN 2

// switch slider
#define SWITCH_PIN 3

// proch n hallway lights
#define PORCH_LED 4
#define HALLWAY_LED 5

// bh sensor 
BH1750 lightMeter;

// room with lights on procudes lux of 125 roughly when turned off goes down to 5, so putting our const of 60 makes sure that tells sys that its dark
const float DARK = 60.0;

// these both turn to true if our interupts happen
volatile bool pirTriggered = false;
volatile bool switchTriggered = false;

// time for led 
const unsigned long porchOnDuration = 3000;    
const unsigned long hallwayOnDuration = 6000;  
unsigned long eventStartTime = 0;

// bool states checkis if lights are on or not and if they should be on
bool lightsOn = false;
bool porchOn = false;
bool hallwayOn = false;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
  }

  Wire.begin();
  lightMeter.begin();

  pinMode(PIR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT);
  pinMode(PORCH_LED, OUTPUT);
  pinMode(HALLWAY_LED, OUTPUT);
  // insures led start off
  digitalWrite(PORCH_LED, LOW); 
  digitalWrite(HALLWAY_LED, LOW);

  // our intterupts where first runs if the pir sensor goes from low to high and swtich runs if it gets fliped
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, RISING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchISR, CHANGE);

  Serial.println("Lina's motion and light sensors are ready");
}

void loop()
{
  // pir interupt at work
  if (pirTriggered)
  {
    pirTriggered = false;

    bool night = isNight();

    if (!lightsOn && night)
    {
      lightsEvent();
      Serial.println(" acitivrty is detected at night. porch and hallway lights are now ON");
    }
    else if (!night)
    {
      Serial.println(" activity was detected but its not dark. lights stay OFF");
    }
  }

  //switch interrupt happened
  if (switchTriggered)
  {
    switchTriggered = false;

    if (!lightsOn && switchStatus())
    {
      lightsEvent();
      Serial.println(" switch turned ON, porch and hallway lights ON");
    }
  }

  //timing for turning lights off
  if (lightsOn)
  {
    unsigned long currentTime = millis() - eventStartTime;

    if (currentTime >= porchOnDuration && porchOn)
    {
      porchLightOff();
      porchOn = false;
      Serial.println("porch light OFF");
    }

    if (currentTime >= hallwayOnDuration && hallwayOn)
    {
      hallwayLightOff();
      hallwayOn = false;
      lightsOn = false;
      Serial.println("hallway light OFF");
    }
  }
}

// isr of pir sets flag that tells interupt has hapend
void pirISR()
{
  pirTriggered = true;
}

// isr of swith
void switchISR()
{
  switchTriggered = true;
}

// ight event
void lightsEvent()
{
  eventStartTime = millis();
  lightsOn = true;

  porchLightOn();
  porchOn = true;

  hallwayLightOn();
  hallwayOn = true;
}

// reads sensor light
float lightSensor()
{
  float lux = lightMeter.readLightLevel();
  Serial.print("Light level: ");
  Serial.print(lux);
  Serial.println(" lx");
  return lux;
}

// is it dark or not bool
bool isNight()
{
  float lux = lightSensor();

  if (lux < DARK)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// switch state check if true returns on 
bool switchStatus()
{
  return digitalRead(SWITCH_PIN) == HIGH;
}

// proch light on
void porchLightOn()
{
  digitalWrite(PORCH_LED, HIGH);
}

// porch light off
void porchLightOff()
{
  digitalWrite(PORCH_LED, LOW);
}

// hallway lights on
void hallwayLightOn()
{
  digitalWrite(HALLWAY_LED, HIGH);
}

// hallway lights off
void hallwayLightOff()
{
  digitalWrite(HALLWAY_LED, LOW);
}