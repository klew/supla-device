/*
  Copyright (C) AC SOFTWARE SP. Z O.O.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*
 * This example shows how to use Bosh BME280 (temperature, humidity, pressure)
 * sensor with SuplaDevice on Arduino-compatible boards.
 *
 * Dependency: Install "Adafruit BME280 Library" via Library Manager.
 */

#include <SuplaDevice.h>
#include <supla/sensor/BME280.h>

// Choose proper network interface for your board:
#ifdef ARDUINO_ARCH_AVR
  // Arduino Mega with EthernetShield W5100:
  #include <supla/network/ethernet_shield.h>
  // Ethernet MAC address
  uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
  Supla::EthernetShield ethernet(mac);

  // Arduino Mega with ENC28J60:
  // #include <supla/network/ENC28J60.h>
  // Supla::ENC28J60 ethernet(mac);
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
  // ESP8266 and ESP32 based boards:
  #include <supla/network/esp_wifi.h>
  Supla::ESPWifi wifi("your_wifi_ssid", "your_wifi_password");
#endif

// Optional: If you use custom I2C pins, initialize Wire in setup() accordingly.
// Default I2C address for BME280 modules is usually 0x76 or 0x77.
#define BME280_I2C_ADDR 0x77

// Optional: Set your station altitude in meters to get sea level pressure
// correction. If not needed, pass NAN (default).
#define LOCAL_ALTITUDE_M 100.0f

void setup() {
  Serial.begin(115200);

  // Replace the following GUID with value that you can retrieve from:
  // https://www.supla.org/arduino/get-guid
  char GUID[SUPLA_GUID_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

  // Replace the following AUTHKEY with value that you can retrieve from:
  // https://www.supla.org/arduino/get-authkey
  char AUTHKEY[SUPLA_AUTHKEY_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

  /*
   * Having your device already registered at cloud.supla.org, if you change
   * CHANNEL sequence or remove any of them, you must also remove the device
   * itself from cloud.supla.org. Otherwise you will get "Channel conflict!".
   */

  // CHANNEL0 - BME280 temperature/humidity and secondary pressure channel
  auto bme280 = new Supla::Sensor::BME280(BME280_I2C_ADDR, LOCAL_ALTITUDE_M);
  (void)bme280; // silence unused warning if not referenced later

  /*
   * SuplaDevice Initialization.
   * Server address is available at https://cloud.supla.org
   * If you do not have an account, you can create it at
   * https://cloud.supla.org/account/create
   * SUPLA and SUPLA CLOUD are free of charge
   */
  SuplaDevice.begin(GUID,              // Global Unique Identifier
                    "svr1.supla.org",  // SUPLA server address
                    "email@address",   // Email used to login to Supla Cloud
                    AUTHKEY);           // Authorization key
}

void loop() {
  SuplaDevice.iterate();
}
