// sensor code
float pressure;
int avgCounts = 1000;
unsigned long measureInterval = 10000; // measure pressure every 10s
unsigned long lastMeasure = 0; // for keeping track of last pressure measurement time
int pressurePin = D1;
String eventPrefix = "jsf/waterSystem/";

void setup() {
  pinMode(pressurePin, INPUT);
}

void loop() {
  if ((lastMeasure - millis()) > measureInterval) {
    measure_pressure();
    lastMeasure = millis();
  }
}

void measure_pressure() {
	// subscribe to this from a terminal with the API like:
  // curl https://api.particle.io/v1/devices/events/temp?access_token=1234
  // averages 1000 readings, should only take a few seconds
  for (int i = 0; i < avgCounts, i++){
    pressure += analogRead(pressurePin);
  }
  pressure = pressure / avgCounts;
  // format is sf/wate(event (i.e. label), data), both as strings
  Particle.publish(eventPrefix + "pressureSensor/pressure", String(pressure));
  pressure = 0;
}

// controller code
float pressure;
int relayPin = 0;
String pumpStatus = "off";
String eventPrefix = "jsf/waterSystem/";

void setup() {
  Particle.subscribe(eventPrefix + "pressureSensor/pressure", pressureHandler);
  pinMode(relayPin, OUTPUT);
}
void pressureHandler(String event, String data) {
  if (event == "jsf/waterSystem/pressureSensor/pressure") {
    pressure = toFloat(data);
    doControl(pressure);
  }
}

void doControl(float p) {
  if (p < 1000) {
    digitalWrite(pumpPin, 1);
    pumpStatus = "on";
  } else if (p > 2000) {
    digitalWrite(pumpPin, 0);
    pumpStatus = "off";
  }
}
