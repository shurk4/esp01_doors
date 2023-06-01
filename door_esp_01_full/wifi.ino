// Алгоритм работы:
// Если это первое включение (configured = false), включается точка доступа без пароля и ожидает настройки в веб интерфейсе
// После применения настроек в EEPROM сохраняются логин, пароль и параметр сообщающий о том, что устройство настроено(configured = true)(перечисление кодов настроек указано в основном файле).
// Затем устройство перезапускается, подключается к указаной сети и ждёт запросов в веб интерфейсе

// Если при включении нет связи с установленой сетью, включается режим точки доступа и производится сканирование доступных сетей.
// Если указанная в настройках сеть стала доступна, подключается к ней.

// Если сеть пропала во время работы устройства, начинается попытка подключиться

//Проверка соединения
bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  return false;
}

void setAvailibleNetworks()
{
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);
 
    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
}

void scanWiFi()
{  
  // Сканируем доступные сети
  n = WiFi.scanNetworks();
  Serial.println("scan done");
}
 
// Создание точки доступа
void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  scanWiFi();

  // если сетей нет, сообщаем об этом. Если сети нашлись, выводим в терминал список
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }

  Serial.println("");
  setAvailibleNetworks();

  delay(100);
  WiFi.softAP("doors_host", "");
  Serial.println("softap MODE");
  Serial.println(WiFi.softAPIP());
  Serial.println("Waiting for configuration");
}

void wifiConnect()
{  
  WiFi.disconnect();
  WiFi.begin(esid.c_str(), epass.c_str());

  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    Serial.println(esid);
    Serial.println(epass);
    connected = true;
    return;
  }
  
  if(!connected)
  {
    Serial.println("Turning the HotSpot On");
    setupAP();// Setup HotSpot

    while ((WiFi.status() != WL_CONNECTED))
    {
      Serial.print(".");
      delay(1000);
      server.handleClient();

      if(configured)
      {
        scanWiFi();

        for(int i = 0; i < n; i++)
        {
          if(WiFi.SSID(i) == esid.c_str())
          {
            Serial.println("WiFi founded, restarting!");
            ESP.reset();
          }
        }
      }
    }
  }
 
  Serial.println();
  Serial.println("Waiting.");
}