#include <ncurses.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "../include/serialib.h"

const char* SERIAL_PORT  = "/dev/ttyACM0";
const int BAUD_RATE = 9600;
const int MAX_RETURN_BYTES = 16;
const int MOTOR_STATE_STRING_LENGTH = 13;

bool ReadMotorPositions(char* motorStateBuffer, int& motor1Pos, int&motor2Pos, int& motor3Pos, int& motor4Pos);

int main(int argc, char *argv[])
{
    serialib serial;

    // Connection to serial port
    char wasOpenSuccessful = serial.openDevice(SERIAL_PORT, BAUD_RATE);

    // If connection fails, return the error code otherwise, display a success message
    if (wasOpenSuccessful != 1)
    {
        std::cout << "Failed to connect to " << SERIAL_PORT << "\n";
        return wasOpenSuccessful;
    }

    std::cout << "Successful connection to " << SERIAL_PORT << "\n";

    int motor1Pos = 90;
    int motor2Pos = 90;
    int motor3Pos = 90;
    int motor4Pos = 90;

    char *motorStateReadBuffer = (char *)malloc(MAX_RETURN_BYTES * sizeof(char));

    std::cout << "Awaiting startup message...\n";

    while (!ReadMotorPositions(motorStateReadBuffer, motor1Pos, motor2Pos, motor3Pos, motor4Pos))
    {
        memset(motorStateReadBuffer, 0, MAX_RETURN_BYTES * sizeof(char));
        serial.readString(motorStateReadBuffer, '\n', MAX_RETURN_BYTES, 100);
    }

    std::cout << "Read string " << motorStateReadBuffer << "\n";
    std::cout << "Read motor equilibrium angles: " << motor1Pos << ", " << motor2Pos << ", " << motor3Pos << ", " << motor4Pos << "\n";

    int motor1TargetPos = motor1Pos;
    int motor2TargetPos = motor2Pos;
    int motor3TargetPos = motor3Pos;
    int motor4TargetPos = motor4Pos;

    memset(motorStateReadBuffer, 0, MAX_RETURN_BYTES * sizeof(char));

    serial.writeString("OK\n");

    int c = 0;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    scrollok(stdscr, TRUE);
    idlok(stdscr, TRUE);
    cbreak();
    nodelay(stdscr, TRUE);

    printw("Starting control\n");

    std::ostringstream commandStream;

    bool shouldSendCommand = false;

    while(c != '#')
    {
        c = 0;
        shouldSendCommand = false;

        switch((c = getch())) {
            case 'w':
                motor1TargetPos -= 1;
                shouldSendCommand = true;
                printw("Up\n");
                break;
            case 's':
                motor1TargetPos += 1;
                shouldSendCommand = true;
                printw("Down\n");
                break;
            case 'a':
                motor2TargetPos += 1;
                shouldSendCommand = true;
                printw("Left\n");
                break;
            case 'd':
                motor2TargetPos -= 1;
                shouldSendCommand = true;
                printw("Right\n");
                break;
            case 'q':
                motor3TargetPos += 1;
                shouldSendCommand = true;
                printw("Proximal pan +\n");
                break;
            case 'c':
                motor3TargetPos -= 1;
                shouldSendCommand = true;
                printw("Proximal pan -\n");
                break;
            case 'e':
                motor4TargetPos += 1;
                shouldSendCommand = true;
                printw("Proximal tilt +\n");
                break;
            case 'z':
                motor4TargetPos -= 1;
                shouldSendCommand = true;
                printw("Proximal tilt -\n");
                break;
        }

        if (shouldSendCommand)
        {
            printw("Sending command\n");
            commandStream << std::setfill('0') << std::setw(3) << motor1TargetPos << std::setfill('0') << std::setw(3) << motor2TargetPos << std::setfill('0') << std::setw(3) << motor3TargetPos << std::setfill('0') << std::setw(3) << motor4TargetPos << '\n';

            printw("Sending command: %s", commandStream.str().c_str());

            serial.writeString(commandStream.str().c_str());

            while (!ReadMotorPositions(motorStateReadBuffer, motor1Pos, motor2Pos, motor3Pos, motor4Pos))
            {
                memset(motorStateReadBuffer, 0, MAX_RETURN_BYTES * sizeof(char));
                serial.readString(motorStateReadBuffer, '\n', MAX_RETURN_BYTES, 100);
            }

            printw("%s\n", motorStateReadBuffer);
            memset(motorStateReadBuffer, 0, MAX_RETURN_BYTES * sizeof(char));
            commandStream.str("");
        }
        else
        {
            printw("No command sent\n");
        }
    }

    free(motorStateReadBuffer);

    refresh();
    getch();
    endwin();

    // Close the serial device
    serial.closeDevice();

    return 0;
}

bool ReadMotorPositions(char* motorStateBuffer, int& motor1Pos, int&motor2Pos, int& motor3Pos, int& motor4Pos)
{
    // Read motor equilibrium angles from control board
    if (strlen(motorStateBuffer) != MOTOR_STATE_STRING_LENGTH)
    {
        return false;
    }

    std::string motorState(motorStateBuffer);

    motor1Pos = std::stoi(motorState.substr(0,3));
    motor2Pos = std::stoi(motorState.substr(3,3));
    motor3Pos = std::stoi(motorState.substr(6,3));
    motor4Pos = std::stoi(motorState.substr(9,3));

    return true;
}
