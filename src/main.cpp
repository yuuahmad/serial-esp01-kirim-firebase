#include <Arduino.h>
// include board
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

// include firebase dan wifi
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#define WIFI_SSID "YUUAHMAD laptop" // ini adalah nama wifi
#define WIFI_PASSWORD "yusuf1112"   // dan ini adalah passwordnya. kosongkan bagian ini kalau tidak pakai password
#define API_KEY "AIzaSyDxdifqzfU7VL7HvOqn1bz8GsesRy6xvhc"
#define DATABASE_URL "solar-tracker-database-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define USER_EMAIL "ahmad.yusuf11@gmail.com"
#define USER_PASSWORD "yusuf1112"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
int nilaitambah = 0;

// buat variabel bool untuk parsing data.
bool parsing = false;
String sData, data[10];
int temperatur, bat_volt, solar_volt, arus;

void setup()
{
  // mulai komunikasi serial
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(3000);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  delay(1000);

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
}

void loop()
{
  // kode untuk mendapatkan nilai dari arduino mega
  while (Serial.available())
  {
    // buat variabel nilaiinput, dan masukkan nilai serial.readString kesana
    // String nilaiInput = Serial.readString();
    // print/tampilkan nilai input tadi di serial monitor
    char inChar = Serial.read();
    sData += inChar;
    if (inChar == '$')
      parsing = true;
    if (parsing)
    {
      int q = 0;
      for (int i = 0; i < sData.length(); i++)
      {
        if (sData[i] == '#')
        {
          q++;
          data[q] = "";
        }
        else
          data[q] += sData[i];
      }
      // setelah semua data didapatkan, kita akan print datanya ke serial satu persatu
      temperatur = data[1].toInt();
      bat_volt = data[2].toInt();
      solar_volt = data[3].toInt();
      arus = data[4].toInt();
      parsing = false;
      sData = "";
    }
  }
  // ini kode untuk upload kode ke firebase
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 4000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    Firebase.RTDB.setFloat(&fbdo, "/temperatur", temperatur);
    Firebase.RTDB.setFloat(&fbdo, "/soltra1/teganganbaterai", bat_volt);
    Firebase.RTDB.setFloat(&fbdo, "/solar_volt", solar_volt);
    Firebase.RTDB.setFloat(&fbdo, "/arus", arus);
  }
}