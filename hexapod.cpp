#include "leg.hpp"
#include "axis.hpp"
#include "hexapod.hpp"
#include "position.hpp"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "threebythree.hpp"
#include <Arduino.h>

Hexapod::Hexapod() { 
    for (uint8_t i = 1; i <= NUM_LEGS; i++) {
        legs[i - 1].initializeAxes(i);
    }
}

void Hexapod::moveLegAxisToPos(uint8_t leg_number, uint8_t axis_number, double target_position) {
	legs[leg_number - 1].axes[axis_number -1].moveToPos(target_position);
}

_Bool Hexapod::moveLegToPos(uint8_t leg_number, double x, double y, double z) {
	return legs[leg_number - 1].rapidMove(x, y, z);
}

// _Bool Hexapod::moveLegToPos(uint8_t leg_number, double x, double y, double z) {
// 	return legs[leg_number - 1].linearMoveSetup(x, y, z);
// }
            
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

uint8_t Hexapod::inverseKinematics(double x, double y, double z, double roll, double pitch, double yaw) {
	Position pos;
	pos.set(x, y, z, roll, pitch, yaw);
	return inverseKinematics(pos);
}

void Hexapod::rapidMove(double x, double y, double z, double roll, double pitch, double yaw) {
	inverseKinematics(x, y, z, roll, pitch, yaw);
	moveLegs();
	_current_pos.set(x, y, z, roll, pitch, yaw);
}

void Hexapod::rapidMove(Position next_pos) {
	inverseKinematics(next_pos);
	moveLegs();
	_current_pos.setPos(next_pos);
}

uint8_t Hexapod::linearMoveSetup(double x, double y, double z, double roll, double pitch, double yaw, double target_speed) {
	_end_pos.set(x, y, z, roll, pitch, yaw);
	_start_pos.setPos(_current_pos);
	uint8_t retval = 0;
    double speed = target_speed;
    if (target_speed > _max_speed) {
        speed = _max_speed;
        retval = 1; // move speed capped
    }
	_move_progress = 0;
	_move_start_time = millis();
	Position pos_delta = _end_pos - _start_pos;
	_move_time = (fabs(pos_delta.magnitude()) / speed) * 1000; //convert to seconds
	_moving_flag = true;
	return retval;
}

uint8_t Hexapod::linearMoveSetup(Position next_pos, double target_speed) {
  _end_pos.setPos(next_pos); 
  _start_pos.setPos(_current_pos);
  uint8_t retval = 0;
    double speed = target_speed;
    if (target_speed > _max_speed) {
        speed = _max_speed;
        retval = 1; // move speed capped
    }
  _move_progress = 0;
  _move_start_time = millis();
  Position pos_delta = _end_pos - _start_pos;
  _move_time = (fabs(pos_delta.magnitude()) / speed) * 1000; //convert to seconds
  _moving_flag = true;
  return retval;
}


uint8_t Hexapod::legLinearMoveSetup(uint8_t leg, double x,  double y, double z, double target_speed) {

	ThreeByOne coord = ThreeByOne(x, y, z);
	coord.rotateYaw(_home_yaws[leg-1]);
	return legs[leg-1].linearMoveSetup(coord.values[0], coord.values[1], coord.values[2], target_speed);
}

void Hexapod::linearMovePerform() {
	if (isBusy()) {
		double move_progress = (float)(millis() - _move_start_time) / ( _move_time);
		if (move_progress <= 1.0) {
			Position next_pos = (_end_pos - _start_pos) * move_progress + _start_pos;
			rapidMove(next_pos);
		}
		for (uint8_t i = 0; i < NUM_LEGS; i++) {
			if (legs[i].isMoving()) {
				legs[i].linearMovePerform();
			}
		}
		if (move_progress > 1.0) {
			_moving_flag = false;
		}
	}
}

void Hexapod::moveLegs() {
    for (uint8_t i = 0; i < NUM_LEGS; i++) {
		if (!legs[i].isMoving()) {
        	legs[i].rapidMove(_next_leg_pos[i][0], _next_leg_pos[i][1],_next_leg_pos[i][2]);
		}
		
	}
}

_Bool Hexapod::isBusy() {
	for (uint8_t i = 0; i < NUM_LEGS; i++) {
		if (legs[i].isMoving())
			return true;
	}
	return _moving_flag;
}

uint8_t Hexapod::inverseKinematics(Position pos) {
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
			potential_results[i][2] = pos.Z + sin(pos.pitch) * (_leg_X_offset[i] + pos.X) + sin(pos.roll) * (_leg_Y_offset[i] + pos.Y);
		// }
	}

	for (uint8_t i = 0; i < NUM_LEGS; i++) {
		ThreeByOne temp = ThreeByOne(potential_results[i][0], potential_results[i][1], potential_results[i][2]);
		temp.rotateYaw(_home_yaws[i]);
		for (uint8_t j = 0; j < NUM_AXES_PER_LEG; j++) {
			potential_results[i][j] = temp.values[j];
		}
	}

	for (uint8_t i = 0; i < NUM_LEGS; i++) {
		if (!postCheckSafeCoords(potential_results[i][0], potential_results[i][1], potential_results[i][2]))
			return 255; // Post-check fail
	}

	for (uint8_t i = 0; i < NUM_LEGS; i++) {
		for (uint8_t j = 0; j < NUM_AXES_PER_LEG; j++) {
			_next_leg_pos[i][j] = potential_results[i][j];
			
		}
		if (DEBUG) {
			Serial.printf("Leg %d, x:%lf, y:%lf, z%lf\n",i,_next_leg_pos[i][0], _next_leg_pos[i][1], _next_leg_pos[i][2]);
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

void Hexapod::forwardKinematics(double angle0, double angle1, double angle2) {
	ThreeByOne resulting_pos = legs[0].forwardKinematics(angle0, angle1, angle2);
    if (DEBUG) {
		Serial.printf("Result\n  x: %f; y: %f; z: %f\n", resulting_pos.values[0], resulting_pos.values[1], resulting_pos.values[2]);
	}
}

void Hexapod::runSpeed() {
	for (uint8_t i =0; i < NUM_LEGS; i++) {
		for (uint8_t j = 0; j < NUM_AXES_PER_LEG; j++) {
			legs[i].axes[j].runSpeed();
		}
	}
}
