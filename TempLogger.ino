#include <OneWire.h>
#include <DallasTemperature.h>
#include<stdlib.h>
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define SSID "XXX"
#define PASS "XXX"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /update?key=XXXXXXXX&field1=";


void setup()
{
  Serial.begin(9600);
  sensors.begin();
  Serial.println("AT");
  delay(5000);
  if(Serial.find("OK")){
    connectWiFi();
  }
}

void loop(){
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  tempC = DallasTemperature::toFahrenheit(tempC);
  char buffer[10];
  String tempF = dtostrf(tempC, 4, 1, buffer);
  updateTemp(tempF);
  delay(60000);
}

void updateTemp(String tenmpF){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  cmd = GET;
  cmd += tenmpF;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    Serial.print(cmd);
  }else{
    Serial.println("AT+CIPCLOSE");
  }
}

 
boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")){
    return true;
  }else{
    return false;
  }
}
