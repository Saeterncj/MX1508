#ifndef MX1508_h
#define MX1508_h

#include "Arduino.h"

typedef void (*CallBackFunction) ();

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
    void motorGo(int pwmVal); // 
    void setResolution(int resolution);
    int getPWM();
    void stopMotor(); 
     
  private:
  
    byte _pinIN1;
    byte _pinIN2;
    int _pwmVal;
    int _pwmResolution;   //max resolution of pwm, default is 255.  
    DecayMode _whichMode;
    NumOfPwmPins _numPwmPins;
};

#endif
