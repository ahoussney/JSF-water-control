String pumpStatus = "off";
String eventPrefix = "jsf/waterSystem";
int relayPin = 0;

void setup() {
    Particle.variable("pumpStatus", pumpStatus);
    Particle.subscribe(eventPrefix, eventHandler);
    pinMode(relayPin, OUTPUT);
}

void loop() {
}

void eventHandler(String event, String data)
{
    if (data == "turn on pump") {
        digitalWrite(relayPin, 1);
        pumpStatus = "on";
    } else if (data == "turn off pump") {
        digitalWrite(relayPin, 0);
        pumpStatus = "off";
    }
}
