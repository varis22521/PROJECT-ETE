
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define EEPROM_STATE_ADDRESS_1 128
#define EEPROM_STATE_ADDRESS_2 144

#define soil 2
#define rain 12
#define Gas A0
int state = 0;
int led = 14;
int ledst = 15;

int buz = 13;
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1);

// Bitmap of MarilynMonroe Image


WidgetTerminal terminal(V3);
WidgetLED led1(V1);
WidgetLED led2(V2);
WidgetLED led3(V3);
WidgetLED led4(V4);
WidgetLED led5(V5);
WidgetLED led6(V6);
WidgetLED led7(V7);
WidgetLED led8(V8);

void gasDetect(void);
char ssid[] = "A30";
char pass[] = "ae246532";
char auth[] = "Tx07n9HuWYokKZTHhaUVge5P-LRyvyQ-";

  // 'atom', 128x64px
// 'def', 124x64px
const unsigned char myBitmap [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xe0, 0x1f, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x0f, 0x8f, 0xff, 0xff, 0xc3, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xc3, 0x8f, 0xf8, 0x21, 0xe0, 0xff, 0xe7, 0x8f, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xfe, 0x3c, 0xfc, 0x3c, 0x73, 0xcf, 0x3f, 0xfe, 0x70, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xe3, 0x3f, 0xf3, 0xfe, 0x73, 0x9f, 0x31, 0xe7, 0xf3, 0x8f, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0x9d, 0xf0, 0xe7, 0xfe, 0x73, 0xce, 0x33, 0xe7, 0x3e, 0x73, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xfc, 0x67, 0xcf, 0xf3, 0xec, 0x20, 0xe0, 0xf3, 0xc7, 0x1f, 0xd8, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xf9, 0x9f, 0xc0, 0x18, 0x1f, 0x80, 0x03, 0xf9, 0x8e, 0x3f, 0xf7, 0x3f, 0xff, 0xf0, 
  0xff, 0xff, 0xe7, 0x7f, 0xff, 0x9f, 0x8f, 0xff, 0xff, 0xc7, 0x7c, 0x7f, 0xfd, 0x8f, 0xff, 0xf0, 
  0xff, 0xff, 0x9b, 0xff, 0xf0, 0x67, 0xff, 0xff, 0xff, 0xff, 0x98, 0xff, 0xff, 0xb3, 0xff, 0xf0, 
  0xff, 0xfe, 0x6f, 0xff, 0xfe, 0xff, 0xfe, 0x6d, 0xde, 0xff, 0xfd, 0x8f, 0xff, 0xec, 0xff, 0xf0, 
  0xff, 0xf9, 0xbb, 0xff, 0xe7, 0xff, 0xba, 0x6c, 0x9e, 0xfe, 0xff, 0xdf, 0xff, 0xfa, 0x3f, 0xf0, 
  0xff, 0xf3, 0x77, 0xff, 0xdf, 0xf1, 0x8a, 0x6c, 0x3e, 0x36, 0x3f, 0xf7, 0xff, 0xdd, 0x9f, 0xf0, 
  0xff, 0xef, 0xc9, 0xde, 0xff, 0xef, 0xb3, 0x6d, 0x7e, 0xf6, 0xdf, 0xfd, 0xff, 0x8e, 0xcf, 0xf0, 
  0xff, 0xcb, 0x88, 0x4d, 0xff, 0x68, 0xbb, 0x9f, 0xfe, 0xf4, 0xd8, 0xff, 0x7e, 0x17, 0xe7, 0xf0, 
  0xff, 0x3f, 0xf0, 0xaf, 0xef, 0xa4, 0xff, 0xfb, 0x3f, 0xfc, 0x19, 0xef, 0xd9, 0xc3, 0xd9, 0xf0, 
  0xfe, 0x6f, 0xfc, 0x5f, 0xe0, 0xbf, 0xfe, 0xdf, 0xfe, 0xff, 0xf7, 0xdf, 0xf5, 0xcf, 0xed, 0xf0, 
  0xfc, 0xfb, 0xf3, 0x7f, 0xfe, 0xff, 0xfd, 0xcb, 0xb2, 0xff, 0xf9, 0xdf, 0xfb, 0x3f, 0xf6, 0xf0, 
  0xf9, 0x31, 0xfa, 0xff, 0xf1, 0xff, 0xef, 0xff, 0x6f, 0xed, 0xff, 0xbf, 0xfd, 0xbf, 0xfb, 0x30, 
  0xf3, 0x68, 0x35, 0xff, 0xff, 0xff, 0x17, 0x83, 0x87, 0x52, 0xff, 0xff, 0xff, 0x8b, 0x9d, 0x30, 
  0xe6, 0xff, 0xcb, 0xff, 0xff, 0xff, 0x7b, 0xcc, 0xe7, 0x9d, 0xff, 0xff, 0xff, 0xbc, 0x7e, 0xd0, 
  0xed, 0xff, 0x77, 0xff, 0xff, 0xff, 0xbf, 0x5e, 0xfd, 0xfb, 0xbf, 0xff, 0xff, 0xf1, 0xfe, 0xd0, 
  0xed, 0xbf, 0xaf, 0xff, 0xff, 0xfb, 0x80, 0xb3, 0xf3, 0x07, 0xff, 0xff, 0xff, 0xdb, 0xfb, 0x60, 
  0xdb, 0x0f, 0xcf, 0xff, 0xff, 0xfd, 0xf9, 0xf1, 0x1b, 0xbf, 0xff, 0xff, 0xff, 0xef, 0xc3, 0x20, 
  0xd3, 0xf0, 0x5f, 0xff, 0xff, 0xf7, 0xf7, 0xa0, 0x1f, 0xcf, 0xe7, 0xff, 0xff, 0xf7, 0xe3, 0xa0, 
  0x97, 0xff, 0xff, 0xff, 0xef, 0xbb, 0x68, 0x3f, 0xf8, 0x3f, 0xdf, 0xcf, 0xff, 0xf7, 0x8d, 0xf0, 
  0xbf, 0xf8, 0xbf, 0xff, 0xe1, 0xff, 0xfb, 0xff, 0xff, 0xbd, 0xff, 0x4f, 0xff, 0xfa, 0x1f, 0xd0, 
  0xbe, 0x00, 0xff, 0xfe, 0x42, 0xff, 0xb7, 0xff, 0xff, 0xcb, 0xfe, 0x84, 0xff, 0xfb, 0xff, 0xd0, 
  0xae, 0x2f, 0x7f, 0xff, 0x07, 0xf8, 0x1f, 0xfe, 0xff, 0xf0, 0x3f, 0x41, 0xff, 0xfb, 0x83, 0xd0, 
  0x2f, 0xce, 0x7f, 0xff, 0x0f, 0xfb, 0xff, 0xfe, 0x7f, 0xff, 0x3f, 0xa1, 0xff, 0xfa, 0x01, 0xd0, 
  0x2f, 0xf9, 0x7f, 0xf9, 0x9b, 0xe0, 0x7f, 0xfa, 0x3f, 0xf8, 0x0f, 0xf0, 0x1f, 0xfa, 0xfe, 0xd0, 
  0x2f, 0xe3, 0x7f, 0xfc, 0x17, 0xe7, 0x7f, 0xfe, 0x1f, 0xfd, 0x8f, 0xd0, 0x7f, 0xfb, 0x0c, 0xd0, 
  0x2e, 0xce, 0x7f, 0xf0, 0x17, 0xe0, 0xfa, 0x02, 0x3c, 0x3c, 0x07, 0xd0, 0x0f, 0xfb, 0xef, 0xd0, 
  0xae, 0xce, 0xff, 0xfc, 0x17, 0xcf, 0xfe, 0xf0, 0x80, 0xfc, 0xc7, 0xd0, 0x7f, 0xfa, 0xff, 0xd0, 
  0xae, 0xcc, 0xbf, 0xfe, 0x17, 0xe7, 0xff, 0xb0, 0xe7, 0xfc, 0x07, 0xd0, 0x5f, 0xf8, 0x1f, 0xd0, 
  0xbf, 0x1f, 0xbf, 0xf0, 0x17, 0xf0, 0x7f, 0xc2, 0x37, 0xfd, 0x1f, 0xd0, 0x1f, 0xfd, 0x81, 0xd0, 
  0x97, 0xff, 0xff, 0xfe, 0x1b, 0xe0, 0x7f, 0x9f, 0x0b, 0xfc, 0x0f, 0xe0, 0xff, 0xf4, 0x0f, 0xb0, 
  0xd3, 0xff, 0xdf, 0xff, 0x0f, 0xf8, 0x3f, 0xbf, 0xe1, 0xfa, 0x7f, 0xa0, 0xff, 0xf9, 0xf3, 0xa0, 
  0xdb, 0xff, 0x2f, 0xf8, 0x0d, 0xf8, 0x1f, 0xff, 0xff, 0xf0, 0x3f, 0xc0, 0xbf, 0xcf, 0xfb, 0x60, 
  0xcd, 0xfe, 0x37, 0xfc, 0x02, 0xff, 0x0f, 0xff, 0xff, 0xe1, 0xfe, 0xc7, 0x7f, 0xf0, 0x3f, 0xc0, 
  0xed, 0xf8, 0xff, 0xff, 0xdd, 0x7f, 0x7b, 0xff, 0xff, 0xbf, 0xfd, 0x37, 0xff, 0xa6, 0x1e, 0xd0, 
  0xe7, 0xe1, 0x7b, 0xff, 0x83, 0xdf, 0x7d, 0xff, 0xff, 0x6d, 0xf7, 0xc3, 0xff, 0x4f, 0xdf, 0x90, 
  0xf3, 0x7f, 0xfe, 0xff, 0xff, 0x2f, 0xff, 0xe0, 0x1f, 0xbf, 0xe3, 0xff, 0xfc, 0xc1, 0x9f, 0x30, 
  0xf9, 0x3f, 0xf1, 0x7f, 0xff, 0xf0, 0x0f, 0xf0, 0x1f, 0xe0, 0x5f, 0xff, 0xff, 0x78, 0x72, 0x70, 
  0xfc, 0xff, 0xfe, 0xbf, 0xff, 0xdf, 0xd8, 0x02, 0xc1, 0x77, 0xff, 0xff, 0xf4, 0x3f, 0xfe, 0xf0, 
  0xfe, 0x7f, 0xfe, 0x2f, 0xff, 0xdf, 0x3d, 0x3e, 0x71, 0x50, 0xb7, 0xff, 0xeb, 0x87, 0xf9, 0xf0, 
  0xff, 0x2f, 0xf0, 0xff, 0xff, 0xff, 0xbf, 0x3b, 0xa9, 0xeb, 0xff, 0xff, 0xb0, 0x31, 0xd3, 0xf0, 
  0xff, 0xdb, 0xcf, 0xfe, 0xff, 0xff, 0xee, 0x7f, 0xf8, 0xef, 0xff, 0xfd, 0xff, 0xdf, 0x6f, 0xf0, 
  0xff, 0xef, 0xef, 0xff, 0xbf, 0xff, 0xf6, 0x03, 0xc0, 0xdf, 0xff, 0xf3, 0xff, 0xdf, 0xdf, 0xf0, 
  0xff, 0xf3, 0x7f, 0xff, 0xef, 0xff, 0xff, 0x9c, 0x73, 0xbf, 0xff, 0xdf, 0xff, 0xfb, 0x3f, 0xf0, 
  0xff, 0xf9, 0xbf, 0xff, 0xfd, 0xff, 0xfe, 0x73, 0xfe, 0xff, 0xfe, 0x7f, 0xff, 0xf6, 0x7f, 0xf0, 
  0xff, 0xfe, 0x6f, 0xff, 0x7f, 0x9f, 0xff, 0xff, 0x7f, 0xff, 0xe7, 0xff, 0xff, 0xd9, 0xff, 0xf0, 
  0xff, 0xff, 0x9b, 0xfc, 0x1f, 0xfc, 0x7f, 0xff, 0xff, 0xfc, 0x7f, 0xb5, 0xff, 0x67, 0xff, 0xf0, 
  0xff, 0xff, 0xe7, 0x7d, 0x4f, 0xff, 0xfc, 0x0f, 0xe0, 0x7f, 0xff, 0xe7, 0xfb, 0x9f, 0xff, 0xf0, 
  0xff, 0xff, 0xf9, 0x9f, 0x7f, 0xff, 0x8e, 0x1f, 0x0e, 0x07, 0xff, 0xbd, 0xee, 0x7f, 0xff, 0xf0, 
  0xff, 0xff, 0xfc, 0x67, 0xff, 0xff, 0x9c, 0xe6, 0x64, 0xe3, 0xff, 0xff, 0x99, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0x9d, 0xff, 0xff, 0x3c, 0xe0, 0xff, 0x83, 0xff, 0xfc, 0xe7, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xe3, 0x3f, 0xff, 0x3c, 0x40, 0x63, 0xf1, 0xff, 0xe7, 0x1f, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xfe, 0x1c, 0xfe, 0x39, 0xe4, 0xf2, 0x73, 0xfc, 0xe1, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xc3, 0xcf, 0xfc, 0x1e, 0x07, 0xff, 0x8f, 0x1f, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x0f, 0x83, 0xff, 0xff, 0x07, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf0
};

 // 'Untitled-2', 125x64px
