#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>
#include <DHT.h>

// Expose some settings for super easy access
#define DisplayUpdateInterval 5000
#define SensorUpdateInterval 1000

DHT dht(2, DHT11);
LiquidCrystal_I2C lcd(0x27,20,4);

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetServer server(80);

// Initialise ALL the variables! (mostly)
unsigned long prevDisplayUpdate = 0;
unsigned long prevSensorUpdate = 0;
float temperature = 0;
float humidity = 0;
float energy = 0;
int disp = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Loading...");
  
  dht.begin();
  
  Ethernet.begin(mac);
  server.begin();

  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Update the display every n seconds
  if ((unsigned long)(currentMillis - prevDisplayUpdate) >= DisplayUpdateInterval) {
    updateScreen();
    // Also make sure the network connection is maintained
    Ethernet.maintain();
    prevDisplayUpdate = millis();
  }

  // Update the sensors every n seconds
  if ((unsigned long)(currentMillis - prevSensorUpdate) >= SensorUpdateInterval) {
    updateSensors();
    prevSensorUpdate = millis();
  }
    
  // listen for incoming Ethernet connections:
  listenForEthernetClients();
}

// *********************************************************************
// Updates the sensors
// *********************************************************************
void updateSensors() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  energy = 123.43;
}

// *********************************************************************
// Updates the display
// *********************************************************************
void updateScreen() {
  lcd.clear();
  printIPAddress();

  lcd.setCursor(0,1);
  if (disp == 0) lcd.print((String)temperature + " C");
  if (disp == 1) lcd.print((String)humidity + " %");
  if (disp == 2) lcd.print((String)energy + " watts");

  disp++;
  if (disp == 3) disp = 0;
}

// *********************************************************************
// Prints the current IP address on top line of the display
// *********************************************************************
void printIPAddress()
{
  String ip;
  
  for (byte i = 0; i < 4; i++) {
    ip+= Ethernet.localIP()[i];
    if (i != 3) ip += ".";
  }
  
  lcd.setCursor(0,0);
  lcd.print(ip);
}
// *********************************************************************
//
// *********************************************************************
void listenForEthernetClients() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println();
          // print the current readings, in JSON format:
          client.print("{energy: " + (String)energy + ",");
          client.print("temp: " + (String)temperature + ",");
          client.print("humidity: " + (String)humidity + "}");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}

