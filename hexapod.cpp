#include "leg.hpp"
#include "axis.hpp"
#include "hexapod.hpp"
#include "position.hpp"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

Hexapod::Hexapod() { 
    for (uint8_t i = 1; i <= NUM_LEGS; i++) {
        legs[i - 1].initializeAxes(i);
		legs[i - 1].setHomeYaw(_home_yaws[i - 1]);
    }
}

void Hexapod::moveLegAxisToPos(uint8_t leg_number, uint8_t axis_number, double target_position) {
	legs[leg_number - 1].axes[axis_number -1].moveToPos(target_position);
}

_Bool Hexapod::moveLegToPos(uint8_t leg_number, double x, double y, double z) {
	return legs[leg_number - 1].rapidMove(x, y, z);
}
            
void Hexapod::moveToZeros() {
	for (uint8_t i = 1; i <= NUM_LEGS; i++) {
		for (uint8_t j = 1; j <= NUM_AXES_PER_LEG; j++) {
			legs[i - 1].axes[j - 1].moveToPos(0);
		}        
    }
} 

void Hexapod::sit() {

}
            
void Hexapod::stand() {
	for (uint8_t i = 1; i <= NUM_LEGS; i++) {
		legs[i - 1].axes[0].moveToPos(0);
		legs[i - 1].axes[1].moveToPos(1);
		legs[i - 1].axes[2].moveToPos(0.75);
	}
}

_Bool Hexapod::linearMove(Position next_pos) {
    return true;
}

uint8_t Hexapod::inverse_kinematics(Position pos) {
	// Just to get something workign assume yaw = 0 
	// Must rework leg IK or set points align all coordinate systems
	// Right now we have 6 coordinate systems harder than we want to do
	// Even if we did like 2 coordinate systems (left / right) would be a big help for this
	// Discuss with Danny/Dillon best way to accomplish this as there are many confusing effects of either way 

	if (!preCheckSafePos(pos))
        return 254; // Pre-check fail
	
	double potential_results[NUM_LEGS][3];
	// trig stuff is unique to each leg to can't use loop efficientlys
	for (uint8_t i = 0; i < NUM_LEGS; i++) {
		// if (i < 3) {
		// 	potential_results[i][0] = -1.0 * (pos.X * cos(pos.yaw) - pos.Y * sin(pos.yaw)); // + trig stuff with yaw
		// 	potential_results[i][1] = -1.0 * (pos.Y * cos(pos.yaw) + pos.X * sin(pos.yaw)); // + trig stuff with yaw
		// 	potential_results[i][2] = pos.Z + sin(pos.pitch) * _leg_X_offset[i] + sin(pos.roll) * _leg_Y_offset[i];
		// }
		// else {
			potential_results[i][0] = pos.X * cos(pos.yaw) - pos.Y * sin(pos.yaw); // + trig stuff with yaw
			potential_results[i][1] = pos.Y * cos(pos.yaw) + pos.X * sin(pos.yaw); // + trig stuff with yaw
			potential_results[i][2] = pos.Z + sin(pos.pitch) * _leg_X_offset[i] + sin(pos.roll) * _leg_Y_offset[i];
		// }
	}

	for (uint8_t i = 0; i < NUM_LEGS; i++) {
		if (!postCheckSafeCoords(potential_results[i][0], potential_results[i][1], potential_results[i][2]))
			return 255; // Post-check fail
	}
	for (uint8_t i = 0; i < NUM_LEGS; i++) {
		for (uint8_t j = 0; j < NUM_AXES_PER_LEG; j++) {
			_next_leg_pos[i][j] = potential_results[i][j];
		}
	}
	return 0;
}

_Bool Hexapod::preCheckSafePos(Position pos) {
	// this function might be unnecessary but keeping as a placeholder
	return true;
}

_Bool Hexapod::postCheckSafeCoords(double x, double y, double z) {
	// this function should check if legs will be in impossible positions or within the bot
	return true;
}
