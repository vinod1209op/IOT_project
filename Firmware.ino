#include "BTHC05.h"
#include "DHT.h"
#include "pulse-sensor-arduino.h"
#include <ADXL335.h>
#include <ThingSpeak.h>

#define ADXL335_PIN_XOUT	A3
#define ADXL335_PIN_YOUT	A1
#define ADXL335_PIN_ZOUT	A5
#define BTHC05_PIN_TXD	11
#define BTHC05_PIN_RXD	10
#define DHT_PIN_DATA	2
#define HEARTPULSE_PIN_SIG	A4
#define HEARTRATE_PIN_SIG	A0
#define MQ3_PIN_AOUT	A2

BTHC05 bthc05(BTHC05_PIN_RXD, BTHC05_PIN_TXD);
DHT dht(DHT_PIN_DATA);
PulseSensor heartpulse;

const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;
float sensorValue;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup()
{
  // Setup Serial which is useful for debugging
  Serial.begin(9600);
  while (!Serial) ; // wait for serial port to connect. Needed for native USB
  Serial.println("start");

  bthc05.begin(9600);
  bthc05.println("Bluetooth On....");
  dht.begin();
  heartpulse.begin(HEARTPULSE_PIN_SIG);
  menuOption = menu();
  pinMode(7, INPUT); // Setup for leads off detection LO +
  pinMode(6, INPUT); // Setup for leads off detection LO -
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop()
{


  if (menuOption == '1')
  {
    int x_adc_value, y_adc_value, z_adc_value;
    x_adc_value = analogRead(ADXL335_PIN_XOUT); /* Digital value of voltage on x_out pin */
    y_adc_value = analogRead(ADXL335_PIN_YOUT); /* Digital value of voltage on y_out pin */
    z_adc_value = analogRead(ADXL335_PIN_ZOUT); /* Digital value of voltage on z_out pin */
    Serial.print("x = ");
    Serial.print(x_adc_value - 332);
    Serial.print("\t\t");
    Serial.print("y = ");
    Serial.print(y_adc_value - 320);
    Serial.print("\t\t");
    Serial.print("z = ");
    Serial.print(z_adc_value - 263);
    Serial.print("\t\n");

    delay(1000);
  }
  else if (menuOption == '2') {
    String bthc05Str = "FWQAF6Z6XYB6Q0ZU";
    //Receive String from bluetooth device
    if (bthc05.available())
    {
      //Read a complete line from bluetooth terminal
      bthc05Str = bthc05.readStringUntil('\n');
      // Print raw data to serial monitor
      Serial.print("BT Raw Data: ");
      Serial.println(bthc05Str);
    }
    //Send sensor data to Bluetooth device
    bthc05.println("PUT YOUR SENSOR DATA HERE");
  }
  else if (menuOption == '3') {
    float dhtTempC = dht.readTempC();
    Serial.print(F("Temp: ")); 
    Serial.print(dhtTempC/20); 
    Serial.println(F(" [C]"));

  }
  else if (menuOption == '4') {
    int heartpulseBPM = heartpulse.BPM;
    Serial.println(heartpulseBPM);
    if (heartpulse.QS == true) {
      Serial.println("PULSE");
      heartpulse.QS = false;
    }
  }
  else if (menuOption == '5')
  {
    if ((digitalRead(7) == 1) || (digitalRead(6) == 1)) {
      Serial.println('!');
    }
    else {
      Serial.println(analogRead(A0));
    }
    delay(1);
  }
  else if (menuOption == '6')
  {
    sensorValue = analogRead(MQ3_PIN_AOUT); // read analog input pin 0    
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);  
    delay(2000);
  }

  if (millis() - time0 > timeout)
  {
    menuOption = menu();
  }

}



// Menu function for selecting the components to be tested
char menu()
{

  Serial.println(F("\nWhich component would you like to test?"));
  Serial.println(F("(1) ADXL335 Accelerometer"));
  Serial.println(F("(2) HC - 05 Bluetooth Serial Module"));
  Serial.println(F("(3) DHT11 Temperature Sensor"));
  Serial.println(F("(4) Heart Rate Pulse Sensor"));
  Serial.println(F("(5) ECG Sensor"));
  Serial.println(F("(6) Alcohol Gas Sensor - MQ-3"));
  Serial.println(F("(menu) send anything else or press on board reset button\n"));
  while (!Serial.available());

  // Read data from serial monitor if received
  while (Serial.available())
  {
    char c = Serial.read();
    if (isAlphaNumeric(c))
    {

      if (c == '1')
        Serial.println(F("Now Testing ADXL335 - Triple Axis Accelerometer"));
      else if (c == '2')
        Serial.println(F("Now Testing HC - 05 Bluetooth Serial Module"));
      else if (c == '3')
        Serial.println(F("Now Testing DHT11 Temperature Sensor"));
      else if (c == '4')
        Serial.println(F("Now Testing Heart Rate Pulse Sensor"));
      else if (c == '5')
        Serial.println(F("Now Testing ECG sensor"));
      else if (c == '6')
        Serial.println(F("Now Testing Alcohol Gas Sensor - MQ-3"));
      else
      {
        Serial.println(F("illegal input!"));
        return 0;
      }
      time0 = millis();
      return c;
    }
  }
}
