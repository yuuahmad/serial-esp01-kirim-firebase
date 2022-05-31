#include <Arduino.h>
#define BLYNK_TEMPLATE_ID "TMPLyN53ar8H"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "NU0sKMDkAA1tNbem3OgpKagj4Ga4dd4-"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "KONTRAKAN UYE";
char pass[] = "KUSANG123";
BlynkTimer timer;

// buat variabel bool untuk parsing data.
bool parsing = false;
String sData, data[10];
int nilaiPertama, nilaiKedua, nilaiKetiga, nilaiKeempat;
BLYNK_WRITE(V0)
{
  int value = param.asInt();
  Blynk.virtualWrite(V1, value);
}

void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V3, nilaiPertama);
  Blynk.virtualWrite(V4, nilaiKedua);
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
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
      nilaiPertama = data[1].toInt();
      nilaiKedua = data[2].toInt();
      parsing = false;
      sData = "";
    }
  }
}
