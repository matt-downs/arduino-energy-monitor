#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

unsigned long interval=2000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.

// Initialize the Ethernet server library
EthernetServer server(80);

void setup() {
  lcd.init();
  lcd.backlight();
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac);
  server.begin();

  Serial.begin(9600);

  // give the sensor and Ethernet shield time to set up:
  delay(1000);
  
  printIPAddress();
}

void loop() {
  unsigned long currentMillis = millis(); // grab current time
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    updateSensors();
  previousMillis = millis();
  }
  Ethernet.maintain();
  // listen for incoming Ethernet connections:
  listenForEthernetClients();
}

void updateSensors() {
  Serial.print("test");
}

void printIPAddress()
{
  lcd.clear();
  String ip;
  
  for (byte i = 0; i < 4; i++) {
    ip+= Ethernet.localIP()[i];
    if (i != 3) ip += ".";
  }
  lcd.setCursor(0,0);
  lcd.print(ip);
  
  Serial.println();
}

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
          // print the current readings, in HTML format:
          client.print("{energy: 500,");
          client.print("temp: 30,");
          client.print("humidity: 50}");
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

