#include "../include/LimitServo.hpp"

LimitServo::LimitServo(int pinNum, int eqAngle, int minAngle, int maxAngle)
{
    this->pinNum = pinNum;
    this->eqAngle = eqAngle;
    this->minAngle = minAngle;
    this->maxAngle = maxAngle;
}

void LimitServo::Attach()
{
    servo.attach(pinNum);
}

void LimitServo::MoveToEquilibrium()
{
    servo.write(constrain(eqAngle, minAngle, maxAngle));
}

void LimitServo::Move(int angle)
{
    servo.write(constrain(angle, minAngle, maxAngle));
}

int LimitServo::GetEquilibriumAngle()
{
    return eqAngle;
}

int LimitServo::GetAngle()
{
    return servo.read();
}
