// Simple test to run motor forward and backward from 0 to max to 0;

#include <Arduino.h>
#include "BTS7960_HBridge.h"

// Change if you need
// On BTS7960 h-bridge module there are pins R_EN and L_EN - connect them together to pin EN, actually there will be pwm signal
// R_PWM and L_PWM pins on the module won't be driven by pwm
#define EN 23
#define R_PWM 19
#define L_PWM 18

BTS7960_HBridge motor(R_PWM, L_PWM, EN);

void setup() {
    Serial.begin(115200);
    // PWM frequency set to 20kHz and 8 bit resolution (0-255)
    motor.begin(20000, 8);

    Serial.println("Driving forward - acceleration");
    for (uint16_t pwm = 0; pwm <= 255; ++pwm) {
        motor.forward(pwm);
        delay(50); //256 * 50 = 12.8s
    }
    Serial.println("Driving forward - deceleration");
    for (uint16_t pwm = 255; pwm > 255; --pwm) {
        motor.forward(pwm);
        delay(50); //256 * 50 = 12.8s
    }
    delay(100);
    Serial.println("Driving backward - acceleration");
    for (uint16_t pwm = 0; pwm <= 255; ++pwm) {
        motor.backward(pwm);
        delay(50); //256 * 50 = 12.8s
    }
    Serial.println("Driving backward - deceleration");
    for (uint16_t pwm = 255; pwm > 255; --pwm) {
        motor.backward(pwm);
        delay(50); //256 * 50 = 12.8s
    }
    Serial.println("Motor stopped");
}

void loop() {
}