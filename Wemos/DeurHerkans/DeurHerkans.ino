#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#ifndef WIFISSID
#define WIFISSID "BeamersWifi" // Wifi naam
#define WIFIPASS "112233ja"  // Wachtwoord van de wifi
#endif

const char* ssid = WIFISSID;
const char* password = WIFIPASS;
const char* host = "192.168.137.10";  //ip van de pi, kan ook pi@mikePi.local zijn
const uint16_t port = 8080;

Servo myservo;

int deurdicht = 80;  // deur sluiten
int deuropenin = 170;  // deur openen naar binnen zijde
int deuropenuit = 0; // deur openen naar buiten zijde

void setup() {
  myservo.attach(D5);    // geef aan op welke pin de servo zit
  deurSluiten();         // deur sluiten wanneer device gestart wordt
  Serial.begin(115200);  // serieel starten met de juiste baudrate
  Wire.begin();          // I2C communicatie beginnen
  wifiVerbinden();       // wifi verbinden aanroepen, deze zorgt ervoor dat we gaan verbinden met de juiste server.
}

void loop() {
  bool brand = false;
  bool hulp = false;
  WiFiClient client = verbindenPi();  // verbinding maken met de host/server Pi
  if(client.connected()){
    client.print("setDeur");
  }
  int timer1ms = 0;
  int vorigeKnopjes = 999;

  while (1) {
    int knopjes = knopjesLezen();
    if (knopjes != vorigeKnopjes) {
      Serial.print("Waarde knopjes:");
      Serial.println(knopjes);
    }
    vorigeKnopjes = knopjes;

    if (client.connected()) {
      if (knopjes == 1) {
        client.print("deurOpenBinnen");
        delay(300);
      }
      if (knopjes > 1) {
        client.print("deurOpenBuiten");
        delay(300);
      }
      knopjes = 0;
    }

    if (client.available()) {  // Lezen wat de server verstuurt en printen.
      char ch = static_cast<const char>(client.read());
      Serial.print("pi zegt:");
      Serial.print(ch);
      Serial.print("\n");

      if (ch == '1' && !brand) { // Pi stuurt dat deur geopend moet worden
        deurOpenen(brand);
        Serial.println("Open deur");
      }
      if (ch == '8') { // Pi stuurt dat er brand is
        brand = true;
        deurOpenen(brand);
        Serial.println("Brand!!");
      }
      if (ch == '9') { // Pi stuurt dat de brand voorbij is.
        brand = false;
        deurOpenen(brand);
        Serial.println("Geen brand!");
      }
      if (ch == '4') { // Pi stuurt dat patiënt hulp nodig heeft
        hulp = true;
        Serial.println("Help patiënt!!");
      }
      if (ch == '5') { // Pi stuurt dat patiënt geen hulp meer nodig heeft
        hulp = false;
        Serial.println("Patiënt geholpen :)");
      }
      ch = '0';  // zodat deur niet dubbel commando's gaat doen. voor het geval dat.
    }

    if (!client.connected()) {
      Serial.println("Verbinding met Server (Pi) verbroken");
      break; // zodat wemos opnieuw verbindt met de Pi
    }
    if ((WiFi.status() != WL_CONNECTED)) {
      Serial.println("Verbinding met WiFi verbroken");
      setup(); // zodat wemos opnieuw verbindt tot het access-point
    }
    delay(1); 
    if (!brand && hulp) {
      if (timer1ms < 1001) {
        timer1ms++;
      }
      if (timer1ms > 499) {
        lichtAan();
      }
      if (timer1ms > 998) {
        timer1ms = 0;
        lichtUit();
      }
    }
  }
}

void deurOpenen(bool brandje) {  // pi kan deze functie aanroepen om deur te openen
  lichtAan(); // licht aan want praktisch voor patiënt
  myservo.write(deuropenin); // servo opent de deur
  int knop;
  for (int i = 0; i < 500; i++) {
    int knop = knopjesLezen();
    if (knop > 0) { // > 0 betekent dat een knop is ingedrukt
      return;
    }
    delay(10);
  }
  //delay(50);
  if (!brandje) {
    deurSluiten();  // deur blijft 5 seconden open en gaat dan weer dicht.
  }
}

void deurSluiten() {  // later moet brandvariabele meegegeven worden.
  lichtUit(); // licht hoeft niet meer aan bij een dichte deur
  myservo.write(deurdicht);  // servo schrijven naar deur dicht hoek, 80 graden
}

void wifiVerbinden() {                     // verbinding maken met Accespoint van de Pi.
  WiFi.mode(WIFI_STA);                     //
  WiFi.begin(ssid, password);              // arduino library doet het moeilijke werk
  while (WiFi.status() != WL_CONNECTED) {  // print . terwijl hij aan t verbinden is.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int knopjesLezen() {
  //Config PCA9554
  //Inside loop for debugging purpose (hot plugging wemos module into i/o board).
  //IO0-IO3 as input, IO4-IO7 as output.
  Wire.beginTransmission(0x38);  // slave adres meegeven
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  //Read PCA9554 outputs (IO40-IO3)
  Wire.beginTransmission(0x38);  // slave adres meegeven
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int inputs = Wire.read();  // hier lees je waarde van knopje met I2C
  return inputs & 0x03;               // knopje waarden teruggeven aan de main.
}

WiFiClient verbindenPi() {  // verbinden met de pi
  Serial.print("connecting to: ");
  Serial.print(host);
  Serial.print(" :");
  Serial.println(port);

  // Hiermee zet je de Wificlient class in TCP connectie modus.
  WiFiClient client;  // client aanmaken
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
  }
  return client;
}

void lichtAan() {
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(3 << 4));
  Wire.endTransmission();
}

void lichtUit() {
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(0));
  Wire.endTransmission();
}