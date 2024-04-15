#pragma once

#include <Arduino.h>

class BTS7960_HBridge {
  public:
    BTS7960_HBridge(uint8_t rPin, uint8_t lPin, uint8_t enPin = 255);

    void begin(uint16_t pwmFreq = 1000, uint8_t pwmRes = 8);

    void stop();
    void forward(uint16_t pwm);
    void backward(uint16_t pwm);
    void dynamicBrake(uint16_t pwm);
    void reverseBrake(uint16_t pwm);

    inline uint16_t getMaxPWM() {return pwmMax_;}

  private:

    uint16_t pwmMax_;
    uint8_t rPin_, lPin_, enPin_;
    uint16_t pwm_;
    uint16_t pwmFreq_;
    uint8_t pwmRes_, pwmChannel_ = 1;
    bool enabled_ = false;
    enum states {
      unconfigured_, forward_, backward_, dynamicBrake_, reverseBrake_
    };
    states state_ = unconfigured_;
};