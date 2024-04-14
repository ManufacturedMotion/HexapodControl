#include <stdint.h>
#include <math.h>
#include "threebythree.hpp"

void ThreeByThree::mult_left_three_by_three(const ThreeByThree& left) {
    double temp[3][3];
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            temp[i][j] = 0;
        }
    }

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            for (uint8_t k = 0; k < 3; k++) {
                temp[i][j] += left.values[i][k] * values[k][j];
            }
        }
    }
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            values[i][j] = temp[i][j];
        }
    }
}
void ThreeByThree::mult_right_three_by_three(const ThreeByThree& right) {
    double temp[3][3];
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            temp[i][j] = 0;
        }
    }

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            for (uint8_t k = 0; k < 3; k++) {
                temp[i][j] += values[i][k] * right.values[k][j];
            }
        }
    }

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            values[i][j] = temp[i][j];
        }
    }
}

double ThreeByThree::value(uint8_t row, uint8_t collumn) {
    return values[row][collumn];
}

void ThreeByThree::invert() {
    double det = value(0,0) * (value(1, 1) * value(2, 2) - value(2, 1) * value(1, 2)) -
             value(0, 1) * (value(1, 0) * value(2, 2) - value(1, 2) * value(2, 0)) +
             value(0, 2) * (value(1, 0) * value(2, 1) - value(1, 1) * value(2, 0));

    double invdet = 1 / det;
    double temp[3][3];
    temp[0][0] = (value(1, 1) * value(2, 2) - value(2, 1) * value(1, 2)) * invdet;
    temp[0][1] = (value(0, 2) * value(2, 1) - value(0, 1) * value(2, 2)) * invdet;
    temp[0][2] = (value(0, 1) * value(1, 2) - value(0, 2) * value(1, 1)) * invdet;
    temp[1][0] = (value(1, 2) * value(2, 0) - value(1, 0) * value(2, 2)) * invdet;
    temp[1][1] = (value(0, 0) * value(2, 2) - value(0, 2) * value(2, 0)) * invdet;
    temp[1][2] = (value(1, 0) * value(0, 2) - value(0, 0) * value(1, 2)) * invdet;
    temp[2][0] = (value(1, 0) * value(2, 1) - value(2, 0) * value(1, 1)) * invdet;
    temp[2][1] = (value(2, 0) * value(0, 1) - value(0, 0) * value(2, 1)) * invdet;
    temp[2][2] = (value(0, 0) * value(1, 1) - value(1, 0) * value(0, 1)) * invdet;

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            values[i][j] = temp[i][j];
        }
    }
}

ThreeByOne::ThreeByOne(double value0, double value1, double value2) {
    values[0] = value0;
    values[1] = value1;
    values[2] = value2;
}

void ThreeByOne::mult_three_by_three(const ThreeByThree& left) {
    double temp[3];
    for (uint8_t i = 0; i < 3; i++) {
        temp[i] = 0;
    }

    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t k = 0; k < 3; k++) {
            temp[i] += values[k] * left.values[i][k];
        }
    }

    for (uint8_t i = 0; i < 3; i++) {
            values[i] = temp[i];
    }
}

void ThreeByOne::rotateYaw(double yaw) {
    ThreeByThree rotation_around_z;
    rotation_around_z.values[0][0] = cos(yaw);
    rotation_around_z.values[0][1] = sin(yaw);
    rotation_around_z.values[0][2] = 0;
    rotation_around_z.values[1][0] = -sin(yaw);
    rotation_around_z.values[1][1] = cos(yaw);
    rotation_around_z.values[1][2] = 0;
    rotation_around_z.values[2][0] = 0;
    rotation_around_z.values[2][1] = 0;
    rotation_around_z.values[2][2] = 1;
    mult_three_by_three(rotation_around_z);
}

void ThreeByOne::rotatePitch(double pitch) { 
    ThreeByThree rotation_around_y;
    rotation_around_y.values[0][0] = cos(pitch);
    rotation_around_y.values[0][1] = 0;
    rotation_around_y.values[0][2] = -sin(pitch);
    rotation_around_y.values[1][0] = 0;
    rotation_around_y.values[1][1] = 1;
    rotation_around_y.values[1][2] = 0;
    rotation_around_y.values[2][0] = sin(pitch);
    rotation_around_y.values[2][1] = 0;
    rotation_around_y.values[2][2] = cos(pitch);
    mult_three_by_three(rotation_around_y);
}
void ThreeByOne::rotateRoll(double roll) {
    ThreeByThree rotation_around_x;
    rotation_around_x.values[0][0] = 1;
    rotation_around_x.values[0][1] = 0;
    rotation_around_x.values[0][2] = 0;
    rotation_around_x.values[1][0] = 0;
    rotation_around_x.values[1][1] = cos(roll);
    rotation_around_x.values[1][2] = sin(roll);
    rotation_around_x.values[2][0] = 0;
    rotation_around_x.values[2][1] = -sin(roll);
    rotation_around_x.values[2][2] = cos(roll);
    mult_three_by_three(rotation_around_x);
}

void ThreeByOne::operator+=(const ThreeByOne& addend) {
    values[0] += addend.values[0];
    values[1] += addend.values[1];
    values[2] += addend.values[2];
}

ThreeByOne ThreeByOne::operator+(const ThreeByOne& addend) {
    ThreeByOne result = ThreeByOne(values[0], values[1], values[2]);
    result += addend;
    return result;
}
