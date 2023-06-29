// kod  odbiornika 
// Mateusz Cieślak nr indeksu 9202
 
// Wykorzystane biblioteki i definicje
#include <esp_now.h>
#include <WiFi.h>
#define RX2 16
#define TX2 17

// Definicja struktury danych
typedef struct struct_message {
  char a[32];
  float b;
  float c;
  float d;
  float e;
  float f;
} struct_message;
 
//struktura obiektu
struct_message danePogoda;
 
// Dane przesyłane
void OdbiorDanych(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&danePogoda, incomingData, sizeof(danePogoda));
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2); //wybór portu szeregowego
  
  Serial2.println("<"+String(danePogoda.b)+","+String(danePogoda.c)+ ","+String(danePogoda.e)+","+String(danePogoda.f)+"> "); //format przesyłanych danych
  
  delay(1500);
}
 
void setup() {
 
  Serial2.begin(9600);
 

  WiFi.mode(WIFI_STA);
 

  if (esp_now_init() != ESP_OK) {
    
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  
  esp_now_register_recv_cb(OdbiorDanych);
}
 
void loop() {
 
 }
