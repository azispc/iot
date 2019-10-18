#include <WiFi.h>

const char* ssid = "ssid";
const char* password = "***";

const uint16_t port = 8080;
const char * host = "192.168.43.201";

//declare type data hasil temperatur
float temp;
char data[5];

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("...");
}
 Serial.println("WiFi connected with IP: "+ WiFi.localIP().toString());
 Serial.begin(115200);
  pinMode(36, INPUT);
}
void loop(){
  //fungsi temperatur
  float temp=(analogRead(36)*3.3/2046/0.01);
  snprintf(data, 5, "%f", temp);
  delay(1000);
  Serial.println(data);
  String hasil = data;
  WiFiClient client;
  if (!client.connect(host,port)) {
  Serial.println("Connection to host failed");
  delay(1000);
  return;
}
  Serial.println("Connected to server successful!");
  client.print("Temperature Hari Ini:" +hasil);
  Serial.println("Disconnecting...");
  client.stop();
delay(10000);
}
