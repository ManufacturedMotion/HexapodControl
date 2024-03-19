#ifndef THREEBYTHREE_H
#define THREEBYTHREE_H
 
#include <stdint.h>

class ThreeByThree {
    public:
        double values[3][3];
        void mult_left_three_by_three(const ThreeByThree& left);
        void mult_right_three_by_three(const ThreeByThree& right);
        void invert();
        double value(uint8_t row, uint8_t collumn);
    private:
};

class ThreeByOne {
    public:
        ThreeByOne(double value0, double value1, double value2);
        double values[3];
        void mult_three_by_three(const ThreeByThree& left);
        void rotateYaw(double yaw);
        void rotatePitch(double pitch);
        void rotateRoll(double roll);
        ThreeByOne add(const ThreeByOne& addend);
        void operator+=(const ThreeByOne& addend);
        ThreeByOne operator+(const ThreeByOne& addend);
    private:
};

#endif
