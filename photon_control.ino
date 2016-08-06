String pumpStatus = "off";
String eventPrefix = "jsf/waterSystem/";
int relayPin = 0;
int minsOn = 0;
int minsOnLeft = 0;
bool pumpOn = false;
unsigned long startTime = 0;
unsigned long pumpOnTime; // keeps track of when pump was turned on
unsigned long pumpOffTime; // keeps track of when pump was turned off
unsigned long pumpOffTimeSetting; // for setting a time to turn off pump in future

void setup() {
    Particle.variable("pumpStatus", pumpStatus);
    Particle.variable("minsOnLeft", minsOnLeft);
    Particle.subscribe(eventPrefix + "pumpOn", pumpOnHandler);
    Particle.subscribe(eventPrefix + "pumpOff", pumpOffHandler);
    pinMode(relayPin, OUTPUT);
}

void loop() {
  if (pumpOn) {
    minsOnLeft = minsOn - (millis() - startTime)/1000;
    if (minsOnLeft < 0)
    {
      digitalWrite(relayPin, 0);
      pumpStatus = "off";
      pumpOn = false;
    }
  }
}

void pumpOnHandler(String event, String data)
{
    digitalWrite(relayPin, 1);
    pumpStatus = "on";
    pumpOnTime = millis();
    pumpOn = true;
    minsOn = int data;
}

void pumpOffHandler(String event, String data)
{
  digitalWrite(relayPin, 0);
  pumpStatus = "off";
  pumpOn = false;
}
