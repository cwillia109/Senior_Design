#include "PWM.hpp"
#include <HamShield.h>

int pwms[] = {25, 75, 125, 175, 225};

// Initialize a frequency
unsigned long current_frequency = 159000;
// Default pins used by HamShield
    // ncs_pin = A1;
    // clk_pin = A5;
    // dat_pin = A4;
    // mic_pin = 3;
    // spkr_pin = A2;
    // VCC = 5V;


HamShield radio;
PWM pwm_in(2);
const int pwm_out = 11;

void setup()
{
    Serial.begin(9600);

    // Initialize interrupt pin for PWM readings 
    pwm_in.begin(true);
    
    // -------------- Simulated PWM Signal --------------
    pinMode(pwm_out, OUTPUT);
    digitalWrite(pwm_out, LOW);

    // -------------- Radio Setup --------------
    // Can be used to check radio status
    int result = radio.testConnection();
    // Boot radio
    radio.initialize();
    // TX is not needed, turn transmit power to 0              
    radio.setRfPower(0);
    // Turns TX and RX off
    radio.setModeOff();      
    // Give system time to power down
    delay(500);
    // Turns on RX
    radio.setModeReceive();
    // Default station (kHz)
    radio.frequency(current_frequency);
        // Frequencies available (MHz):
        // 134 - 174
        // 200 - 260
        // 400 - 520
}

void loop()
{
    // TODO: Replace for loop with PWM readings from flight controller
    for (int current_pwm = 0; current_pwm < 5; current_pwm++)
    {
        // Simulate a PWM signal
        analogWrite(pwm_out, pwms[current_pwm]);
        // Translate PWM signal to frequency
        current_frequency = pwm_to_frequency(pwm_in.getValue());
        // Signal switch
        radio.frequency(current_frequency);
        Serial.println("Frequency set: " + String(radio.getFrequency()));
        delay(1000);
    }
}


// Helper function
unsigned long pwm_to_frequency(int pwm_raw)
{
    float pwm = pwm_raw / 4;  //Helps translate into analogWrite() numbers
    unsigned long frequencies[] = {159000, 174000, 25000, 400000, 520000};  // Frequencies to be used
    Serial.println(String(pwm));
    if (pwm < 50)
    {
        return frequencies[0];
    }
    else if (pwm >= 50 && pwm < 100)
    {
        return frequencies[1];
    }
    else if (pwm >= 100 && pwm < 150)
    {
        return frequencies[2];
    }
    else if (pwm >= 150 && pwm < 200)
    {
        return frequencies[3];
    }
    else
    {
        return frequencies[4];
    }
}
