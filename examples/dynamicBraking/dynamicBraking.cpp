// Simple test to check how dynamic braking works
// Motor will be drived to max speed and then dynamic braking will be turned on
// In dynamic braking motor works as generator and we dissipating power in wires, motor and h-bridge resistance
// Remember that dynamic braking with high pwm settings may damage motor or H-bridge, especially if motor is big
// Note that not every h-bridge is capable of dynamic braking - BTS7960 works because current can flow via integrated diode - look up on schematic on github
//
// Dynamic braking usually offers slower braking than dynamic braking, but there is no worry about start to rotate motor backwards

#include <Arduino.h>
#include "BTS7960_HBridge.h"

// Change pins if you need
// On BTS7960 h-bridge module there are pins R_EN and L_EN - connect them together to pin EN, actually there will be pwm signal
// R_PWM and L_PWM pins on the module won't be driven by pwm
#define EN 23
#define R_PWM 19
#define L_PWM 18

// PWM value for dynamic braking - try from low values to not damage motor or H-bridge
#define PWM_BREAKING 63

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
    Serial.println("Dynamic braking");
    motor.dynamicBrake(PWM_BREAKING);
    // Because we don't driving motor backward there is no worry about stopping motor and then spinning it backward like in reverse braking
    //delay(BREAKING_TIME);
    Serial.println("Motor stopped");
}

void loop() {
}