#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// // Вэб страницы
// void homePage();
// void returnHome();
// void settingsPage();
// void returnSettings();
// void scanWiFiPage();
// void restartPage();
// void applySettingsPage();
// void onPage();
// void offPage();

//Variables
int statusCode;
int n;

//EEPROM config
int paramSize = 16;
//Параметры
String param = "";
// c - configured

// a - режим AP / b - режим WiFi клиент
// 

// WiFi config
String esid = "";
String epass = "";
String st; // Строка для списка WiFi сетей
String content; // Строка HTML для отправки клиенту

bool configured = false;
bool AP = false;
bool connected = false;
bool ledOn = false;
 
//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
 
//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);

void setup()
{ 
  //Подключение debug if(DEBUG)Serial Monitor
  Serial.begin(115200);

  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");

  //Подключение EEPROM
  EEPROM.begin(512);
  delay(10);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.println("Startup");
 
  readEEPROM();
  
  launchWeb();
}

void loop() {
  if ((WiFi.status() != WL_CONNECTED))
  {
    Serial.println("Disconnected! Trying connect to SSID: " + esid);
    wifiConnect();
  }  
  server.handleClient();
}