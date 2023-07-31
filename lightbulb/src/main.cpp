#include <Arduino.h>


  #include <WiFi.h>

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"



#include <Arduino_JSON.h>

// Replace with your network credentials
const char* ssid = "TAB";
const char* password = "faustyna";
AsyncWebServer server(80);


const int M1 = 5;
const int M2 = 18;
const int M3 = 15;
const int Pin = 4;
const int N1 = 19;
const int N2 = 21;
const int N3 = 22;
const int N4 = 23;
const int WYL=5;

// Initialize Telegram BOT
#define BOTtoken "5983991434:AAFGZr1j1rlpcMDb5PH2EenfEYNaZ4mkLXQ"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "5444708628"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;


void initSPIFFS() {
 if (!SPIFFS.begin()) {
 Serial.println("An error has occurred while mounting SPIFFS");
 }
 Serial.println("SPIFFS mounted successfully");
}
void initWiFi() {
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 Serial.print("Connecting to WiFi ..");
 while (WiFi.status() != WL_CONNECTED) {
 Serial.print('.');
 delay(1000);
 }
 Serial.println(WiFi.localIP());
}

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Cześć " + from_name + ".\n";
      welcome += "Użyj tych komend aby obsłużyć wyjścia pojazdu:.\n\n";
      welcome += "/prosto -aby jechać prosto  \n";
      welcome += "/lewo -aby skręcić w lewo \n";
      welcome += "/prawo -aby skręcić w prawo oraz /zawroc by zawrócić  \n";
       welcome += "/zawroc by zawrócić  \n";
      welcome += "/off -aby wyłączyć  \n";

      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/prosto") {
      bot.sendMessage(chat_id, "jedzie prosto ", "");
         digitalWrite(M1, HIGH);
    digitalWrite(M2, LOW);
     digitalWrite(M3, LOW);
      digitalWrite(Pin, HIGH);
       digitalWrite(N1, LOW);
    digitalWrite(N2, HIGH);
     digitalWrite(N3, LOW);
      digitalWrite(N4, HIGH);
      
    }
    
    if (text == "/right") {
      bot.sendMessage(chat_id, "jedź w prawo ", "");
     /*digitalWrite(ledPin, LOW);
 digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, LOW);
 digitalWrite(ledPin4, HIGH);
 */
 digitalWrite(M1,LOW);
    digitalWrite(M2, HIGH);
     digitalWrite(M3, HIGH);
      digitalWrite(Pin, LOW);
       digitalWrite(N1, LOW);
    digitalWrite(N2, LOW);
     digitalWrite(N3, HIGH);
      digitalWrite(N4, LOW);
    }
    
    if (text == "/left") {
       bot.sendMessage(chat_id, "jedź w lewo ", "");
      /*digitalWrite(ledPin, HIGH);
 digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, HIGH);
 digitalWrite(ledPin4, LOW);
 */
digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
     digitalWrite(M3, LOW);
      digitalWrite(Pin, HIGH);
       digitalWrite(N1, LOW);
    digitalWrite(N2, HIGH);
     digitalWrite(N3, LOW);
      digitalWrite(N4, LOW);
 
    }
    if (text == "/zawroc") {
       bot.sendMessage(chat_id, "zawroc ", "");
     /* digitalWrite(ledPin, LOW);
 digitalWrite(ledPin2, HIGH);
 digitalWrite(ledPin3, HIGH);
 digitalWrite(ledPin4, LOW);*/
   digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
     digitalWrite(M3, HIGH);
      digitalWrite(Pin, LOW);
       digitalWrite(N1, HIGH);
    digitalWrite(N2, LOW);
     digitalWrite(N3, HIGH);
      digitalWrite(N4, LOW);
 
    }
  
  if(text=="/off")
  {
    bot.sendMessage(chat_id,"wyłącz","");
  /*digitalWrite(ledPin, HIGH);
 digitalWrite(ledPin2, HIGH);
 digitalWrite(ledPin3, HIGH);
 digitalWrite(ledPin4, HIGH);
 */
 digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
     digitalWrite(M3, LOW);
      digitalWrite(Pin, LOW);
       digitalWrite(N1, HIGH);
    digitalWrite(N2, HIGH);
     digitalWrite(N3, LOW);
      digitalWrite(N4, LOW);
  }
  }
}

void setup() {
  

  


 Serial.begin(9600);
 initWiFi();
 initSPIFFS();
 
 // Set GPIO2 as an OUTPUT
  pinMode(M1 , OUTPUT);
 pinMode(M2 , OUTPUT);
 pinMode(M3 , OUTPUT);
 pinMode(Pin , OUTPUT);
 pinMode(N1 , OUTPUT);
 pinMode(N2 , OUTPUT);
 pinMode(N3 , OUTPUT);
 pinMode(N4 , OUTPUT);
 pinMode(WYL,INPUT_PULLUP);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
 request->send(SPIFFS, "/index.html", "text/html", false);
 /*if(digitalRead(WYL)==LOW)
  {
    digitalWrite(ledPin,HIGH);
    digitalWrite(ledPin2,HIGH);
  }else{
    digitalWrite(ledPin,LOW);
     digitalWrite(ledPin2,LOW);


  }*/
 
 });
server.serveStatic("/", SPIFFS, "/");
 // Route to set GPIO state to HIGH
 server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
  digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
     digitalWrite(M3, LOW);
      digitalWrite(Pin, LOW);
       digitalWrite(N1, HIGH);
    digitalWrite(N2, HIGH);
     digitalWrite(N3, LOW);
      digitalWrite(N4, LOW);

 request->send(SPIFFS, "/index.html", "text/html", false);
 });
 server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request){
 digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
     digitalWrite(M3, LOW);
      digitalWrite(Pin, HIGH);
       digitalWrite(N1, LOW);
    digitalWrite(N2, HIGH);
     digitalWrite(N3, LOW);
      digitalWrite(N4, LOW);

 
 request->send(SPIFFS, "/index.html", "text/html", false);
 });
 server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request){
 digitalWrite(M1,LOW);
    digitalWrite(M2, HIGH);
     digitalWrite(M3, HIGH);
      digitalWrite(Pin, LOW);
       digitalWrite(N1, LOW);
    digitalWrite(N2, LOW);
     digitalWrite(N3, HIGH);
      digitalWrite(N4, LOW);
 
 request->send(SPIFFS, "/index.html", "text/html", false);
 });
 server.on("/zawroc", HTTP_GET, [](AsyncWebServerRequest *request){
digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
     digitalWrite(M3, HIGH);
      digitalWrite(Pin, LOW);
       digitalWrite(N1, HIGH);
    digitalWrite(N2, LOW);
     digitalWrite(N3, HIGH);
      digitalWrite(N4, LOW);

 
 request->send(SPIFFS, "/index.html", "text/html", false);
 });


 server.on("/prosto", HTTP_GET, [](AsyncWebServerRequest *request){
 digitalWrite(M1, HIGH);
    digitalWrite(M2, LOW);
     digitalWrite(M3, LOW);
      digitalWrite(Pin, HIGH);
       digitalWrite(N1, LOW);
    digitalWrite(N2, HIGH);
     digitalWrite(N3, LOW);
      digitalWrite(N4, HIGH);

 
 request->send(SPIFFS, "/index.html", "text/html", false);
 });
 




 server.begin();
}



void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}