// Simple test to check how reverse braking works
// Motor will be drived to max speed and then reverse braking will be turned on for setted time
// In reverse braking motor will be driven with reverse current - motor is spinning forward and we trying to spin it backward
// Remember that reverse braking with high pwm settings may damage motor or H-bridge, especially if motor is big
//
// Reverse braking usually offers faster braking than dynamic braking, but motor may start to rotate backwards

#include <Arduino.h>
#include "BTS7960_HBridge.h"

// Change pins if you need
// On BTS7960 h-bridge module there are pins R_EN and L_EN - connect them together to pin EN, actually there will be pwm signal
// R_PWM and L_PWM pins on the module won't be driven by pwm
#define EN 23
#define R_PWM 19
#define L_PWM 18

// PWM value for reverse braking - try from low values to not damage motor or H-bridge
#define PWM_BREAKING 63
// Breaking time in milliseconds - if is to long then motor may spin bacward
#define BREAKING_TIME 200

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
    Serial.println("Reverse braking");
    motor.reverseBrake(PWM_BREAKING);
    delay(BREAKING_TIME);
    Serial.println("Motor stopped");
}

void loop() {
}