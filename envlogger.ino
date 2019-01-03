// This #include statement was automatically added by the Particle IDE.
#include <SparkFunRHT03.h>


/////////////////////
// Pin Definitions //
/////////////////////
const int RHT03_DATA_PIN = D3; // RHT03 data pin
const int LIGHT_PIN = A0; // Photocell analog output
const int LED_PIN = D7; // LED to show when the sensor's are being read --> D7 is the Photon onboard led, no need for external

///////////////////////////
// RHT03 Object Creation //
///////////////////////////
RHT03 rht; // This creates a RTH03 object, which we'll use to interact with the sensor

unsigned int light;
char lightString[4];

float tempC;
char tempString[4];

float humidity;
char humidityString[4];

#define PRINT_RATE 30000 // Time in ms to delay between publish.

void setup() 
{
    
    // Using the 'rht' object created in the global section, we'll begin by calling
    // its member function `begin`.
    // The parameter in this function is the DIGITAL PIN we're using to communicate
    // with the sensor.
    rht.begin(RHT03_DATA_PIN);  // Initialize the RHT03 sensor
    
    // Don't forget to set the pin modes of our analog sensor (INPUT) and the LED (OUTPUT):
    pinMode(LIGHT_PIN, INPUT); // Set the photocell pin as an INPUT.
    pinMode(LED_PIN, OUTPUT); // Set the LED pin as an OUTPUT
    digitalWrite(LED_PIN, LOW); // Initially set the LED pin low -- turn the LED off.
}

void loop() 
{
    digitalWrite(LED_PIN, HIGH); // Turn the LED on -- it'll blink whenever the sensor is being read.

    // Use the RHT03 member function `update()` to read new humidity and temperature values from the sensor.
    // There's a chance the reading might fail, so `update()` returns a success indicator. It'll return 1
    // if the update is successful, or a negative number if it fails.
    int update = rht.update();
    
    if (update == 1) // If the update succeeded, print out the new readings:
    {
        // Use analogRead to get the latest light sensor reading:
        light = analogRead(LIGHT_PIN);
        
        sprintf(lightString, "%d", light);
        Particle.publish("Light", lightString);
        
        // The `humidity()` RHT03 member function returns the last successfully read relative
        // humidity value from the RHT03.
        // It'll return a float value -- a percentage of RH between 0-100.
        // ONLY CALL THIS FUNCTION AFTER SUCCESSFULLY RUNNING rht.update()!.
        humidity = rht.humidity();
                
        // Publish the relative humidity:

        sprintf(humidityString, "%f", humidity, 1);
        Particle.publish("Humidity", humidityString);
        
        // The `tempC()` RHT03 member function returns the last succesfully read 
        // celsius temperature value from the RHT03.
        // It returns a float variable equal to the temperature in Celsius.

        // ONLY CALL THIS FUNCTION AFTER SUCCESSFULLY RUNNING rht.update()!.
        tempC = rht.tempC();
        
        // Publish the temperature in °C:
        sprintf(tempString, "%f", tempC, 1);
        Particle.publish("Temp", tempString);
        
    }
    else // If the update failed, give the sensor time to reset:
    {
        Particle.publish("Failed to read from RHT03");
        
        delay(RHT_READ_INTERVAL_MS); // The RHT03 needs about 1s between read attempts
    }
    digitalWrite(LED_PIN, LOW); // Turn the LED off
    
    delay(PRINT_RATE); // delay for 1s, printing too much will make the output very hard to read.
}

