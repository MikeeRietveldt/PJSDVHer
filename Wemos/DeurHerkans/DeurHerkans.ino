#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#ifndef WIFISSID
#define WIFISSID "BeamersWifi"  // Wifi network name.
#define WIFIPASS "112233ja"     // Password of the wifi network.
#endif

const char* ssid = WIFISSID;
const char* password = WIFIPASS;
const char* host = "192.168.137.10";  //Ip of the Pi, can also be pi@mikePi.local.
const uint16_t port = 8080;

Servo myservo;              // Initialize a servo variable.
WiFiClient client;          // Initialize a wificlient to use for making connections.

// Angles for servo
int deurdicht = 80;
int deuropenin = 170;
int deuropenuit = 0;

void setup() {
  myservo.attach(D5);       // Assign pin D5 to be the servo pin.
  closeDoor();              // Close the door on program startup.
  Serial.begin(115200);     // ================ serieel starten met de juiste baudrate.
  Wire.begin();             // I2C communicatie beginnen.
}

void delaySeconds(int seconds) {
  for (int i = 0; i < seconds; i++) {
    delay(1000);
  } 
}

void loop() {
  // Check if a wifi connection is currently active, otherwise try to establish one.
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
    return;
  }

  // Try to make a connection to the pi server. If no success, reset.
  if (!connectToServer())
    return;
  
  // Set server configuration to match door usage. 
  Serial.print("\nSent: setDeur to server");
  client.print("setDeur");  // ENGLISH =============

  bool hulp = false;
  int previousInput = 999;

  /*  
    Start input reading sequence in a while loop,
    testing on client.connected() to boot code out of the loop when connection gets lost.
  */
  while (client.connected()) {
    int input = readInput();
    // Serial.print("\nInput : ");
    // Serial.print(input);
    // Serial.print("\PreviousInput : ");
    // Serial.print(previousInput);
    
    if (input != 0 && input == previousInput) {
      
      Serial.print("\nSame input detected, skipping...");
      delay(1000);
      continue;
    }
    
    // Set previousInput to the input received on previous cycle, unless its 0 meaning no input, to prevent input spam.
    if (input != previousInput) {
      previousInput = input;
    }

    if (client.connected()) {
      if (input == 1) {
        client.print("deurOpenBinnen");
        delay(1000); // Depends on wifi speed
      }
      if (input == 2) {
        client.print("deurOpenBuiten");
        delay(1000); // Depends on wifi speed
      }
    }

    if (client.available()) {  // Lezen wat de server verstuurt en printen.
      char message = static_cast<const char>(client.read());

      // Print callback from server
      Serial.print("\nPi zegt: ");
      Serial.print(message);

      switch (message) {  
        case '1':
          // Open door inwards
          Serial.print("\nOpen deur naar binnen");
          doorController(1);
          Serial.print("\nDeur gaat dicht");
          previousInput = 0;
          break;

        case '2':
          // Open door outwards
          Serial.print("\nOpen deur naar buiten");
          doorController(2);
          Serial.print("\nDeur gaat dicht");
          previousInput = 0;
          break;

        case '4':
          // Patiënt requires help
          hulp = true;
          Serial.print("\nHelp patiënt!!");
          break;

        case '5':
          // Patiënt no longer requires help
          hulp = false;
          Serial.print("\nPatiënt geholpen :)");
          break;

        default:
          // Empty input
          break;          
      }
    }
    continue;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("\nConnection to wifi network lost");
    return;
  } else {
    Serial.print("\nConnection to server (Pi) lost");
    return;
  }
}

// Attempting to make a connection to wifi network from Pi
void connectToWifi() {                     
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("\nEstablishing Connection to wifi network");
  for (int i = 0; i < 10; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    } else {
      Serial.print("\nWiFi connection established on " + WiFi.localIP().toString());
      return;
    }
  } 
  Serial.print("\nCouldn't establish a connection to wifi network");
}

bool connectToServer() {
  Serial.print("\nEstablishing connection to (Pi) server");

  if (!client.connect(host, port)) {
    Serial.print("\nCouldn't establish a connection to (Pi) server on ");
    Serial.print(host);
    Serial.print(":");
    Serial.print(port);
  }

  for (int i = 0; i < 10; i++) {
    if (client.connected()) {
      Serial.print("\n(Pi) Server connection established on ");
      Serial.print(host);
      Serial.print(":");
      Serial.print(port);
      //Serial.print("" + host + ":" + port.to);
      return 1;
    }
    Serial.print(".");
    delay(1000);
  } 
  
  Serial.print("\nCouldn't establish a connection to (Pi) server on ");
  Serial.print(host);
  Serial.print(":");
  Serial.print(port);
  // Serial.print( + host + ":" + port);
  return 0;
}

int readInput() {
  // Config PCA9554
  // Inside loop for debugging purpose (hot plugging wemos module into i/o board).
  // IO0-IO3 as input, IO4-IO7 as output.
  Wire.beginTransmission(0x38);       // slave adres meegeven
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  // Read PCA9554 outputs (IO40-IO3)
  Wire.beginTransmission(0x38);       // slave adres meegeven
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int inputs = Wire.read();  // hier lees je waarde van knopje met I2C
  return inputs & 0x03;               // knopje waarden teruggeven aan de main.
}

void lichtAan() {
  Serial.println("\nLichtje aan");
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(3 << 4));
  Wire.endTransmission();
}

void lichtUit() {
  Serial.print("Lichtje uit");
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(0));
  Wire.endTransmission();
}

void doorController(int status) {   // pi kan deze functie aanroepen om deur te openen
  lichtAan();                       // licht aan want praktisch voor patiënt

  if (status == 1) {
    myservo.write(deuropenin);
  }
  else if (status == 2) {
    myservo.write(deuropenuit);
  }

  delaySeconds(5);
  closeDoor();                // deur blijft 5 seconden open en gaat dan weer dicht.
}

void closeDoor() {            // later moet brandvariabele meegegeven worden.
  lichtUit();                 // licht hoeft niet meer aan bij een dichte deur
  myservo.write(deurdicht);   // servo schrijven naar deur dicht hoek, 80 graden
}