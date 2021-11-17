
#include <MX1508.h>
// Must have enableInterrupt.h library for Quadrature Encoder
#include <QuadratureEncoder.h>


#define PINA 9
#define PINB 10  // Can be any digital pin. if using NUMPWM = 1
#define NUMPWM PWM_1PIN
// MX1508 schematics(in Chinese) can be found here at: http://sales.dzsc.com/486222.html
/*
   MX1508(uint8_t pinIN1, uint8_t pinIN2, DecayMode decayMode, NumOfPwmPins numPWM);
   DecayMode must be FAST_DECAY or SLOW_DECAY,
   NumOfPwmPins, either use PWM_1PIN or PWM_2PIN.
   I recommend using 2 pwm pins per motor so spinning motor forward and backward gives similar response.
   if using 1 pwm pin, make sure its pinIN1, then set pinIN2 to any digital pin. I don't recommend this setting because
   we need to use FAST_DECAY in one direction and SLOW_DECAY for the other direction.
*/
MX1508 motorA(PINA, PINB, SLOW_DECAY, NUMPWM);
Encoders motorEncoder(3, 2);
void setup() {
  Serial.begin(115200);
}
/*
   Ramp up to pwm = 100, by increasing pwm by 1 every 50 millisecond.
   then ramp down to pwm = -100, by decreasing pwm every 50 millisecond.
   positive value pwm results in forward direction.
   negative value pwm results in opposite direction.
*/
void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long lastMilli = 0;
  static bool cwDirection = true; // assume initial direction(positive pwm) is clockwise
  static int pwm = 1;
  static long lastEncoderCount = 0;

  if (millis() - lastMilli > 50) { // every 50 millisecond
    lastMilli = millis();
    if (cwDirection && pwm++ > 200 ) { // Increase PWM until 200
      cwDirection = false;
    } else if (!cwDirection && pwm-- < -200) { // Decrease PWM until -200
      cwDirection =  true;
    }
    long currentEncoderCount = motorEncoder.getEncoderCount();
    long encoderSpeed = currentEncoderCount - lastEncoderCount; // units = cts/50ms
    lastEncoderCount = currentEncoderCount;
    
    motorA.motorGo(pwm);  // Command motor to go. similar to analogWrite(). 
    
    Serial.print(encoderSpeed);
    Serial.print(" , ");
    Serial.println(motorA.getPWM()); // we can just print pwm but just showing that member function getPWM() works.
  }

}
