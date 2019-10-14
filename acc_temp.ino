#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "Moh.Azis";
const char* password = "12345678";

IPAddress local_ip(192,168,1,2);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

const int led1 = LED_BUILTIN;
bool led1status = LOW;
float temp;

void handle_OnConnect() {
  server.send(200, "text/html",SendHTML(led1status));
}

void handle_led1on() {
  led1status = HIGH;
  server.send(200, "text/html",SendHTML(led1status));
  Serial.println("LED dinyalakan.");
}

void handle_led1off() {
  led1status = LOW;
  server.send(200, "text/html",SendHTML(led1status));
  Serial.println("LED dimatikan.");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ":" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(led1, OUTPUT);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  Serial.println("");
  /*
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }*/

  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  Serial.begin(115200);
  pinMode(36, INPUT); // pin output LM-35  
}


void loop(void) {
  server.handleClient();
  if (led1status) {
    digitalWrite(led1, 1);
    } // LED1 seharusnya menyala
  else {
    digitalWrite(led1, 0); 
    } // LED0 seharusnya menyala
    
  temp = analogRead(36) * 3.3 / 2046 / 0.01;
  Serial.println(temp);
  delay(1000);
  }
  
  String SendHTML(uint8_t led1stat){
    String teks = "<!DOCTYPE html> <html>\n";
    teks += "<h2>Suhu :"; 
    teks += temp;
    teks += "</h2>";
    if (led1status) {
      teks += "<h2>Status LED:MENYALA</h2><br>";
      }
    else {
      teks += "<h2>Status LED: MATI</h2><br>";
    }
    
    teks += "<a href=\"/led1on\"\"><button>Nyalakan LED </button></a>";
    teks += "<a href=\"/led1off\"\"><button>Matikan LED </button></a>";
    teks += "</html>";
  return teks; 
}
