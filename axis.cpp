/*
Raspberry Pi 
send XYZRPW + 6 ZERO_POINTS-> Teensy

Teensy
- Hexapod class calculates XYZ positions for each of the 3 legs
- Leg class takes XYZ positions and does IK
- Axis class controls motors to specific positions
*/

#include "axis.hpp"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <PWMServo.h>
#include <elapsedMillis.h>

Axis::Axis() {
    _max_pos = 0;
    _min_pos = 0;	
}

void Axis::initializePositionLimits(uint8_t pwm_pin, double min_pos, double max_pos) {
    servo.attach(pwm_pin);
    //servo.write(0);
    _min_pos = min_pos;
    _max_pos = max_pos;
}

uint8_t Axis::moveToPos(double pos) {
    if (pos < _min_pos || pos > _max_pos)
        return 255;     //Move out of range
    if (_next_go_time > _millis)
        return 254;     //Moving too quickly
    uint32_t millis_to_pos = (fabs(getCurrentPos() - pos) / _max_speed) * 1000;
    _next_go_time = _millis + millis_to_pos;
    uint8_t motor_pos = _motorMap(pos); 
    servo.write(motor_pos);
    _current_pos = pos;
    return motor_pos;
}

uint8_t Axis::moveToPosAtSpeed(double pos, double target_speed) { //Must call runSpeed() frequently when using this function
    if (pos < _min_pos || pos > _max_pos)
        return 255;
    double speed = target_speed;
    uint8_t retval = 0;
    if (target_speed > _max_speed) {
        speed = _max_speed;
        retval = 1;
    }
    _move_time = (fabs(getCurrentPos() - pos) / speed) * 1000;
    _start_rads = getCurrentPos();
    _end_rads = pos;
    _move_progress = 0;
    _move_start_time = _millis;
    return retval;
}

uint8_t Axis::runSpeed() {
    _move_progress = (float)(_millis - _move_start_time) / ((float) _move_time);
    if (_move_progress <= 1.0) {
        double angle = _move_progress * (_end_rads - _start_rads) + _start_rads;
        return moveToPos(angle);
    }
    return 253;
}

_Bool Axis::setMapping(double zero_pos, double map_mult) {
    _zero_pos = zero_pos;
    _map_mult = map_mult;
    //moveToPos(0);
    return true;
}

uint8_t Axis::_motorMap(double x) {
  return (uint8_t)(_radsToDegrees((x + _zero_pos) * _map_mult)); 
}

double Axis::_radsToDegrees(double rads) {
    return rads * 180.0 / M_PI;
}

double Axis::_degreesToRads(double degrees) {
    return degrees * M_PI / 180.0;
}

_Bool Axis::setMaxPos(double max_pos) {
    _max_pos = max_pos;
    return true;
}
_Bool Axis::setMinPos(double min_pos) {
    _min_pos = min_pos;
    return true;
}

double Axis::getCurrentPos() {
    return _current_pos;
}

_Bool Axis::setMaxSpeed(double max_speed) {
    _max_speed = max_speed;
    return true;
};

double Axis::getMaxSpeed() {
    return _max_speed;
};
