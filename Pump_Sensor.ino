//This controls and reads the water sensor attached to a photon in the cellar at JSF
    //read the raw pressure sensor reading here:
        // https://api.particle.io/v1/devices/280037000a47353138383138/pressure?access_token=d9b7e4e94f6d0a743b57aff6d4d7dd3a6bdf4739&format=raw
        // https://api.particle.io/v1/devices/280037000a47353138383138/pressure?access_token=d9b7e4e94f6d0a743b57aff6d4d7dd3a6bdf4739
    // See a chart of pressure here:
        // https://thingspeak.com/channels/149039
    //Gallons reading here:
        // https://api.particle.io/v1/devices/280037000a47353138383138/gallons?access_token=d9b7e4e94f6d0a743b57aff6d4d7dd3a6bdf4739&format=raw
    

double pressure;
double gallons;

bool pumpOn; //domestic water pressure pump status
int avgCounts = 1000;
unsigned long measureInterval = 10000; // measure pressure every 10s
unsigned long lastMeasuredAt = 0; // for keeping track of last pressure measurement time
int pressurePin = A0;

int deltaThreshold = 70; //we discard pressure readings who differ from the previous reading by more this value
int empty = 350; //the pressure sensor reading when JSF water system is empty
int one = 1750; //reading when pipes are full but tanks are empty before one tank begins to fill
int two = 1850; //reading when second tank begins to fill
int three = 2400; //reading when second tank is full and first tank is still filling
int full = 2401; //reading when both tanks are full (adjust upward when plumbing is done

float emptyRate = 0.1429; // between 'empty' and 'one' for every point of sensor pressure the system gains 0.14+ gallons
float oneRate = 3.27; // fill rate (gallons per unit pressure) between 'one' and 'two'
float twoRate = 6.27; // fill rate between 'two' and 'three'
float threeRate = 3.27; //fill rate between 'three' and 'full'

double reading;
double previousReading;
double oldestReading;

String eventPrefix = "jsf/waterSystem/";

bool withinThreshold(int one, int two) {
   return abs(one - two) < deltaThreshold;
}

double readAveragePressure() {
   double average = 0;
   for (int i = 0; i < avgCounts; i++) {
       average += analogRead(pressurePin);
   }
   return average / avgCounts;
}

double pressureToGallons(double pressure) {
    double gallons = 0;
    if (pressure < one) {
        return (pressure - empty) * emptyRate;
    } else {
        gallons = (one - empty) * emptyRate;
    }
    
    if (pressure < two) {
        return gallons + ((pressure - one) * oneRate);
    } else {
        gallons = gallons + ((two - one) * oneRate);
    }
    
    if (pressure < three) {
        return gallons + ((pressure - two) * twoRate);
    } else {
        gallons = gallons + ((three - two) * twoRate);
    }
    
    if (pressure < full) {
        return gallons + ((pressure - three) * threeRate);
    } else {
        gallons = 4400;
    }
    return gallons;
}

void setup() {
   pinMode(pressurePin, INPUT);
   Particle.variable("gallons", gallons);
   Particle.variable("pressure", pressure);
//   Particle.variable("housePumpOn", pumpOn);
//   Particle.variable("recordPump", recordPump);
}

void loop() {
 if ((millis()) - lastMeasuredAt > measureInterval) {
    measurePressure();
//  pressureToGallons();
    lastMeasuredAt = millis(); 
//millis Returns the number of milliseconds (as unsigned long) since the device began running the current program.
//This number will overflow (go back to zero), after approximately 49 days`so this method needs revision
 }
}

void measurePressure() {
   oldestReading = previousReading;
   previousReading = reading;
   reading = readAveragePressure();

   if (reading < empty) {
       pumpOn = true;
   } else {
       pumpOn = false;
       if (withinThreshold(reading, previousReading) && withinThreshold(previousReading, oldestReading)) {
           pressure = reading;
           gallons = pressureToGallons(pressure);
           Particle.publish(eventPrefix + "pressureSensor/pressure", String(pressure));
           Particle.publish(eventPrefix + "pressureSensor/gallons", String(gallons));
       }
   }
}
