#include "include/LimitServo.hpp"

const int BAUD_RATE = 9600;
const int MOTOR_STATE_STRING_LENGTH = 12;

const int servoUpDownPin = 7;
const int servoLeftRightPin = 8;
const int servoProximalPanPin = 9;
const int servoProximalTiltPin = 10;

const int servoUpDownEqAngle = 88;
const int servoLeftRightEqAngle = 84;
const int servoProximalPanEqAngle = 95;
const int servoProximalTiltEqAngle = 90;

const int servoUpDownMinAngle = 80;
const int servoLeftRightMinAngle = 78;
const int servoProximalPanMinAngle = 89;
const int servoProximalTiltMinAngle = 84;

const int servoUpDownMaxAngle = 94;
const int servoLeftRightMaxAngle = 100;
const int servoProximalPanMaxAngle = 100;
const int servoProximalTiltMaxAngle = 95;

int servoUpDownPos = servoUpDownEqAngle;
int servoLeftRightPos = servoLeftRightEqAngle;
int servoProximalPanPos = servoProximalPanEqAngle;
int servoProximalTiltPos = servoProximalTiltEqAngle;

char* motorStateString;

LimitServo servoUpDown(
    servoUpDownPin,
    servoUpDownEqAngle,
    servoUpDownMinAngle,
    servoUpDownMaxAngle
);

LimitServo servoLeftRight(
    servoLeftRightPin,
    servoLeftRightEqAngle,
    servoLeftRightMinAngle,
    servoLeftRightMaxAngle
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

    servoUpDown.Attach();
    servoLeftRight.Attach();
    servoProximalPan.Attach();
    servoProximalTilt.Attach();

    servoUpDown.MoveToEquilibrium();
    servoLeftRight.MoveToEquilibrium();
    servoProximalPan.MoveToEquilibrium();
    servoProximalTilt.MoveToEquilibrium();

    bool hasSentStartupString = false;
    motorStateString = (char *)malloc(MOTOR_STATE_STRING_LENGTH * sizeof(char));

    int servoUpDownEqAngle = servoUpDown.GetEquilibriumAngle();
    int servoLeftRightEqAngle = servoLeftRight.GetEquilibriumAngle();
    int servoProximalPanEqAngle = servoProximalPan.GetEquilibriumAngle();
    int servoProximalTiltEqAngle = servoProximalTilt.GetEquilibriumAngle();

    sprintf(motorStateString, "%03d%03d%03d%03d\n", servoUpDownEqAngle, servoLeftRightEqAngle, servoProximalPanEqAngle, servoProximalTiltEqAngle);

    while (!Serial)
    {
    }
    
    while (!hasSentStartupString)
    {
        Serial.print(motorStateString);

        String response = Serial.readStringUntil('\n');
        hasSentStartupString = (response == "OK");
    }

    memset(motorStateString, 0, MOTOR_STATE_STRING_LENGTH * sizeof(char));
}

void loop() {
    while (Serial.available())
    {
        String commandString = Serial.readStringUntil('\n');

        // Serial.print("Received command: ");
        // Serial.print(commandString);
        // Serial.print(", Length: ");
        // Serial.println(commandString.length());

        if (commandString.length() == 12)
        {
            // Serial.print("Moving motor 1 to: ");
            // Serial.println(state.substring(0,3));
            servoUpDown.Move(commandString.substring(0,3).toInt());

            // Serial.print("Moving motor 2 to: ");
            // Serial.println(state.substring(3,6));
            servoLeftRight.Move(commandString.substring(3,6).toInt());

            // Serial.print("Moving motor 3 to: ");
            // Serial.println(state.substring(6,9));
            servoProximalPan.Move(commandString.substring(6,9).toInt());

            // Serial.print("Moving motor 4 to: ");
            // Serial.println(state.substring(9,12));
            servoProximalTilt.Move(commandString.substring(9,12).toInt());

            sprintf(motorStateString, "%03d%03d%03d%03d\n", servoUpDown.GetAngle(), servoLeftRight.GetAngle(), servoProximalPan.GetAngle(), servoProximalTilt.GetAngle());
            Serial.print(motorStateString);
        }
    }
}
