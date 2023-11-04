#include <SimpleTimer.h>


#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
BlynkTimer timer; 
int Balarm = 14;   //Buzzer  alarm  connected to GPIO-14 or D5 of nodemcu


const int reedSwitch = 4;
// Detects whenever the door changed state
bool changeState = false;

bool state;
String doorState;

unsigned long previousMillis = 0; 
const long interval = 1000;

    
 char auth[] ="JzPboWju3Rb1NFoFpoZHL9Ml7Qi7W3qL";
 
 char ssid[] = "Prasuk's Phone_1";
 char password[] ="Prasukj@101914";
 

// Runs whenever the re  edswitch changes state
ICACHE_RAM_ATTR void changeDoorStatus() {
  Serial.println("State changed");
  changeState = true;
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Read the current door state
  pinMode(reedSwitch, INPUT_PULLUP);
  state = digitalRead(reedSwitch);
  pinMode(Balarm, OUTPUT);   // Buzzer alaram as output
  digitalWrite (Balarm, LOW);

  // Set LED state to match door state
  // Set the reedswitch pin as interrupt, assign interrupt function and set CHANGE mode
  attachInterrupt(digitalPinToInterrupt(reedSwitch), changeDoorStatus, CHANGE);
   Blynk.virtualWrite(V0, state);

 WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");

  // Random seed is a number used to initialize a pseudorandom number generator
  randomSeed(analogRead(0));
   Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
}

 void loop() {
   
  if (changeState){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // If a state has occured, invert the current door state   
      state = !state;
      if(state==0) {
         doorState = "closed";
       
         digitalWrite (Balarm, LOW); 
         Blynk.virtualWrite(V0, state); //No intrusion Buzzer off

      //Serial.println("Motion absent!");
       
        
        }
      else{
        doorState = "open";
        digitalWrite (Balarm, HIGH); 
         Blynk.virtualWrite(V0, state);
         Blynk.virtualWrite(V1, "Alert","WARNNG! Door is open");
         Blynk.logEvent("door","Door is open");
       
        //If intrusion detected ring the buzzer
                       //Ring buzzer for 15 seconds 
      //Serial.println("Motion detected!");
        
         
      }
      changeState = false;
      Serial.println(state);
      Serial.println(doorState);
      
        }
  }
   Blynk.run();//Run the Blynk library
    timer.run();//Run the Blynk timer

  }
