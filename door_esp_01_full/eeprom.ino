// Чтение EEPROM
void readEEPROM()
{
  Serial.println("Reading EEPROM ssid");
  int currentParam = paramSize;

  // Чтение SSID
  for (int i = 0; i < currentParam; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println();

  // Чтение пароля WiFi
  Serial.println("Reading EEPROM pass");
  currentParam += paramSize;
  for (int i = currentParam - paramSize; i < currentParam; ++i) // Было: i < 96
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
  Serial.println();

  // Чтение параметров
  Serial.println("Reading EEPROM param");
  currentParam += paramSize;
  // Serial.println();
  // Serial.println("Read EEPROM blocks from ");
  // Serial.println(currentParam - paramSize);
  // Serial.println(" to ");
  // Serial.println(currentParam);
  // Serial.println();
  for (int i = currentParam - paramSize; i < currentParam; ++i) // Было: i < 96
  {
    param += char(EEPROM.read(i));
  }

  for(int i = 0; i < param.length(); i++)
  {
    if(param[i] == 'c')
    {
      configured = true;
    }
  }
  Serial.print("PARAM: ");
  Serial.println(param);
  Serial.println();
}

// очистка EEPROM
void clearEEPROM()
{
  Serial.println("clearing eeprom");
  for (int i = 0; i < 96; ++i) 
  {
  EEPROM.write(i, 0);
  }
}

// запись EEPROM
void writeEEPROM()
{
  clearEEPROM();

  //Запись SSID
  Serial.println("writing eeprom ssid:");
  int currentParam = 0;
  for (int i = 0; i < esid.length(); ++i)
  {
    EEPROM.write(i, esid[i]);
    Serial.print("Wrote: ");
    Serial.println(esid[i]);
  }
  
  //Запись пароля SSID
  Serial.println("writing eeprom pass:");
  currentParam += paramSize;
  for (int i = 0; i < epass.length(); ++i)
  {
    EEPROM.write(currentParam + i, epass[i]);
    Serial.print("Wrote: ");
    Serial.println(epass[i]);
  }

  // Запись параметров
    Serial.println("writing eeprom param:");
  currentParam += paramSize;

  if(configured)
  {
    param = "c";
  }

  param += "t";

  for (int i = 0; i < param.length(); ++i)
  {
    EEPROM.write(currentParam + i, param[i]);
    Serial.print("Wrote: ");
    Serial.println(param[i]);
  }

  EEPROM.commit();

  content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
  statusCode = 200;
}