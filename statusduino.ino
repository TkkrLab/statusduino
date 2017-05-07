#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

volatile byte state = LOW;

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0x08, 0xED };
IPAddress server(10, 42, 1, 2);

EthernetClient ethClient;
PubSubClient client(ethClient);

void printIP()
{
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++)
  {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
}

void editSwitchState()
{
  state = !digitalRead(2);
  digitalWrite(9, state);
  sendMQTT();
}

void sendMQTT()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("statusduino")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
  if (state)
    client.publish("tkkrlab/statusduino/spaceknop","1",true);
  else
    client.publish("tkkrlab/statusduino/spaceknop","0",true);
}

void setup()
{
  pinMode(2,INPUT_PULLUP);
  pinMode(9, OUTPUT);
  Serial.begin(57600);
  client.setServer(server, 1883);
  Ethernet.begin(mac);
  // Allow the hardware to sort itself out
  delay(1500);
  printIP();
  attachInterrupt(0, editSwitchState, CHANGE);
  editSwitchState();
}

void loop()
{
}
