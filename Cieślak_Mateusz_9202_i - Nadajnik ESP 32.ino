// Mateusz Cieślak nr indeksu 9202
// biblioteki wykorzystane i ich definicja
#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>
#include <PMS.h>
#define DHTPIN 23     // Pin podłączenia DHT
#define DHTTYPE DHT22   // Typ DHT
#define RXD2   16 // pin nadajnika
#define TXD2   17 //pin nadajnika
DHT dht(23, DHT22);
PMS pms(Serial1);
PMS::DATA data;
float humidity;                          
uint8_t broadcastAddress[] =   {0xEC,0x62,0x60,0x76,0xF3,0x1C};  // adres mac odbiornika
 
// definicja struktury danych
typedef struct struct_message {
  char a[32];
  float b;
  float c;
  float d;
  float e;
  float f;
} struct_message;
 
// struktura obiektu
struct_message danePogoda;
 
// informacje o peer
esp_now_peer_info_t peerInfo;
 
// metoda wysyłania danych
void WysylaneDane(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {

  
  Serial.begin(115200); //Serial ustawiony na 115200 baud
  Serial1.begin(9600, SERIAL_8N1, 16, 17);
  dht.begin();
  Serial.println("Serial Txd is on pin: "+String(16));
  Serial.println("Serial Rxd is on pin: "+String(17));
  
  WiFi.mode(WIFI_STA);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Błąd inicjalizacji ESP-NOW");
    return;
  }
 
 
  esp_now_register_send_cb(WysylaneDane);
  
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Nie udało się dodać peera");
    return;
  }
}
 
void loop() {
 
  if (pms.read(data))
  {
    Serial.print("PM 1.0 (ug/m3): "); 
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
  }
  float temperature = dht.readTemperature(); // odczyt temperatury
  float temp = dht.readTemperature(); // odczyt temp_zew
  float humidity = dht.readHumidity();        // odczyt wilgotności
  float wilg = dht.readHumidity(); // odczyt wilgotności
   if (isnan(temperature) || isnan(humidity)) 
   {
    Serial.println("Nie udało się odczytać z czujnika DHT!");
    return;
   }
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C   Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  
  delay(1000);
 
 


  danePogoda.b = temperature;
  danePogoda.c = humidity;
  danePogoda.d = data.PM_AE_UG_1_0;
  danePogoda.e = data.PM_AE_UG_2_5;
  danePogoda.f = data.PM_AE_UG_10_0;
  Serial.print("PM 1.0 (ug/m3): ");
  Serial.println(danePogoda.d);
  Serial.print("PM 2.5 (ug/m3): ");
  Serial.println(danePogoda.e);
  Serial.print("PM 10.0 (ug/m3): ");
  Serial.println(danePogoda.f);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &danePogoda, sizeof(danePogoda));
   
  if (result == ESP_OK) {
    Serial.println("Potwierdzone wysyłanie");
  }
  else {
    Serial.println("Błąd przesyłania");
  }
  delay(2000);
}