const unsigned char MarilynMonroe [] PROGMEM = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7d, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc1, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x81, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x81, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xe0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0xc1, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x03, 0x81, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x0f, 0x81, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x0f, 0x81, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x1f, 0x01, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x3c, 0x01, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0xf9, 0xe2, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x42, 0x09, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x42, 0x09, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x42, 0x08, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x40, 0x18, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x41, 0xe0, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};


void setup()
{
  pinMode(buz,OUTPUT);
pinMode(led,OUTPUT);
pinMode(ledst,OUTPUT);

  pinMode(rain, INPUT);
  pinMode(Gas, INPUT);
  pinMode(soil, INPUT);

  Blynk.begin(auth, ssid, pass);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  // Clear the buffer.
  display.clearDisplay();

  // Display bitmap
  display.drawBitmap(0, 0,  MarilynMonroe, 125, 64, WHITE);
  display.display();
delay(5000);
                                                              
  display.clearDisplay();
  display.drawBitmap(0, 0,    myBitmap, 125, 64, WHITE);
  display.display();
delay(5000);
                                                                 
  // Clear the buffer.
  display.clearDisplay();
  // Invert Display
  //display.invertDisplay(1);
  // Changing Font Size
display.clearDisplay();
display.setTextColor(WHITE);
display.setCursor(0,0);
display.setTextSize(2);
display.println("START");
display.display();
delay(2000);
 display.clearDisplay();
}
void loop() {
  if (Blynk.connected()) {
     Blynk.run();  
    display.setTextColor(WHITE);
display.setCursor(0,0);
display.setTextSize(2);
display.println("ONLINE");
display.display();
delay(2000);
 display.clearDisplay();
    digitalWrite(ledst,HIGH);
     digitalWrite(led,LOW);
   
 led5.on();
  }
  else{
    display.setTextColor(WHITE);
display.setCursor(0,0);
display.setTextSize(2);
display.println("OFFLINE");
display.display();
delay(2000);
 display.clearDisplay();
    digitalWrite(ledst,LOW);
    digitalWrite(led,HIGH);
    led5.off();
  }
 gasDetect();
  }
void gasDetect(void) {
  if (analogRead(Gas)>100){
    led4.on();
    delay(200);
    state = 2;
    Serial.println("GAS, GAS, GAS");      
  }
  else{
    led4.off();
  }
}

    void soilrain(void){
      if((digitalRead(soil)==HIGH&&digitalRead(rain)==HIGH)){
      led7.on();
      led3.off();
      led6.off();
      
    }
    else if(( digitalRead(soil)==HIGH&&digitalRead(rain)==LOW)){
       led6.on();
       led3.off();
       led7.off();
    }
    else  if((digitalRead(soil)==LOW&&digitalRead(rain)==HIGH)){
      led3.on();
      led6.off();
      led7.off();
    }
      
    }
    
