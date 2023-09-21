#include <MX1508.h>

#define PINA 9
#define PINB 10
// MX1508 schematics(in Chinese) can be found here at: http://sales.dzsc.com/486222.html
/*
 * MX1508(uint8_t pinIN1, uint8_t pinIN2, DecayMode decayMode, NumOfPwmPins numPWM);
 * DecayMode must be FAST_DECAY or SLOW_DECAY,
 * NumOfPwmPins, either use 1 or 2 pwm.
 * I recommend using 2 pwm pins per motor so spinning motor forward and backward gives similar response.
 * if using 1 pwm pin, make sure its pinIN1, then set pinIN2 to any digital pin. I dont recommend this setting because
 * we need to use FAST_DECAY in one direction and SLOW_DECAY for the other direction.
 */
MX1508 motorA(PINA, PINB, FAST_DECAY, PWM_2PIN);

void setup() {}

void loop(){
    
    // running motor during 10 sec at max speed (pwm=1).
    motorA.motorGo(1);
    delay(10000);

    // braking motor to stop immediately the rotation (during 300 ms).
    motorA.brakeMotor();
    delay(300);

    // then standby the motor during 3 sec.
    motorA.stopMotor();
    delay(3000);
}
