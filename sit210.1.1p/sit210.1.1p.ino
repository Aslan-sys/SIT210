// here we define our pins and their locations

const int buttonPin = 2;
const int porchLightPin = 4;
const int hallwayLightPin = 3;


// our timers for our lights

const unsigned long porchTimer = 30000;
const unsigned long hallwayTimer = 60000;

// our Vars

bool systemActive = false;
unsigned long startTime = 0;
int lastButtonPressed = HIGH;

// functions declares

void pinSetup();
void checkButtonPressed();
void activeLights();
void controlLights();
void turnOffLights();


// program starts
void setup()
{
  pinSetup();
  Serial.begin(9600);
}

// look checking if system is active to control the lights
void loop()
{
  checkButtonPressed();
  if (systemActive)
    {
      controlLights();
    }
}

// gives our button an input that system can use
void pinSetup()
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(porchLightPin, OUTPUT);
  pinMode(hallwayLightPin, OUTPUT);
  digitalWrite(porchLightPin, LOW);
  digitalWrite(hallwayLightPin, LOW);
}

// checks the state of our button if pressed or not and if pressed actives the lights
void checkButtonPressed()
{
  int currentButtonState = digitalRead(buttonPin);

  if (lastButtonPressed == HIGH && currentButtonState == LOW)
  {
    activeLights();
  }

  lastButtonPressed = currentButtonState;
}

// responsilble for turning the lghts on
void activeLights()
{
  systemActive = true;
  startTime = millis();

  digitalWrite(porchLightPin, HIGH);
  digitalWrite(hallwayLightPin, HIGH);

  Serial.println("Light are now on");
}

// timing controll for porch and hallway
void controlLights()
{
  unsigned long elapsedTime = millis() - startTime;


  if (elapsedTime >= porchTimer)
  {
    digitalWrite(porchLightPin, LOW);
  }

  // Hallway OFF after 60s
  if (elapsedTime >= hallwayTimer)
  {
    digitalWrite(hallwayLightPin, LOW);
    turnOffLights();
  }
}


// at the end turns the lights off and turns back to default state
void turnOffLights()
{
  systemActive = false;
  Serial.println("Lights are now off.");
}









