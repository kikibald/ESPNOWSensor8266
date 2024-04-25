#include <espnow.h>
#include <ESP8266WiFi.h>
uint8_t receiverAddress[] = {0xEC, 0xFA, 0xBC, 0x8A, 0xB3, 0xD0};

#define ID 1
#define TYPE 1 // Bodenfeuchte
// define TYPE 2 // Lufttemperatur/Luftfeuchte/Helligkeit


typedef struct struct_outMessage {
  int id = ID; // must be unique for each sender board
  int type = TYPE;
  int data1=0;
  int data2=0;
  int data3=0;
  int data4=0;
  int data5=0;
  int data6=0;
  int data7=0;
  int data8=0;
} struct_outMessage;

struct_outMessage outMessage;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
void initESP(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } 
  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Once ESPNow is successfully init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void setup() {
  Serial.begin(115200);
  initESP();
  

}

void loop() {
  
 esp_now_send(0, (uint8_t *) &outMessage, sizeof(outMessage));
  delay(3000);
}
