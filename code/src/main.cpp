#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Adafruit_BMP280.h>

#define WIFI_SSID "red"
#define WIFI_PASSWORD "contraseña red"

#define FIREBASE_HOST "weatherapp...com"
#define FIREBASE_AUTH "..."

FirebaseData fbdo;
FirebaseData firebaseData;
FirebaseJson updateSensorValue;
FirebaseJson json;

#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)
#define LED_PIN 33

Adafruit_BMP280 bmp; // I2C
// Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
// Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

float Temperatura = 0;
float Presion = 0;
float Altitude = 0;
unsigned long lastFirebaseUpdate = 0;
const unsigned long FIREBASE_INTERVAL = 3000;

void update_firebase();

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  // SENSOR BMP280
  Serial.println(F("BMP280 Forced Mode Test."));

  // if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
  if (!bmp.begin())
  {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or ""try a different address!"));
    while (1)
      delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // Wifi y base de datos
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  WiFi.setSleep(false);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void update_firebase()
{
  digitalWrite(LED_PIN, HIGH);
  updateSensorValue.add("Temperatura", Temperatura);
  updateSensorValue.add("Presion", Presion);
  updateSensorValue.add("Altitude", Altitude);

  if (Firebase.RTDB.updateNode(&fbdo, "/Monitor", &updateSensorValue))
  {
    Serial.println("Datos enviados a Firebase correctamente");
  }
  else
  {
    Serial.print("Error al enviar a Firebase: ");
    Serial.println(fbdo.errorReason());
  }
  digitalWrite(LED_PIN, LOW);
}

void loop()
{
  if (millis() - lastFirebaseUpdate >= FIREBASE_INTERVAL)
  {
    lastFirebaseUpdate = millis();
    update_firebase();
    // must call this to wake sensor up and get new measurement data
    // it blocks until measurement is complete
    if (bmp.takeForcedMeasurement())
    {
      // can now print out the new measurements
      Serial.print(F("Temperature = "));
      Temperatura = bmp.readTemperature();
      Serial.print(bmp.readTemperature());
      Serial.print(Temperatura);
      Serial.println(" *C");

      Serial.print(F("Pressure = "));
      Presion = bmp.readPressure() * 0.00750062f;
      Serial.print(Presion);
      Serial.println(" Pa");

      Serial.print(F("Approx altitude = "));
      Altitude = bmp.readAltitude(1013.25);
      Serial.print(Altitude);
      Serial.println(" m");

      Serial.println();
    }
    else
    {
      Serial.println("Forced measurement failed!");
    }
  }
}