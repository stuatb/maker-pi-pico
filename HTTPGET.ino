/****************************************************************************************************************************
  HTTPGET.ino
  For ESP8266/ESP32-AT-command running shields
  
  ESP_AT_Lib is a wrapper library for the ESP8266/ESP32 AT-command shields
  
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_Lib
  Licensed under MIT license
  
  @example ConnectWiFi.ino
  @brief The ConnectWiFi demo of library WeeESP8266.
  @author Wu Pengfei<pengfei.wu@itead.cc>
  @date 2015.03
  
  @par Copyright:
  Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version. \n\n
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  
  Version: 1.4.0
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for ESP8266/ESP32-AT shields to support Mega, nRF52, SAMD, DUE, STM32, etc.
  1.1.0   K Hoang      10/05/2021 Add support to BOARD_SIPEED_MAIX_DUINO and RASPBERRY_PI_PICO
  1.2.0   K Hoang      17/05/2021 Add support to RP2040-based boards using Arduino-mbed RP2040 core. Fix compiler warnings
  1.3.0   K Hoang      29/05/2021 Add support to RP2040-based Nano_RP2040_Connect using Arduino-mbed RP2040 core
  1.4.0   K Hoang      13/08/2021 Add support to Adafruit nRF52 core v0.22.0+
 *****************************************************************************************************************************/

/* Comment this out to disable prints and save space */
#define ESP_AT_DEBUG_OUTPUT     Serial

#define _ESP_AT_LOGLEVEL_       4

// Uncomment to use ESP32-AT commands
//#define USE_ESP32_AT      true

#define SERIAL_TX_BUFFER_SIZE
#define SERIAL_RX_BUFFER_SIZE

#define BOARD_TYPE  "RASPBERRY_PI_PICO"
#define EspSerial Serial1   

#include "ESP_AT_Lib.h"

#define SSID        "SSID"
#define PASSWORD    "PASSWORD"

// Your board <-> ESP_AT baud rate:
#define ESP_AT_BAUD       115200

ESP8266 wifi(&EspSerial);

uint8_t buffer[4096] = {0};

#define HOST_NAME   "arduino.cc"
#define HOST_PORT   (80)

void setup(void)
{ Serial.begin(115200);
  while (!Serial);
  delay(1000);
  Serial.println("\nStart ConnectWiFi on " + String(BOARD_NAME));
  Serial.println(ESP_AT_LIB_VERSION);
  // initialize serial for ESP module
  EspSerial.setPinout(16,17);
  EspSerial.begin(ESP_AT_BAUD);
  Serial.printf("FW Version: %s\n",wifi.getVersion().c_str());
  if(wifi.setOprToStation()) { Serial.println("Set STA Mode OK"); } 
  else { Serial.println("Set STA Mode failed"); }
  if(wifi.joinAP(SSID, PASSWORD)) Serial.printf("Connect to WiFi (%s) OK\nIP: %s",SSID,wifi.getLocalIP().c_str()); 
  else Serial.printf("Connect to WiFi (%s) failed\n",SSID);
  Serial.print("disableMUX ");
  if(wifi.disableMUX()) { Serial.println("OK"); } 
  else { Serial.println("failed"); }
  Serial.println("Done");
}

void loop(void)
{ uint32_t len;
  
  Serial.print("Create TCP ");
  if(wifi.createTCP(HOST_NAME, HOST_PORT)) Serial.println("OK");  
  else Serial.println("failed");
  char hello[]="GET /asciilogo.txt HTTP/1.1\r\nHost: arduino.cc\r\nConnection: close\r\n\r\n";
  wifi.send((const uint8_t*)hello, strlen(hello));
  len=wifi.recv(buffer,sizeof(buffer),10000);
  Serial.printf("Buffer Len: %d\n",len);
  Serial.println("=========================Received============================");
  if(len > 0) for (uint32_t i = 0; i < len; i++) Serial.print((char) buffer[i]); 
  Serial.println("\n============================================================");
  if (wifi.releaseTCP()) Serial.println("Release TCP OK");
  delay(10000);
}
