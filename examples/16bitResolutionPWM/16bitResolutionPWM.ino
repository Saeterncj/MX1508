
/*
 * Created by: Cheng Saetern
 * This example is meant to demonstrate the ability to adjust pwm frequency and/or  
 * resolution for Arduino Nano/Uno pin 9 and 10 using function setPWM16(uint8_t prescaler, unsigned int resolution).
 * Equation for fast pwm frequency = frequency of MPU(16Mhz)/prescaler/resolution
 * F_PWM = 16000000/prescaler/resolution.
 * prescaler argument value MUST be between 1-5, representing 1,8,64,256,1024 respectively. 
 * resolution MUST be a value from 0-65535.
 */
 
#include <MX1508.h>

#define PINA 9
#define PINB 10
#define NUMPWM 2
// MX1508 schematics(in Chinese) can be found here at: http://sales.dzsc.com/486222.html
/*
 * MX1508(uint8_t pinIN1, uint8_t pinIN2, DecayMode decayMode, NumOfPwmPins numPWM);
 * DecayMode must be FAST_DECAY or SLOW_DECAY,
 * NumOfPwmPins, must be the value of 2 for setPWM16() function  
 * PINA and PINB MUST!!! be pin 9 and 10 for setPWM16() function  
 * Example MX1508 myMotor(10,9,FAST_DECAY, 2). 
 */
MX1508 motorA(PINA,PINB, FAST_DECAY, 2);

int resolution = 1000;
void setup() {
  Serial.begin(115200);
  
  motorA.setPWM16(2,resolution); // prescaler at 8, resolution 1000, PWM frequency = 16Mhz/8/1000=2000Hz
  // prescalar 1=1, 2=8, 3=64, 4=256, 5 =1028
  /*------------ setPWM16 Class function is defined as below-----------------
   void MX1508::setPWM16(uint8_t prescaler, unsigned int resolution){
   if(prescaler > 5 || prescaler == 0) prescaler = 3; // default to 64 if not in range.
    DDRB  |= _BV(PB1) | _BV(PB2);       // set pin 9and 10 as outputs 
      TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);    // non-inverting PWM, mode 14 fastPWM, TOP =ICR1
    TCCR1B = _BV(WGM13) | _BV(WGM12) | prescaler ;  
    ICR1 = resolution;
    _pwmResolution = resolution;
    _useAnalogWrite16 = true; 
  }
  */
}
/*
 * Ramp up to pwm = resolution/2 , by increasing pwm by 1 every 50 millisecond. 
 * then ramp down to pwm = -resolution/2, by decreasing pwm every 50 millisecond.
 * positive value pwm results in forward direction.
 * negative value pwm results in opposite direction. 
 */
void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long lastMilli = 0;
  static bool cwDirection = true; // assume initial direction(positive pwm) is clockwise
  static int pwm = 1;
  
  if(millis()-lastMilli > 50){ // every 50 millisecond
      if (cwDirection && pwm++ > resolution/2 ) {  
        cwDirection = false;
      } else if (!cwDirection && pwm-- < -(resolution/2)) {
        cwDirection =  true;
      } 
    motorA.motorGo(pwm);
    lastMilli = millis();
    Serial.println(motorA.getPWM()); // we can just print pwm but just showing that member function getPWM() works.
  }

}
