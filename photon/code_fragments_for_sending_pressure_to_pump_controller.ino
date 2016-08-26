// sensor code
double pressure;
double avgPress;
int avgCounts = 1000;
unsigned long measureInterval = 10000; // measure pressure every 10s
unsigned long lastMeasure = 0; // for keeping track of last pressure measurement time
int pressurePin = A0;
String eventPrefix = "jsf/waterSystem/";

void setup() {
  pinMode(pressurePin, INPUT);
  Particle.variable("pressure", pressure);
}

void loop() {
  if ((millis()) - lastMeasure > measureInterval) {
    measure_pressure();
    lastMeasure = millis(); //millis Returns the number of milliseconds (as unsigned long) since the device began running the current program. 
//This number will overflow (go back to zero), after approximately 49 days`
  }
}

void measure_pressure() {
// subscribe to this from a terminal with the API like:
// curl https://api.particle.io/v1/devices/events/pressure?access_token=1234
// https://api.particle.io/v1/devices/280037000a47353138383138/pressure?access_token=d9b7e4e94f6d0a743b57aff6d4d7dd3a6bdf4739&format=raw
// averages 1000 readings, should only take a few seconds
  if(analogRead(pressurePin)<350){//this is checking to see if the sensor is reading very low due to the fact that when the pressure pump is on it causes a suction in the system reading in the 5-10 range
      delay(10000); //this is waiting to check again if pressure pump is still on
      }else{
          delay(10000); //this is waiting for pressure to normalize after pump has turned off
          for (int i = 0; i < avgCounts; i++) {
            avgPress += analogRead(pressurePin);
          }    
          pressure = avgPress / avgCounts;
          delay(10000);// waiting to see if pressure pump has turned on before readings were completely recorded 
          // format is sf/wate(event (i.e. label), data), both as strings
          if(analogRead(pressurePin)>350){ //checking to see if pressure pump turned back on before readings were completed
          Particle.publish(eventPrefix + "pressureSensor/pressure", String(pressure));
        }
  avgPress = 0;
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
