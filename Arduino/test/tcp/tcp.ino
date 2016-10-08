#include <UIPEthernet.h> // Used for Ethernet

#define IT_TRIGER_DELAY_UUS 10

#define IT_PIN 2

void triger_it(int it_pin);

byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 };
IPAddress ip(192, 168, 1, 179);
EthernetServer server(80);

void setup() {
  pinMode(IT_PIN, OUTPUT);
  Serial.begin(115200);
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client)
  {
    while (client.connected())
    {
      if (client.available())
      {
        uint8_t rx_code = client.read();
        triger_it(IT_PIN);
        Serial.write(rx_code);
      }
      if (Serial.available())
      {
        client.write(Serial.read());
      }
    }
    delay(10);
    client.stop();
  }
}

void triger_it(int it_pin)
{
  digitalWrite(it_pin, HIGH);
  delayMicroseconds(IT_TRIGER_DELAY_UUS);
  digitalWrite(it_pin, LOW);
  delayMicroseconds(IT_TRIGER_DELAY_UUS);
}

