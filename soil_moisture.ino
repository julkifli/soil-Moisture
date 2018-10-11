/***************************************************************************************************************
 * IoT DHT Temp/Hum and Soil Moister Station using NodeMCU ESP-12 Develop Kit V1.0
 *  DHT connected to NodeMCU pin D3
 *  Soil Moister Sensor connected to A0
 *  Sensor Data on local OLED Display
 *
 *  Developed by MJRovai 12 October 2017
 ********************************************************************************************************************************/

/* OLED */
#include <ACROBOTIC_SSD1306.h> // library for OLED: SCL ==> D1; SDA ==> D2
#include <SPI.h>
#include <Wire.h>

/* DHT22*/
#include "DHT.h"
#define DHTPIN D3  
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
float hum = 0;
float temp = 0;

/* Soil Moister */
#define soilMoisterPin A0
#define soilMoisterVcc D4
int soilMoister = 0;

void setup() 
{
  pinMode(soilMoisterVcc, OUTPUT);
  Serial.begin(115200);
  delay(10);
  dht.begin();
  oledStart();
  digitalWrite (soilMoisterVcc, LOW);
}

void loop() 
{
  getDhtData();
  getSoilMoisterData();
  displayData();
  delay(2000); // delay for getting DHT22 data
}

/***************************************************
 * Start OLED
 **************************************************/
void oledStart(void)
{
  Wire.begin();  
  oled.init();                      // Initialze SSD1306 OLED display
  clearOledDisplay();
  oled.clearDisplay();              // Clear screen
  oled.setTextXY(0,0);              
  oled.putString("  MJRoBot.org");
}

/***************************************************
 * Get DHT data
 **************************************************/
void getDhtData(void)
{
  float tempIni = temp;
  float humIni = hum;
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  if (isnan(hum) || isnan(temp))   // Check if any reads failed and exit early (to try again).
  {
    Serial.println("Failed to read from DHT sensor!");
    temp = tempIni;
    hum = humIni;
    return;
  }
}

/***************************************************
 * Get Soil Moister Sensor data
 **************************************************/
void getSoilMoisterData(void)
{
  soilMoister = 0;
  digitalWrite (soilMoisterVcc, HIGH);
  delay (500);
  int N = 3;
  for(int i = 0; i < N; i++) // read sensor "N" times and get the average
  {
    soilMoister += analogRead(soilMoisterPin);   
    delay(150);
  }
  digitalWrite (soilMoisterVcc, LOW);
  soilMoister = soilMoister/N; 
  Serial.println(soilMoister);
  soilMoister = map(soilMoister, 380, 0, 0, 100); 
}

/***************************************************
 * Display data at Serial Monitora & OLED Display
 **************************************************/
void displayData(void)
{
  Serial.print(" Temperature: ");
  Serial.print(temp);
  Serial.print("oC   Humidity: ");
  Serial.print(hum);
  Serial.println("%");
  
  oled.setTextXY(3,0);              // Set cursor position, start of line 2
  oled.putString("TEMP: " + String(temp) + " oC");
  oled.setTextXY(5,0);              // Set cursor position, start of line 2
  oled.putString("HUM:  " + String(hum) + " %");
   oled.setTextXY(7,0);              // Set cursor position, start of line 2
  oled.putString("Soil: " + String(soilMoister) + " %");
}

/***************************************************
 * Clear OLED Display
 **************************************************/
void clearOledDisplay()
{
  oled.setFont(font8x8);
  oled.setTextXY(0,0); oled.putString("                ");
  oled.setTextXY(1,0); oled.putString("                ");
  oled.setTextXY(2,0); oled.putString("                ");
  oled.setTextXY(3,0); oled.putString("                ");
  oled.setTextXY(4,0); oled.putString("                ");
  oled.setTextXY(5,0); oled.putString("                ");
  oled.setTextXY(6,0); oled.putString("                ");
  oled.setTextXY(7,0); oled.putString("                ");
  oled.setTextXY(0,0); oled.putString("                ");              
}
