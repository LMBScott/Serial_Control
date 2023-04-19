#include <Servo.h>
#include <Arduino.h>

class LimitServo
{
private:
    Servo servo;
    int pinNum;
    int eqAngle;
    int minAngle;
    int maxAngle;
public:
    LimitServo(int pinNum, int eqAngle, int minAngle, int maxAngle);
    void Attach();
    void MoveToEquilibrium();
    void Move(int angle);
    int GetEquilibriumAngle();
    int GetAngle();
};
