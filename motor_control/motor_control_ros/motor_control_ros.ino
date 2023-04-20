#include <ArduinoHardware.h>
#include <ros.h>
#include <motor_angles_msg/MotorAngles.h>

#include "include/LimitServo.hpp"

ros::NodeHandle nodeHandle;

motor_angles_msg::MotorAngles motorStates;

ros::Publisher motorStatePublisher("micro_module_motor_states", &motorStates);

const int servoDistalTiltPin = 7;
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

LimitServo servoDistalTilt(
    servoDistalTiltPin,
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

void motorCommandCallback(const motor_angles_msg::MotorAngles& motorAngles)
{
    servoProximalPan.Move(motorAngles.proximal_pan_angle);
    servoProximalTilt.Move(motorAngles.proximal_tilt_angle);
    servoDistalPan.Move(motorAngles.distal_pan_angle);
    servoDistalTilt.Move(motorAngles.distal_tilt_angle);
}

ros::Subscriber<motor_angles_msg::MotorAngles> motorCommandSubscriber("micro_module_motor_command", &motorCommandCallback);

void setup()
{
    servoDistalTilt.Attach();
    servoDistalPan.Attach();
    servoProximalPan.Attach();
    servoProximalTilt.Attach();

    servoDistalTilt.MoveToEquilibrium();
    servoDistalPan.MoveToEquilibrium();
    servoProximalPan.MoveToEquilibrium();
    servoProximalTilt.MoveToEquilibrium();

    nodeHandle.initNode();
    nodeHandle.advertise(motorStatePublisher);
    nodeHandle.subscribe(motorCommandSubscriber);

    motorStates.proximal_pan_angle = servoProximalPan.GetEquilibriumAngle();
    motorStates.proximal_tilt_angle = servoProximalTilt.GetEquilibriumAngle();
    motorStates.distal_pan_angle = servoDistalPan.GetEquilibriumAngle();
    motorStates.distal_tilt_angle = servoDistalTilt.GetEquilibriumAngle();

    motorStatePublisher.publish(&motorStates);
}

void loop() {
    motorStates.proximal_pan_angle = servoProximalPan.GetAngle();
    motorStates.proximal_tilt_angle = servoProximalTilt.GetAngle();
    motorStates.distal_pan_angle = servoDistalPan.GetAngle();
    motorStates.distal_tilt_angle = servoDistalTilt.GetAngle();

    motorStatePublisher.publish(&motorStates);
    nodeHandle.spinOnce();
}
