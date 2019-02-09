#include "MX1508.h"

MX1508::MX1508( uint8_t pinIN1, uint8_t pinIN2) {
  _pinIN1 = pinIN1; // always a PWM pin
  _pinIN2 = pinIN2; // can be a non-Pwm pin.
  _pwmResolution = 255;
  _whichMode = FAST_DECAY;
  _numPwmPins = PWM_2PIN;
  pinMode(_pinIN1, OUTPUT);
  pinMode(_pinIN2, OUTPUT);
}

MX1508::MX1508( uint8_t pinIN1, uint8_t pinIN2, DecayMode decayMode, NumOfPwmPins numPins) {
  _pinIN1 = pinIN1; // always a PWM pin
  _pinIN2 = pinIN2; // can be a non-Pwm pin.
  _pwmResolution = 255;
  _whichMode = decayMode;
  _numPwmPins = numPins;
  pinMode(_pinIN1, OUTPUT);
  pinMode(_pinIN2, OUTPUT);
}

int MX1508::getPWM() {
  return _pwmVal;
}

void MX1508::stopMotor() {
  digitalWrite(_pinIN1, LOW);
  digitalWrite(_pinIN2, LOW);
}

void MX1508::setResolution(int pwmResolution) {
  _pwmResolution = pwmResolution;
}

void MX1508::motorGo(int pwmSpeed) {
  _pwmVal = pwmSpeed;
  
  // if set decay mode is set as fast decay mode
  if (this->_whichMode == FAST_DECAY) {
    if (pwmSpeed >= 0) {                        //forward fast decay
      digitalWrite(_pinIN2, LOW);
      analogWrite(_pinIN1, pwmSpeed);
    } else if (this->_numPwmPins == PWM_2PIN) { // reverse fast decay
      pwmSpeed *= -1;
      digitalWrite(_pinIN1, LOW);
      analogWrite(_pinIN2, pwmSpeed);
    } else if (this->_numPwmPins == PWM_1PIN) { // reverse slow decay
      pwmSpeed *= -1;
      pwmSpeed = map(pwmSpeed, 0, 255, 255, 0);
      digitalWrite(_pinIN2, HIGH);
      analogWrite(_pinIN1, pwmSpeed);
    }
  } 
   // if decay mode is set as slow decay mode
  else {
    if (pwmSpeed >= 0) {                        // forward slow decay
      pwmSpeed = map(pwmSpeed, 0, 255, 255, 0);
      digitalWrite(_pinIN1, HIGH);
      analogWrite(_pinIN2, pwmSpeed);
    } else if (this->_numPwmPins == PWM_2PIN) { // reverse slow decay
      pwmSpeed *= -1;
      pwmSpeed = map(pwmSpeed, 0, 255, 255, 0);
      digitalWrite(_pinIN2, HIGH);
      analogWrite(_pinIN1, pwmSpeed);
    } else if (this->_numPwmPins == PWM_1PIN) { // reverse fast decay
      pwmSpeed *= -1;
      digitalWrite(_pinIN1, LOW);
      analogWrite(_pinIN2, pwmSpeed);
    }
  }
}
