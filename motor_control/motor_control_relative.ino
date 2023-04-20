#include "../include/LimitServo.hpp"

const int BAUD_RATE = 9600;

const int servoDistalPanPin = 7;
const int servoDistalPanPin = 8;
const int servoProximalPanPin = 9;
const int servoProximalTiltPin = 10;

const int servoDistalTiltEqAngle = 88;
const int servoDistalPanEqAngle = 84;
const int servoProximalPanEqAngle = 95;
const int servoProximalTiltEqAngle = 90;

const int servoDistalTiltMinAngle = 80;
const int servoDistalPanMinAngle = 78;
const int servoProximalPanMinAngle = 89;
const int servoProximalTiltMinAngle = 84;

const int servoDistalTiltMaxAngle = 94;
const int servoDistalPanMaxAngle = 100;
const int servoProximalPanMaxAngle = 100;
const int servoProximalTiltMaxAngle = 95;

int servoUpDownPos = servoDistalTiltEqAngle;
int servoLeftRightPos = servoDistalPanEqAngle;
int servoProximalPanPos = servoProximalPanEqAngle;
int servoProximalTiltPos = servoProximalTiltEqAngle;

LimitServo servoDistalTilt(
    servoDistalPanPin,
    servoDistalTiltEqAngle,
    servoDistalTiltMinAngle,
    servoDistalTiltMaxAngle
);

LimitServo servoDistalPan(
    servoDistalPanPin,
    servoDistalPanEqAngle,
    servoDistalPanMinAngle,
    servoDistalPanMaxAngle
);

LimitServo servoProximalPan(
    servoProximalPanPin,
    servoProximalPanEqAngle,
    servoProximalPanMinAngle,
    servoProximalPanMaxAngle
);

LimitServo servoProximalTilt(
    servoProximalTiltPin,
    servoProximalTiltEqAngle,
    servoProximalTiltMinAngle,
    servoProximalTiltMaxAngle
);

void setup()
{
    Serial.begin(BAUD_RATE);

    servoDistalTilt.Attach();
    servoDistalPan.Attach();
    servoProximalPan.Attach();
    servoProximalTilt.Attach();

    servoDistalTilt.MoveToEquilibrium();
    servoDistalPan.MoveToEquilibrium();
    servoProximalPan.MoveToEquilibrium();
    servoProximalTilt.MoveToEquilibrium();

    // if (Serial.available())
    // {
    //     int servoUpDownEqAngle = servoUpDown.GetEquilibriumAngle();
    //     int servoLeftRightEqAngle = servoLeftRight.GetEquilibriumAngle();
    //     int servoProximalPanEqAngle = servo3.GetEquilibriumAngle();
    //     int servoProximalTiltEqAngle = servo4.GetEquilibriumAngle();

    //     String eqOutputString;
    //     sprintf(eqOutputString, "%3d%3d%3d%3d", servoUpDownEqAngle, servoLeftRightEqAngle, servoProximalPanEqAngle, servoProximalTiltEqAngle);
    //     Serial.print();
    // }
    delay(1000);
}

void loop() {
    while (Serial.available())
    {
        String state = Serial.readStringUntil('\n');

        Serial.print("Received command: ");
        Serial.print(state);
        Serial.print(", Length: ");
        Serial.println(state.length());

        if (state.length() == 12)
        {
            // Serial.print("Moving motor 1 to: ");
            // Serial.println(state.substring(0,3));
            servoDistalTilt.Move(state.substr(0,3).toInt());

            // Serial.print("Moving motor 2 to: ");
            // Serial.println(state.substring(3,6));
            servoDistalPan.Move(state.substr(3,6).toInt());

            // Serial.print("Moving motor 3 to: ");
            // Serial.println(state.substring(6,9));
            servoProximalPan.Move(state.substr(6,9).toInt());

            // Serial.print("Moving motor 4 to: ");
            // Serial.println(state.substring(9,12));
            servoProximalTilt.Move(state.substr(9,12).toInt())
        }
    }
}
