/*
 * This is a basic example on how to use Espalexa with RGB color devices.
 */ 
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#define ESPALEXA_ASYNC
#include <Espalexa.h>

#include <IRremote.h>
const int IR_SEND_PIN = 13;
uint32_t reverseBits( uint32_t val ) {
  uint32_t ret = 0;
  for (uint8_t i = 0; i < 32; i++)
  {
    ret = (ret << 1) | ((val >> i) & 1);
  }
  return ret;
}

int off = 0;
int red;
int blue;
int green;
// prototypes
boolean connectWifi();

//callback function prototype
void colorLightChanged(uint8_t brightness, uint32_t rgb);

// Change this!!
const char* ssid = "SSID";
const char* password = "Pass";

boolean wifiConnected = false;

Espalexa espalexa;

void setup()
{
  Serial.begin(115200);
  // Initialise wifi connection
  wifiConnected = connectWifi();
  IrSender.begin(IR_SEND_PIN, DISABLE_LED_FEEDBACK); //Now included as per GitHub

  if(wifiConnected){
    espalexa.addDevice("LED Strip", colorLightChanged);

    espalexa.begin();
    
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}
 
void loop()
{
   espalexa.loop();
   delay(1);
}

//the color device callback function has two parameters
void colorLightChanged(uint8_t brightness, uint32_t rgb) {
  // do what you need to do here, for example, control RGB LED strip
  //Serial.print("Brightness: ");
  //Serial.print(brightness);
  //Serial.print(", Red: ");
 // Serial.print((rgb >> 16) & 0xFF); // get red component
 // Serial.print(", Green: ");
  //Serial.print((rgb >> 8) & 0xFF);  // get green
  //Serial.print(", Blue: ");
  //Serial.println(rgb & 0xFF);       // get blue

  red = (rgb >> 16) & 0xFF;
  green = (rgb >> 8) & 0xFF;
  blue = rgb & 0xFF;
  //Serial.print(red);
  //Serial.print(",");
  //Serial.print(green);
  //Serial.print(",");
  //Serial.println(blue);
  
  if (brightness == 0) {
    Serial.println("Lights Off");
    IrSender.sendNECMSB(reverseBits(0xFD02EF00),32); //off
    //IrSender.sendNECMSB(reverseBits(0xFC03EF00),32); //on
    off = 0;
    Serial.print("Lights are off");
  }else if (brightness == 255 && off == 0) {
    IrSender.sendNECMSB(reverseBits(0xFC03EF00),32); //on
    off = 1;
    Serial.print("Lights are on now, they were off");

  

  } else if (red == 255 && green == 192 && blue == 140) {
    Serial.println("Just White");
    IrSender.sendNECMSB(reverseBits(0xF807EF00),32); 


  } else if (red == 255 && green == 0 && blue == 0) {
    Serial.println("Just Red");
    IrSender.sendNECMSB(reverseBits(0xFB04EF00),32); 


  } else if (red == 0 && green == 255 && blue == 0) {
    Serial.println("Just Green");
    IrSender.sendNECMSB(reverseBits(0xFA05EF00),32);

  } else if (red == 0 && green == 0 && blue == 255) {
    IrSender.sendNECMSB(reverseBits(0xF906EF00),32);
    Serial.println("Just Blue");

 } else if (red == 255 && green == 166 && blue == 0) {
      IrSender.sendNECMSB(reverseBits(0xF708EF00),32);

    Serial.println("Orange");

 } else if (red == 254 && green == 255 && blue == 0) {
      IrSender.sendNECMSB(reverseBits(0xEB14EF00),32);

    Serial.println("Yellow");

 } else if (red == 71 && green == 255 && blue == 236) {
    IrSender.sendNECMSB(reverseBits(0xF609EF00),32);
    Serial.println("Turquoise");

 } else if (red == 147 && green == 224 && blue == 255) {
    IrSender.sendNECMSB(reverseBits(0xEA15EF00),32);
    Serial.println("Sky Blue");

 } else if (red == 159 && green == 127 && blue == 255) {
    IrSender.sendNECMSB(reverseBits(0xE916EF00),32);
    Serial.println("Lavender");
  }
}



// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 40){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
