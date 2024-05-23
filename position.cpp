#include "position.hpp"
#include <stdbool.h>
#include <math.h>
#include "three_by_matrices.hpp"


void Position::set(double new_X, double new_Y, double new_Z, double new_roll, double new_pitch, double new_yaw) {
    X = new_X;
    Y = new_Y;
    Z = new_Z;
    roll = new_roll;
    pitch = new_pitch;
    yaw = new_yaw;
}

void Position::setPos(const Position& pos) {
    X = pos.X;
    Y = pos.Y;
    Z = pos.Z;
    roll = pos.roll;
    pitch = pos.pitch;
    yaw = pos.yaw;
}

void Position::scalarMult(double factor) {
    X = X * factor;
    Y = Y * factor;
    Z = Z * factor;
    roll = roll * factor;
    pitch = pitch * factor;
    yaw = yaw * factor;
}

void Position::operator*=(const double& multiplier) {
    X = X * multiplier;
    Y = Y * multiplier;
    Z = Z * multiplier;
    roll = roll * multiplier;
    pitch = pitch * multiplier;
    yaw = yaw * multiplier;
}

Position Position::operator*(const double& multiplier) {
    Position product;
    product.set(X, Y, Z, roll, pitch, yaw);
    product *= multiplier;
    return product;
}

void Position::independentScalarMult(double factors[6]) {
    X = X * factors[0];
    Y = Y * factors[1];
    Z = Z * factors[2];
    roll = roll * factors[3];
    pitch = pitch * factors[4];
    yaw = yaw * factors[5];
}

void Position::operator-=(const Position& pos) {
    X = X - pos.X;
    Y = Y - pos.Y;
    Z = Z - pos.Z;
    roll = roll - pos.roll;
    pitch = pitch - pos.pitch;
    yaw = yaw - pos.yaw;
}

Position Position::operator-(const Position& subtrahend) {
    Position difference;
    difference.set(X, Y, Z, roll, pitch, yaw);
    difference -= subtrahend;
    return difference;
}

void Position::operator+=(const Position& pos) {
    X = X + pos.X;
    Y = Y + pos.Y;
    Z = Z + pos.Z;
    roll = roll + pos.roll;
    pitch = pitch + pos.pitch;
    yaw = yaw + pos.yaw;
}

Position Position::operator+(const Position& addend) {
    Position sum;
    sum.set(X, Y, Z, roll, pitch, yaw);
    sum += addend;
    return sum;
}

double Position::magnitude() {
    double orientation_magnitude = sqrt(roll*roll + pitch*pitch + yaw*yaw) * 100.0;
    double cartesian_magnitude = sqrt(X*X + Y*Y + Z*Z);
    return sqrt(orientation_magnitude*orientation_magnitude + cartesian_magnitude*cartesian_magnitude);
}

_Bool Position::equals(const Position& pos) {
    if (fabs(X - pos.X) > .003)
        return false;
    if (fabs(Y - pos.Y) > .003)
        return false;
    if (fabs(Z - pos.Z) > .003)
        return false;
    if (fabs(roll - pos.roll) > .003)
        return false;
    if (fabs(pitch - pos.pitch) > .003)
        return false;
    if (fabs(yaw - pos.yaw) > .003)
        return false;
    return true;
}

ThreeByOne Position::coord() {
    return ThreeByOne(X, Y, Z);
}
