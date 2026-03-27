// libraries
#include <DHT.h>
#include <SPI.h>
#include <ThingSpeak.h>
#include <WiFiNINA.h>

// pin assignments
#define DHTPIN 2
#define DHTTYPE DHT22

const int lightPin = A0;

// WiFi details
char ssid[] = "asus";
char pass[] = "mis76Mis76";
WiFiClient client;

// ThingSpeak details
unsigned long channelID = 3316139;
const char *writeAPIKey = "G3WWOWFK384PW1FO";

// sensor object
DHT dht(DHTPIN, DHTTYPE);

// timing
const unsigned long updateInterval = 30000;
unsigned long lastUpdateTime = 0;

// function declarations
void connectToWiFi();
float readTemperature();
int readLightLevel();
void sendDataToThingSpeak(float temperature, int lightLevel);

// program starts
void setup()
{
  Serial.begin(9600);
  while (!Serial);

  dht.begin();
  connectToWiFi();
  ThingSpeak.begin(client);
}

void loop()
{
  if (millis() - lastUpdateTime >= updateInterval)
  {
    float temperature = readTemperature();
    int lightLevel = readLightLevel();

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" Degrees, Light: ");
    Serial.println(lightLevel);

    sendDataToThingSpeak(temperature, lightLevel);
    lastUpdateTime = millis();
  }
}

void connectToWiFi()
{
  Serial.print("Connecting");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    Serial.print(".");
    delay(5000);
  }

  Serial.println();
  Serial.println("Connected to the WiFi");
}

float readTemperature()
{
  float temp = dht.readTemperature();

  if (isnan(temp))
  {
    Serial.println("Can't detect the DHT readings");
    return -1;
  }

  return temp;
}

int readLightLevel()
{
  int lightValue = analogRead(lightPin);
  return lightValue;
}

void sendDataToThingSpeak(float temperature, int lightLevel)
{
  if (temperature == -1)
  {
    Serial.println("Invalid input, data not being received");
    return;
  }

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, lightLevel);

  int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (responseCode == 200)
  {
    Serial.println("Data sent");
  }
  else
  {
    Serial.print("ThingSpeak error: ");
    Serial.println(responseCode);
  }
}