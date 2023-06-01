// Запуск веб сервера
void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());

  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}
 
// Заполнение  веб сервера
void createWebServer()
{
  server.on("/", homePage);
  server.on("/on", onPage);
  server.on("/off", offPage);
  server.on("/settings", settingsPage);
  server.on("/scan", scanWiFiPage);
  server.on("/restart", restartPage);
  server.on("/setting", applySettingsPage);
}

//--------------- Страницы сервера -----------------------

void homePage()
{
  Serial.println("start page is opened");

  controlPage();
}

void returnHome() // Возвращает на домашнюю страницу
{
  content = "<meta http-equiv=\"Refresh\" content=\"1; URL=/\">";
  server.send(200, "text/html", content);
}

void controlPage()
{
  Serial.println("start page is opened");

  content = "<!DOCTYPE HTML>\r\n<html>Door host - settings";
  String tempString = "Led is: ";
  if(ledOn)
  {
    tempString += "ON";
  }
  else
  {
    tempString += "OFF";
  }
  content += tempString;
  content += "<form action=\"/on\" method=\"POST\"><input type=\"submit\" value=\"on\"></form>";
  content += "<form action=\"/off\" method=\"POST\"><input type=\"submit\" value=\"off\"></form>";  
  content += "<form action=\"/settings\" method=\"POST\"><input type=\"submit\" value=\"settings\"></form>";
  content += "</html>";
  
  server.send(200, "text/html", content);
}

void settingsPage()
{
  IPAddress ip = WiFi.softAPIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

  content = "<!DOCTYPE HTML>\r\n<html>Door host - settings";
  content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
  content += "<form action=\"/restart\" method=\"POST\"><input type=\"submit\" value=\"restart\"></form>";
  content += ipStr;
  content += "<p>";
  content += st;
  content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
  content += "</html>";

  server.send(200, "text/html", content);
}

void returnSettings() // Возвращает на страницу settings
{
  content = "<meta http-equiv=\"Refresh\" content=\"1; URL=/settings\">";
  server.send(200, "text/html", content);
}

void applySettingsPage()
{
  esid = server.arg("ssid");
  epass = server.arg("pass");
  configured = true;

  if (esid.length() > 0 && epass.length() > 0)
  {
    Serial.println(esid);
    Serial.println("");
    Serial.println(epass);
    Serial.println("");
    writeEEPROM();
    ESP.reset();
  } 
  else 
  {
    content = "{\"Error\":\"404 not found\"}";
    statusCode = 404;
    Serial.println("Sending 404");
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(statusCode, "application/json", content);

  returnHome();
}

void scanWiFiPage()
{
  IPAddress ip = WiFi.softAPIP();

  scanWiFi();
  setAvailibleNetworks();

  returnSettings();
  // homePage();
}

void restartPage()
{
  returnHome();
  delay(100);
  Serial.println("Device rebooting...");
  ESP.reset();
}

void onPage()
{
  digitalWrite(LED_BUILTIN, HIGH);
  ledOn = true;
  returnHome();
}
void offPage()
{
  digitalWrite(LED_BUILTIN, LOW);
  ledOn = false;
  returnHome();
}