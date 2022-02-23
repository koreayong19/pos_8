/*
wemos d1과 심박센서, 0.96인치 디스플레이 연결
완성코드입니다

연결 단자 설명
디스플레이: VCC - 5V만 가능(3.3V안됨)/ SCL - D1/ SDA - D2
심박센서: VCC - 5V, 3.3V 둘다 가능. 여기서는 단자수 문제로 3.3V에 연결
검은색줄-ground
중간 빨간줄 - Vin
보라줄 - A0
 
 */

#define pulsePin A0
//디스플레이용
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
//디스플레이용
#define OLED_RESET 0
Adafruit_SSD1306 OLED(OLED_RESET);

const char* ssid = "AIEDU-2"; 
const char* password = "0542795613";

//const char* ssid = "Ken"; 
//const char* password = "hyung3569";


WiFiClient client;

const int channelID = 916158;
String writeAPIKey = "417G635Y5OZU9I0F"; 
const char* server = "api.thingspeak.com";
const int postingInterval = 10 * 1000; 

int rate[10];                    
unsigned long sampleCounter = 0; 
unsigned long lastBeatTime = 0;  
unsigned long lastTime = 0, N;
int BPM = 0;
int IBI = 0;
int P = 512;
int T = 512;
int thresh = 512;  
int amp = 100;                   
int Signal;
boolean Pulse = false;
boolean firstBeat = true;          
boolean secondBeat = true;
boolean QS = false;    

void setup() {
  OLED.begin();
  OLED.clearDisplay();
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("와이파이 설정중");
    
    OLED.clearDisplay();
    OLED.setTextWrap(false);
    OLED.setTextSize(1);
    OLED.setTextColor(WHITE);
    OLED.setCursor(0,0);
    OLED.println("Wifi setting");
    OLED.display();
    
    delay(500);
  }
}

void loop() {

  if (QS == true) {
   //Serial.println("BPM: "+ String(BPM));
   QS = false;
   } else if (millis() >= (lastTime + 0.5)) {
     readPulse(); 
     lastTime = millis();
   }     
}



void readPulse() {

  Signal = analogRead(pulsePin);
  //Serial.println(analogRead(pulsePin));
                
  sampleCounter += 2;                           
  int N = sampleCounter - lastBeatTime;   

  detectSetHighLow();

  if (N > 250) {  
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI / 5) * 3) )
      pulseDetected();
  }

  if (Signal < thresh && Pulse == true) {  
    Pulse = false;
    amp = P - T;
    thresh = amp / 2 + T;  
    P = thresh;
    T = thresh;
  }

  if (N > 2500) {
    thresh = 512;
    P = 512;
    T = 512;
    lastBeatTime = sampleCounter;
    firstBeat = true;            
    secondBeat = true;           
  }

}

void detectSetHighLow() {

  if (Signal < thresh && N > (IBI / 5) * 3) {
    if (Signal < T) {                       
      T = Signal;                         
    }
  }

  if (Signal > thresh && Signal > P) {    
    P = Signal;                           
  }                                       

}

void pulseDetected() {
  Pulse = true;                           
  IBI = sampleCounter - lastBeatTime;     
  lastBeatTime = sampleCounter;           

  if (firstBeat) {                       
    firstBeat = false;                 
    return;                            
  }
  if (secondBeat) {                    
    secondBeat = false;                
    for (int i = 0; i <= 9; i++) {   
      rate[i] = IBI;
    }
  }

  word runningTotal = 0;                   

  for (int i = 0; i <= 8; i++) {          
    rate[i] = rate[i + 1];            
    runningTotal += rate[i];          
  }

  rate[9] = IBI;                      
  runningTotal += rate[9];            
  runningTotal /= 10;                 
  BPM = 60000 / runningTotal;         
  QS = true;
  if (client.connect(server, 80)) {
    
    String body = "field1=";
           body += BPM;
           
    Serial.println("BPM: "+ String(BPM));
    
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: ESP8266 (nothans)/1.0");
    client.println("Connection: close");
    client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(body.length()));
    client.println("");
    client.print(body);

    OLED.clearDisplay();
    OLED.setTextWrap(false);
    OLED.setTextSize(3);
    OLED.setTextColor(WHITE);
    OLED.setCursor(0,0);
    OLED.println("BPM");
    OLED.println(BPM);
    OLED.display();
    
  }
  client.stop();
  delay(postingInterval);                              
}
