#include <LiquidCrystal_I2C.h>   // from https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
#include <Wire.h>                // Standard Arduino Library
#include <Adafruit_ADS1015.h>    // Must use the soligen2010 fork at https://github.com/soligen2010/Adafruit_ADS1X15
#include <PWM.h>                 // From https://code.google.com/archive/p/arduino-pwm-frequency-library/downloads
#include "PowerSupplyMonitorCalibration.h"

#define EN_PIN     2
#define RW_PIN     1
#define RS_PIN     0
#define D4_PIN     4
#define D5_PIN     5
#define D6_PIN     6
#define D7_PIN     7
#define BL_PIN     3   //Backlight

LiquidCrystal_I2C lcd(0x27,   // Set the LCD I2C address
                      EN_PIN,
                      RW_PIN,
                      RS_PIN,
                      D4_PIN,
                      D5_PIN,
                      D6_PIN,
                      D7_PIN,
                      BL_PIN,
                      POSITIVE);  


Adafruit_ADS1115 ads;
float adsMVperStep;

int fanSpeed = 210;

void setup() {
  bool success = SetPinFrequencySafe(9,25000); // set fan PWM frequency to 25 kHz
  
  //Serial.begin(115200);
  
  setupADC();
  lcd.begin(16, 2);

  lcd.clear();
} 

void loop() {   
  int a;
  int overSampleCount = 16;   // number of over-samples to take before updating display
    
  int16_t voltReading = 0; 
  int16_t ampReading = 0;  
  int16_t ampSetReading = 0;  
  
  int32_t voltOverSample = 0; 
  int32_t ampOverSample = 0; 
  int32_t ampSetOverSample = 0; 

  float voltIn = 0.0;
  float ampIn = 0.0;
  float ampSetIn = 0.0;
  float watts = 0.0;

    // display only over-sampled readings
    // this reduces display jitter

    // AIN0 - voltage
    // AIN1 - Before shunt resistor
    // AIN2 - amp current limit set
    // AIN3 - GND (After shunt resistor)

  //Take readings all at once so ADC does not have to switch inputs with each reading  
  //Also take a priming read so inputs settle
  voltReading = ads.readADC_Differential_0_3();
  for(a = 0; a < overSampleCount; a = a + 1) { 
    voltReading = ads.readADC_Differential_0_3();
    voltOverSample = voltOverSample + voltReading;
  }  
  ampReading = ads.readADC_Differential_1_3();
  for(a = 0; a < overSampleCount; a = a + 1) { 
    ampReading = ads.readADC_Differential_1_3();
    ampOverSample = ampOverSample + ampReading;
  }
  ampSetReading = ads.readADC_Differential_2_3();    
  for(a = 0; a < overSampleCount; a = a + 1) { 
    ampSetReading = ads.readADC_Differential_2_3();    
    ampSetOverSample = ampSetOverSample + ampSetReading;
  }  
  
  voltIn =  (((((float) voltOverSample) / (float) a) * adsMVperStep)  * VOLT_SET_MULTIPLIER) + VOLT_SET_OFFSET;
  voltIn -= ((((float) ampOverSample) / (float) a * adsMVperStep));    // Adjust by the voltage drop accross the shunt resistor
  
  ampIn = (((((float) ampOverSample) / (float) a * adsMVperStep)) / SHUNT_RESISTOR) + SHUNT_OFFSET;
  
  ampSetIn = (((((float) ampSetOverSample) / (float) a * adsMVperStep)) * AMP_SET_ADJUST) + AMP_SET_OFFSET;

  //Amp set changes slightly per amp actually being drawn so add to the display based on this
  ampSetIn = ampSetIn  + (ampIn * AMP_SET_LOAD_COMPENSATION);

  // Avoid display jitter when there is nothng connected
  if (ampSetIn < 0.0007) {ampSetIn = 0.0;}
  if (ampIn < 0.0007) {ampIn = 0.0;}
  if (voltIn < 0.008) {voltIn = 0.0;}
  
  watts = voltIn * ampIn;
  
  adjustFanSpeed(watts, ampIn);
    
  display(voltIn, ampIn, ampSetIn, watts);
}


void display(float volt, float amp, float ampSet, float watt) {

  if (true) {
    lcd.setCursor(0, 1);lcd.print(" I=");
    lcd.setCursor(3, 1);lcd.print(amp, 3);
  }
  if (true) {
    lcd.setCursor(0, 0);lcd.print("CL=");
    lcd.setCursor(3, 0);lcd.print(ampSet, 3);
  }
  if (true) {
    lcd.setCursor(8, 1);lcd.print(" W=");
    if (watt > 10.0) {
      lcd.setCursor(11, 1);lcd.print(watt, 2);
    }
    else {
        lcd.setCursor(11, 1);lcd.print(" ");
        lcd.setCursor(12, 1);lcd.print(watt, 2);
    }
    
    lcd.setCursor(8, 0);lcd.print(" V=");
    float voltDisplay = volt;
    if (voltDisplay > 10.0) {
      lcd.setCursor(11, 0);lcd.print(voltDisplay, 2);
    }
    else {
        lcd.setCursor(11, 0);lcd.print(" ");
        lcd.setCursor(12, 0);lcd.print(voltDisplay, 2);
    }
  }
}

void setupADC() {
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                           ADS1015  ADS1115
  //                                                                           -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);             // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);                   // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);                   // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);                  // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);                 // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);               // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV  
  
  ads.begin();
  ads.setGain(GAIN_TWO);
  adsMVperStep = ads.voltsPerBit();
  ads.setSPS(ADS1115_DR_250SPS);      // 860 Samples per second

}

void adjustFanSpeed (float watts, float amps) {
  int newFanSpeed;
  #define FAN_IDLE_SPEED  155
  #define FAN_MAX_SPEED   255
   
    // scale fan speed based on how much heat is generated.  The transistor's dsipation is based on watts, but the shunt
    // resistor disipation is based on the amps.  Choose fan speed based on whichever is higher
    newFanSpeed =  max (FAN_IDLE_SPEED + (float ((watts/MAX_WATTS) * (float (FAN_MAX_SPEED - FAN_IDLE_SPEED))))  ,
                        FAN_IDLE_SPEED + (float ((amps/MAX_AMPS)   * (float (FAN_MAX_SPEED - FAN_IDLE_SPEED))))  );
  if (newFanSpeed != fanSpeed) {
    fanSpeed = newFanSpeed;
    pwmWrite(9,fanSpeed);  
  }
}

