#include <HamShield.h>
#include "PWM.hpp"

int pwms[] = {25, 75, 125, 175, 225};

const int pwm_out = 11; // Pin to send generated PWM signal, DELETE WHEN ANOTHER SOURCE IS PROVIDED
unsigned int pwm_signal = 0; // variable used to save pwm values
unsigned long current_frequency = 159000; // Initial and current frequency
unsigned long new_frequency;
// ncs_pin = A1;
// clk_pin = A5;
// dat_pin = A4;
// mic_pin = 3;
//spkr_pin = A2;
//VCC = 5V
// Do not use the above pins for anything else unless the default pins are changed in radio()


HamShield radio; // Creates the HamShield object from the hamshield library using the default pins
PWM pwm_in(2); // Uses the PWM library

void setup() {
  pwm_in.begin(true); // Initialize pin digital pin 2 to detect HIGH signals, MUST BE ABLE TO SUPPORT AN INTERRUPT
  pinMode(pwm_out, OUTPUT); // DELETE WHEN A NEW SOURCE IS PROVIDED
  digitalWrite(pwm_out, LOW); // DELETE WHEN A NEW SOURCE IS PROVIDED
  Serial.begin(9600);

  int result = radio.testConnection(); // Can be used to check radio status
  radio.initialize(); // Boot radio
  radio.setRfPower(0); // TX is not needed, turn transmit power to 0
  radio.setModeOff(); // Turns TX and RX off
  delay(500); // Give system time to power down

  radio.setModeReceive(); // Turns on RX
  radio.frequency(159000); // default station (kHz)
  // Frequencies available (MHz):
  // 134 - 174
  // 200 - 260
  // 400 - 520
}



void loop() {

  for (int current_pwm = 0; current_pwm < 5; current_pwm++) {
    analogWrite(pwm_out, pwms[current_pwm]);
    new_frequency = pwm_to_frequency(pwm_in.getValue());
    if (true) { // condition used to initiate signal switch
      radio.frequency(new_frequency);
      current_frequency = new_frequency;
      Serial.println("New frequency: " + String(radio.getFrequency()));
    }
    delay(1000);
  }

}



unsigned long pwm_to_frequency(int pwm_raw) { // Helper function to translate input signal to a desired frequency
  float pwm = pwm_raw/4; //Helps translate into analogWrite() numbers
  unsigned long frequencies[] = {159000, 174000, 25000, 400000, 520000}; // Frequencies to be used
  Serial.println(String(pwm));
  if (pwm < 50) {
    return frequencies[0];
  } else if (pwm >= 50 and pwm < 100) {
    return frequencies[1];
  } else if (pwm >= 100 and pwm < 150) {
    return frequencies[2];
  } else if (pwm >= 150 and pwm < 200) {
    return frequencies[3];
  } else {
    return frequencies[4];
  }
}
