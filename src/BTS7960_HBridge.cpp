#include "BTS7960_HBridge.h"

BTS7960_HBridge::BTS7960_HBridge(uint8_t rPin, uint8_t lPin, uint8_t enPin) {
  this->enPin_ = enPin;
  this->rPin_ = rPin;
  this->lPin_ = lPin;
}

/*  Sets pins
    @param pwmFreq PWM frequency
    @param pwmRes PWM resolution
*/
void BTS7960_HBridge::begin(uint16_t pwmFreq, uint8_t pwmRes) {
  pinMode(this->enPin_, OUTPUT);
  pinMode(this->lPin_, OUTPUT);
  pinMode(this->rPin_, OUTPUT);

  this->pwmFreq_ = pwmFreq;
  this->pwmRes_ = pwmRes;

  this->pwmMax_ = (1 << this->pwmRes_) - 1;

  ledcSetup(this->pwmChannel_, this->pwmFreq_, this->pwmRes_);
  ledcAttachPin(this->enPin_, this->pwmChannel_);
  this->stop();
}

/* Stops driving motor - high impedance outputs
*/
void BTS7960_HBridge::stop() {
  ledcWrite(this->pwmChannel_, 0);
}

/*  Driving motor forward with pwm
    @param pwm pwm to set
*/
void BTS7960_HBridge::forward(uint16_t pwm) {
  this->pwm_ = constrain(pwm, 0, this->pwmMax_);
  ledcWrite(this->pwmChannel_, this->pwm_);
  if (this->state_ != forward_) {
    digitalWrite(this->lPin_, 0);
    digitalWrite(this->rPin_, 1);
    this->state_ = forward_;
  }
}

/*  Driving motor backward with pwm
    @param pwm pwm to set
*/
void BTS7960_HBridge::backward(uint16_t pwm) {
  this->pwm_ = constrain(pwm, 0, this->pwmMax_);
  ledcWrite(this->pwmChannel_, this->pwm_);
  if (this->state_ != backward_) {
    digitalWrite(this->lPin_, 1);
    digitalWrite(this->rPin_, 0);
    this->state_ = backward_;
  }
}

/*  Dynamic braking with specified PWM - connecting both motor pins to ground so motor is working as generator
    and power is dissipated across motor, wires and H-bridge resistance
    @warning High braking PWM on large motors may damage H-bridge or themselves
    @param pwm PWM to set
*/
void BTS7960_HBridge::dynamicBrake(uint16_t pwm) {
  this->pwm_ = constrain(pwm, 0, this->pwmMax_);
  ledcWrite(this->pwmChannel_, this->pwm_);
  if (this->state_ != dynamicBrake_) {
    digitalWrite(this->lPin_, 0);
    digitalWrite(this->rPin_, 0);
    this->state_ = dynamicBrake_;
  }
}

/*  Reverse current braking with specified PWM - drives motor with reverse current.
    If set dynamic braking before then this only changes dynamic brake PWM value.
    @warning High braking PWM on large motors may damage H-bridge or themselves
    @param pwm PWM to set
*/
void BTS7960_HBridge::reverseBrake(uint16_t pwm) {
  this->pwm_ = constrain(pwm, 0, this->pwmMax_);
  ledcWrite(this->pwmChannel_, this->pwm_);
  switch (this->state_) {
    case dynamicBrake_:
      break;
    
    case forward_:
      digitalWrite(this->lPin_, 1);
      digitalWrite(this->rPin_, 0);
      this->state_ = reverseBrake_;
      break;

    case backward_:
      digitalWrite(this->lPin_, 0);
      digitalWrite(this->rPin_, 1);
      this->state_ = reverseBrake_;
      break;

    default:
      break;

  }
}