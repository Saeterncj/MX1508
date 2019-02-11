#include "MX1508.h"

MX1508::MX1508( uint8_t pinIN1, uint8_t pinIN2) {
  _pinIN1 = pinIN1; // always a PWM pin
  _pinIN2 = pinIN2; // can be a non-Pwm pin.
  _whichMode = FAST_DECAY;
  _numPwmPins = PWM_2PIN;
  pinMode(_pinIN1, OUTPUT);
  pinMode(_pinIN2, OUTPUT);
}

MX1508::MX1508( uint8_t pinIN1, uint8_t pinIN2, DecayMode decayMode, NumOfPwmPins numPins) {
  _pinIN1 = pinIN1; // always a PWM pin
  _pinIN2 = pinIN2; // can be a non-Pwm pin.
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

void MX1508::setResolution(unsigned int pwmResolution) {
  _pwmResolution = pwmResolution;
  if(_useAnalogWrite16) ICR1 = pwmResolution; 
}

void MX1508::setPWM16(uint8_t prescaler, unsigned int resolution){
	if(prescaler > 5 || prescaler == 0) prescaler = 3; // default to 64 if not in range.
	DDRB  |= _BV(PB1) | _BV(PB2);       /* set pin 9and 10 as outputs */
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);    // non-inverting PWM, mode 14 fastPWM, TOP =ICR1
	TCCR1B = _BV(WGM13) | _BV(WGM12) | prescaler ;	// rescaler must be 1->5, 1,8,64,256,1028 respectively
	ICR1 = resolution;
	_pwmResolution = resolution;
	_useAnalogWrite16 = true; 
}

void MX1508::analogWrite16(uint8_t pin, uint16_t val)
{
  if(_useAnalogWrite16){
	if(val < 5) val =5;
	switch (pin) {
		case  9: OCR1A = val; break;
		case 10: OCR1B = val; break;
		default: analogWrite(pin,val);
	} 
  }else{
	  analogWrite(pin, val);
  }
}

void MX1508::motorGo(long pwmSpeed) {
  _pwmVal = pwmSpeed;
  
  // if set decay mode is set as fast decay mode
  if (this->_whichMode == FAST_DECAY) {
    if (pwmSpeed >= 0) {                        //forward fast decay
      if(_numPwmPins == PWM_1PIN)digitalWrite(_pinIN2, LOW);
	  else analogWrite16(_pinIN2, 1);
      analogWrite16(_pinIN1, pwmSpeed);
    } else if (this->_numPwmPins == PWM_2PIN) { // reverse fast decay
      pwmSpeed *= -1;
	  analogWrite16(_pinIN1, 1);
      analogWrite16(_pinIN2, pwmSpeed);
    } else if (this->_numPwmPins == PWM_1PIN) { // reverse slow decay
      pwmSpeed *= -1;
      pwmSpeed = map(pwmSpeed, 0, _pwmResolution, _pwmResolution, 0);
      digitalWrite(_pinIN2, HIGH);
      analogWrite16(_pinIN1, pwmSpeed);
    }
  } 
   // if decay mode is set as slow decay mode
  else {
    if (pwmSpeed >= 0) {                        // forward slow decay
      pwmSpeed = map(pwmSpeed, 0, _pwmResolution, _pwmResolution, 0);
      if(_numPwmPins == PWM_1PIN)digitalWrite(_pinIN1, HIGH);
	  else analogWrite16(_pinIN1, _pwmResolution);
      analogWrite16(_pinIN2, pwmSpeed);
    } else if (this->_numPwmPins == PWM_2PIN) { // reverse slow decay
      pwmSpeed *= -1;
      pwmSpeed = map(pwmSpeed, 0, _pwmResolution, _pwmResolution, 0);
	  analogWrite16(_pinIN2, _pwmResolution);
      analogWrite16(_pinIN1, pwmSpeed);
    } else if (this->_numPwmPins == PWM_1PIN) { // reverse fast decay
      pwmSpeed *= -1;
      digitalWrite(_pinIN1, LOW);
      analogWrite16(_pinIN2, pwmSpeed);
    }
  }
}
