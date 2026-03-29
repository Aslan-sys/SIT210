#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>

// Wi-Fi name and password
char ssid[] = "asus";
char pass[] = "mis76Mis76";

// IFTTT details
char host[] = "maker.ifttt.com";
String key = "fB2r2Quk4JvMvpbJoe1izAr5gVEumhm5dz1ZURRLCi5";

// event names
String sunlightActiveEvent = "sunlight_active";
String sunlightOffEvent = "sunlight_off";

// BH1750 sensor object
BH1750 lightMeter;

// Wi-Fi client
WiFiClient client;

// light threshold
float lightThreshold = 500.0;

// keeps track of whether sunlight is detected
bool sunlightDetected = false;

// function declarations
void connectToWiFi();
void sendIFTTTEvent(String eventName);

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Wire.begin();

  // check sensor starts properly incase of error, this helped me debug as sensor was working for the example test but not with this due to the VCC not being plugged in as i mis plugged it.
  if (lightMeter.begin())
  {
    Serial.println("BH1750 started successfully");
  }
  else
  {
    Serial.println("Error: BH1750 did not start");
    while (1);
  }

  delay(200);

  connectToWiFi();
}

void loop()
{
  float lux = lightMeter.readLightLevel();

  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");

  // if the sensor read failed, skip this loop
  if (lux < 0)
  {
    Serial.println("sensor read failed");
    delay(2000);
    return;
  }

  // when light goes above threshold
  if (lux > lightThreshold && sunlightDetected == false)
  {
    sunlightDetected = true;
    Serial.println("sunlight is detected, notification is sending");
    sendIFTTTEvent(sunlightActiveEvent);
  }
  // when light goes below threshold
  else if (lux <= lightThreshold && sunlightDetected == true)
  {
    sunlightDetected = false;
    Serial.println("sunlight has stopped, notification is sending");
    sendIFTTTEvent(sunlightOffEvent);
  }

  delay(5000);
}

void connectToWiFi()
{
  Serial.print("network connecting");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    Serial.print(".");
    delay(5000);
  }

  Serial.println();
  Serial.println("wifi is on");
}

void sendIFTTTEvent(String eventName)
{
  if (client.connect(host, 80))
  {
    String url = "/trigger/" + eventName + "/with/key/" + key;

    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println();

    Serial.print("event is sent: ");
    Serial.println(eventName);
  }
  else
  {
    Serial.println("failed connection to ifttt.");
  }
}