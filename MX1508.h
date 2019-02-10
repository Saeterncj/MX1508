#ifndef MX1508_h
#define MX1508_h

#include "Arduino.h"

typedef enum
{
  FAST_DECAY  = 0,  // set non-PWM pin low
  SLOW_DECAY = 1    // set non-PWM pin high
} DecayMode;

typedef enum
{
  PWM_1PIN  = 1,
  PWM_2PIN  = 2
} NumOfPwmPins;

class MX1508 {
  public:
    MX1508(uint8_t pinIN1, uint8_t pinIN2); // default fast decay, 2 pwm pins
    MX1508(uint8_t pinIN1, uint8_t pinIN2, DecayMode decayMode, NumOfPwmPins numPWM);
    void motorGo(long pwmVal); // 
    void setResolution(unsigned int resolution);
    int getPWM();
    void stopMotor(); 
	void analogWrite16(uint8_t pin, uint16_t val);
    void setPWM16(uint8_t prescaler, unsigned int resolution);
  private:
  
    uint8_t _pinIN1;
    uint8_t _pinIN2;
	bool _useAnalogWrite16 = false;
    int _pwmVal;
    int _pwmResolution = 255;   //max resolution of pwm, default is 255.  
    DecayMode _whichMode;
    NumOfPwmPins _numPwmPins;
};

#endif
