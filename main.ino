#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP32WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>  
#include <ArduinoJson.h>
#include <HTTPClient.h>
const char* ssid = "";
const char* password = "";
const char* serverName = "https://api.thingspeak.com/update?api_key=64A121SOW112X84M&field1=0";
String apiKey = "64A121SOW112X84M";
#define BOTtoken "2147382245:AAG_BjSMEsrBPZeq1cVdqJYh6ddG859h4V0"  
#define CHAT_ID ""
#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif
int v,r,n;
int a ,b ,c ;
int cashremaining;
int cashdispensed;
int x = 5,y = 10,z = 10;
//x is 2000
//y is 1000
//z is 500
int atm_val= 2000*x+1000*y+500*z ;
int bal = 22000;
int amount ;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


int botRequestDelay = 1000;
unsigned long lastTimeBotRan;



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
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/login to generate OTP \n";
      welcome += "/balance\n";
      welcome += "/transaction \n";
      welcome += "/amount withdrawn \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    v = random(10,99);
    if (text == "/login")
    {
      bot.sendMessage(chat_id, "OTP is \n" + String(v) ,"");
    }\
    
    if (text == "/balance")
    {
      bot.sendMessage(chat_id, "Your Account Balance is\n" + String(bal) ,"");
    }
     
     if (text == "/transaction")
    {
      Serial.println("Enter OTP \n");
      Serial.write(n);
      Serial.println(n);
      if(n == v)
      {
        Serial.println("Transaction Authorised");
        Serial.write(amount);
        Serial.println(amount);
          if(amount > atm_val)
        Serial.println("sufficient cash not in the device");
          if(amount> bal)
        Serial.println("Sufficient Funds not available");
  
        Serial.println("Enter which notes you want");
        Serial.write(a);
        Serial.println(a);
        Serial.write(b);
        Serial.println(b);
      
        Serial.write(c);
        Serial.println(c);
        cashdispensed = 2000*x + 1000*y + 500*z;
        if(cashdispensed = amount)
        Serial.println("amount is dispensed");
        x = x - a ;
        y = y - b ;
        z = z - c;
        bal = bal - amount;
        atm_val = atm_val - amount;
        Serial.print("atm_val"); 
        Serial.print(atm_val);
        Serial.print("2000 denomination ");
        Serial.print(x);
        Serial.print("1000 denomination ");
        Serial.print(y);
        Serial.print("500 denomination ");
        Serial.print(z);
        Serial.println("balance available is");
        Serial.println(bal); 
        
      }
      
      bot.sendMessage(chat_id, "\n" + String(bal) ,"");
    }
    if (text == "/amount")
    {
      
      bot.sendMessage(chat_id, "\n" + String(amount) ,"");
    }
  }
}
void setup() {
  
  
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert); 
  #endif

  
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  //Serial.println("Enter OTP \n");
  
  //Serial.write(n);
  //if(n == v)
    //Serial.println("Transaction Authorised");
  //else
    //Serial.println("Wrong OTP");
  //Serial.println("Enter amount");
  //Serial.write(amount);
  //if(amount > atm_val)
    //Serial.println("sufficient cash not in the device");
  //if(amount> bal)
    //Serial.println("Sufficient Funds not available");
  
  //Serial.println("Enter which notes you want");
  //Serial.write(a);
  //Serial.write(b);
  //Serial.write(c);
  //cashdispensed = 2000*x + 1000*y + 500*z;
  //if(cashdispensed = amount)
    //Serial.println("amount is dispensed");
    //x = x - a ;
    //y = y - b ;
    //z = z - c;
    //bal = bal - amount;
    //atm_val = atm_val - amount;
  //Serial.println("balance available is");
  //Serial.println(bal);   
  
}

void loop() 
{
  if (millis() > lastTimeBotRan + botRequestDelay)  
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) 
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
   if(WiFi.status() == WL_CONNECTED )
  {
    HTTPClient http;
    http.begin(serverName);
    String DataSent = "api_key=" +apiKey + "&field1=" + String(atm_val) + "&field2=" + String(x) +  "&field3=" + String(y) + "&field4=" + String(z);
    int Response = http.POST(DataSent);
    //int Response = http.POST(DataSent);
    
    
    //String DataSent = "api_key=" +apiKey + "&field1=" + String(v) + "&field2=" + String((temprature_sens_read() - 32) / 1.8));
    //int Response = http.POST(DataSent);
    http.end();
  }

  
}