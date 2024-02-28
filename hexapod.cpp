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
    }
}

void Hexapod::moveLegToPos(int leg_number, int axis_number, double target_position) {
	legs[leg_number - 1].axes[axis_number -1].moveToPos(target_position);
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

	if (!preCheckSafePos(Position pos))
        return 254; // Pre-check fail
	
	// trig stuff is unique to each leg to can't use loop efficientlys
	for (uint8_t i = 0; i < NUM_LEGS; i++) {
		_potential_results[i][0] = pos.X; // + trig stuff with yaw
		_potential_results[i][1] = pos.Y; // + trig stuff with yaw
		_potential_results[i][2] = pos.Z + sin(pos.pitch) * _leg_X_offset[i] + sin(pos.roll) * _leg_Y_offset[i]
	}

	if (!postCheckSafeCoords(_potential_resutls))
		return 255; // Post-check fail

	for (uint8_t i = 0; i < NUM_LEGS; i++) {
		for (uint8_t j = 0; j < NUM_AXES_PER_LEG; j++) {
			_next_leg_pos[i][j] = potential_results[i][j]
		}
	}
}

_Bool Hexapod::preCheckSafePos(Position pos) {
	// this function might be unnecessary but keeping as a placeholder
	return true;
}

_Bool Hexapod::postCheckSafeCoords(double x, double y, double x) {
	// this function should check if legs will be in impossible positions or within the bot
	return true;
}
