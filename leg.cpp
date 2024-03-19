#include "leg.hpp"
#include "hexapod.hpp"
#include "axis.hpp"
#include "config.hpp"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "threebythree.hpp"


double zero_points[NUM_LEGS][NUM_AXES_PER_LEG] = ZERO_POINTS;
int pwm_pins[NUM_LEGS][NUM_AXES_PER_LEG] = PWM_PINS;
double min_pos[NUM_LEGS][NUM_AXES_PER_LEG] = MIN_POS;
double max_pos[NUM_LEGS][NUM_AXES_PER_LEG] = MAX_POS;
double scale_fact[NUM_LEGS][NUM_AXES_PER_LEG] = SCALE_FACT;
_Bool reverse_axis[NUM_LEGS][NUM_AXES_PER_LEG] = REVERSE_AXIS;

Leg::Leg() {
    _leg_number = 0;
}

void Leg::initializeAxes(uint8_t leg_number) {
    _leg_number = leg_number;
    for (uint8_t i = 0; i < NUM_AXES_PER_LEG; i++) {
        axes[i].initializePositionLimits(pwm_pins[_leg_number-1][i], min_pos[_leg_number-1][i], max_pos[_leg_number-1][i]);
        axes[i].setMapping(zero_points[_leg_number-1][i], scale_fact[_leg_number-1][i], reverse_axis[_leg_number-1][i]);
    }
}

_Bool Leg::inverseKinematics(double x, double y, double z) {

    if (!checkSafeCoords(x,y,z))
        return false; 
    double potential_results[3];
    if (fabs(y < 0.0005)) {
        if (x > 0.0005) {
            potential_results[0] = PI / 2.0;
        }
        else if (x < -0.0005) {
            potential_results[0] = -PI / 2.0;
        }
        else {
            potential_results[0] = 0;
        }
    }  
    else {
        potential_results[0] = atan2(x,y);
    }
    _Bool negative_theta0_flag = false;
    // If the most direct route to the point is out of reach try negative
    if (potential_results[0] > axes[0].getMaxPos()) {
        double other_possibility = potential_results[0] - PI;
        if ((other_possibility < axes[0].getMaxPos()) && (other_possibility >axes[0].getMinPos())) {
            potential_results[0] = other_possibility;
            negative_theta0_flag = true;
        } 
        else {
            return false;
        }
    }

    double x_virtual;
    double y_virtual;

    double y_virtual_planar;
    double planar_distance;

    double theta1_tool0;
    double theta1_tool1;

    double theta2_tool;

    
    x_virtual = x - _length0 * cos(potential_results[0]);
    y_virtual = y - _length0 * sin(potential_results[0]);
    if (!negative_theta0_flag) {
        y_virtual_planar = sqrt(x_virtual * x_virtual + y_virtual * y_virtual);
    }
    else {
        y_virtual_planar = - 1.0 * sqrt(x_virtual * x_virtual + y_virtual * y_virtual);
    }
    planar_distance = - 1.0 * sqrt(y_virtual_planar * y_virtual_planar + z * z);

    theta1_tool0 = atan2(y_virtual_planar, z);
    theta1_tool1 = acos((planar_distance * planar_distance + _length1 * _length1 - _length2 * _length2) / (2 * planar_distance * _length1));

    theta2_tool = acos((_length1 * _length1 + _length2 * _length2 - planar_distance * planar_distance) / (2 * _length1 * _length2));
    Serial.println(PI);
    potential_results[1] = theta1_tool0 - theta1_tool1;
    potential_results[2] = PI - theta2_tool;
    Serial.printf("t1_tool0: %f; t1_tool1: %f; t2_tool: %f\n", theta1_tool0, theta1_tool1, theta2_tool);
    Serial.printf("x_virt: %f; y_virt: %f; y_virt_planar: %f\n", x_virtual, y_virtual, y_virtual_planar);
    // potential_results[1] = theta1_tool0 + theta1_tool1;
    // potential_results[2] = -(PI - theta2_tool);

    for (uint8_t i = 0; i < NUM_AXES_PER_LEG; i++) {
        if (potential_results[i] != potential_results[i]) { //Check for NaN
            Serial.println(i);
            Serial.println(potential_results[i]);
            return false;
        }
    }

    ThreeByOne resulting_pos = forwardKinematics(potential_results[0], potential_results[1], potential_results[2]);
    Serial.printf("Result\n  x: %f; y: %f; z: %f", resulting_pos.values[0], resulting_pos.values[1], resulting_pos.values[2]);

    
    for (uint8_t i = 0; i < NUM_AXES_PER_LEG; i++) {
        if (i == 0 && fabs(potential_results[i]) >= 0.01)
            _next_angles[i] = potential_results[i] + _home_yaw;
        else
            _next_angles[i] = potential_results[i];
    }
    return true;
}

void Leg::setHomeYaw(double home_yaw) {
    _home_yaw = home_yaw;
    Serial.printf("Leg: %f; home yaw: %f\n", _leg_number, _home_yaw);
}

_Bool Leg::checkSafeCoords(double x, double y, double z) {
    //If coords within robot body
    //  return false;
    
    return true;
}

_Bool Leg::rapidMove(double x,  double y, double z) {
    if (inverseKinematics(x, y, z)) {
        moveAxes();
        return true;
    }
    return false;
}

_Bool Leg::linearMove(double x,  double y, double z, double speed) {
    return false;
}

void Leg::moveAxes() {
    for (uint8_t i = 0; i < NUM_AXES_PER_LEG; i++) {
        axes[i].moveToPos(_next_angles[i]);
        Serial.println(_next_angles[i]);
    }
    for (uint8_t i = 0; i < NUM_AXES_PER_LEG; i++) {
        current_angles[i] = _next_angles[i];
    }
}

ThreeByOne Leg::forwardKinematics(double axis0_angle, double axis1_angle, double axis2_angle) {
    ThreeByOne length0(_length0, 0.0, 0.0);
    ThreeByOne length1(_length1, 0.0, 0.0);
    ThreeByOne length2(_length2, 0.0, 0.0);

    length0.rotateYaw(axis0_angle);

    length1.rotateYaw(axis0_angle);
    length1.rotatePitch(axis1_angle);

    length2.rotateYaw(axis0_angle);
    length2.rotatePitch(axis1_angle);
    length2.rotatePitch(axis2_angle);

    return length0 + length1 + length2;
}

