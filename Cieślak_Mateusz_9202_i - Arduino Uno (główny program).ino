// Kod główny Arduino UNO
//Mateusz Cieślak nr indeksu 9202
// biblioteki wykorzystane i definicje
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"
#include <RTClib.h>
#include <MQ7.h>
#define DHTPIN 7
#define DHTTYPE DHT22
#define TFT_RST 4
#define TFT_CS 2
#define TFT_DC 9
#define CO_Czujnik A0
#define LED_PINB 5
#define LED_PINR 3
#define RXD2 17
#define TXD2 16


Adafruit_ST7735 tft = Adafruit_ST7735(2, 9, 4); //definicja połączeń wyświetlacza
#define BMP280_I2C_ADDRESS 0x76
Adafruit_BMP280 bmp280;
DHT dht(7, 22); //definicja połączeń czujnika DHT
RTC_DS1307 rtc;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        

boolean noweDane = false;

float float1;
float float2;
float float3;
float float4;

void setup(void) 
{
  Serial.begin(9600);

  dht.begin();
  rtc.begin();
  tft.initR(INITR_GREENTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.drawFastHLine(0, 15, tft.width(), ST77XX_CYAN);  //   Rysowanie linii na wyświetlaczu (definicje koloru)
  tft.drawFastHLine(0, 35, tft.width(), ST77XX_CYAN);
  tft.drawFastHLine(0, 55, tft.width(), ST77XX_CYAN);
  tft.drawFastHLine(0, 75, tft.width(), ST77XX_CYAN);
  tft.drawFastHLine(0, 95, tft.width(), ST77XX_CYAN);
  tft.drawFastHLine(0, 115, tft.width(), ST77XX_CYAN);
  tft.drawFastHLine(0, 135, tft.width(), ST77XX_CYAN);
  tft.drawFastHLine(0, 155, tft.width(), ST77XX_CYAN);
  tft.drawFastVLine(64, 15, 140, ST77XX_CYAN);
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);  //ustawienia koloru czcionki i tła
  tft.setTextSize(1);                           // ustawienie wielkości czcionki

  tft.setCursor(20, 4);  //pozycja kursora ustawiona na (20, 4)
  tft.print("Pogoda Arduino");

  if (!rtc.begin()) {
    tft.println("Nie znaleziono RTC");
    tft.flush();
    while (1)
      ;
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}


// main loop
void loop() {

  int czujnik_tlenku_wegla = analogRead(CO_Czujnik);
  float napiecie = czujnik_tlenku_wegla * (5.0 / 1023.0);
  float ppm = napiecie * 100;


  if (ppm > 35) {
    digitalWrite(LED_PINR, HIGH);
    digitalWrite(LED_PINB, LOW);
  } else {
    digitalWrite(LED_PINR, LOW);
    digitalWrite(LED_PINB, HIGH);
  }
  if (bmp280.begin(BMP280_I2C_ADDRESS) == 0) {  
    tft.setTextColor(ST77XX_YELLOW, ST77XX_CYAN);  
    tft.setTextSize(1);                        
    tft.setCursor(5, 76);                       
    tft.print("Połączenie");
    tft.setCursor(35, 100);  
    tft.print("Błąd Połączenia");
    while (1);  

  
  }
  char _buffer[8];

  float temp_pow = dht.readTemperature(); 
  float wil_pow = dht.readHumidity();
  float cis = bmp280.readPressure();
  float temp_z = float1;
  float wil_z = float2;
  
  if (temp_pow < 0)  
    sprintf(_buffer, "-%02u.%02u", (int)abs(temp_pow), (int)(abs(temp_pow) * 100) % 100);
  //else                                                                           
    sprintf(_buffer, " %02u.%02u", (int)temp_pow, (int)(temp_pow * 100) % 100);  
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);                                 
  tft.setCursor(85, 44);                                                         
  tft.print(_buffer);                                                            
  tft.drawCircle(55, 44, 1, ST77XX_YELLOW);                                      
  tft.setCursor(57, 44);                                                        
  tft.print("C");

  if (temp_z < 0)  
    sprintf(_buffer, "-%02u.%02u", (int)abs(temp_z), (int)(abs(temp_z) * 100) % 100);
  else                                                                       
    sprintf(_buffer, " %02u.%02u", (int)temp_z, (int)(temp_z * 100) % 100);  
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);                             
  tft.setCursor(16, 44);                                                     
  tft.print(_buffer);                                                      
  tft.drawCircle(55, 44, 1, ST77XX_YELLOW);                                 
  tft.setCursor(57, 44);                                                     


  sprintf(_buffer, "%04u.%02u", (int)(cis / 100), (int)((uint32_t)cis % 100));  
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);                                
  tft.setCursor(65, 64);                                                        
  tft.print(_buffer);                                                           
  tft.setCursor(108, 64);                                                       
  tft.print("hPa");
  sprintf(_buffer, "%02u   ", (int)(wil_z));      
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);  
  tft.setCursor(30, 144);                         
  tft.print(_buffer);                            
  tft.setCursor(43, 144);                         
  tft.print("%");
  sprintf(_buffer, "%02u   ", (int)(wil_pow));  
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setCursor(30, 144);
  tft.print(_buffer);
  tft.setCursor(43, 144);
  tft.print("%");
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.drawFastHLine(0, 15, tft.width(), ST77XX_CYAN);
  tft.setCursor(20, 15);
  tft.setTextSize(1);
  DateTime now = rtc.now();
  tft.setCursor(3, 25);
  tft.print(now.year());
  tft.print('/');
  tft.print(now.month());
  tft.print('/');
  tft.print(now.day());
  tft.setCursor(70, 25);
  tft.print(now.hour());
  tft.print("");
  tft.print(':');
  if (now.minute() < 10) tft.print('0');
  tft.print(now.minute());
  tft.print("");
  tft.print(':');
  if (now.second() < 10) tft.print('0');
  tft.print(now.second());
  tft.println("");

  tft.setCursor(3, 44);
  tft.print("TZ");

  tft.setCursor(70, 44); 
  tft.print("TW:");
  
  tft.setCursor(3, 64);
  tft.print("Cisnienie:");

  tft.setCursor(3, 84);
  tft.print("CO:");
  tft.setCursor(70, 84);
  tft.print(ppm);
  tft.print(" ppm");
  tft.setCursor(3, 104);
  tft.print("PM 2.5");
  tft.setCursor(70, 104);
  tft.setCursor(3, 124);
  tft.print("PM 10");
  tft.setCursor(70, 124);

  tft.setCursor(3, 144);
  tft.print("HW:");

  tft.setCursor(70, 144);
  tft.print("HZ:");
  tft.print(wil_z);
     recvWithStartEndMarkers();
    if (noweDane == true) {
        strcpy(tempChars, receivedChars);
            
        parsujDane();
        pokazParsowaneDane();
        noweDane = false;
    }
  

}


void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && noweDane == false) {
        rc = Serial.read();
 
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; 
                recvInProgress = false;
                ndx = 0;
                noweDane = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parsujDane() {      

    char * strtokIndx; 

    strtokIndx = strtok(tempChars,",");      
    float1 = atof(strtokIndx);
 
     strtokIndx = strtok(NULL, ",");
    float2 = atof(strtokIndx);;     

    strtokIndx = strtok(NULL, ",");
    float3 = atof(strtokIndx);     
    
    strtokIndx = strtok(NULL, ",");
    float4 = atof(strtokIndx);

    
}

//============

void pokazParsowaneDane() {

    
   tft.setCursor(90, 104);
    
   tft.print(float3);

   tft.setCursor(90, 124);
  
   tft.print(float4);
}

