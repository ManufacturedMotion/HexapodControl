#include "axis.hpp"
#include "leg.hpp"
#include "position.hpp"
#include <stdbool.h>
#include <stdint.h>

#ifndef HEXA_H
#define HEXA_H

	#define NUM_LEGS 6
	class Hexapod {
		public:
			Hexapod();
			void moveLegAxisToPos(uint8_t leg_number, uint8_t axis_number, double target_position);
			_Bool moveLegToPos(uint8_t leg_number, double x, double y, double z);
			void moveToZeros();
			void sit();
			void stand();
			Leg legs[NUM_LEGS];
			void forwardKinematics(double angle0, double angle1, double angle2);
			void rapidMove(double x, double y, double z, double roll, double pitch, double yaw);
			uint8_t linearMoveSetup(double x, double y, double z, double roll, double pitch, double yaw, double target_speed);
			void linearMovePerform();
			void rapidMove(Position next_pos);
		private:
			uint8_t _leg_group_1[NUM_LEGS / 2] = {1,4,6}; // Divide legs into two self-stable groups
			uint8_t _leg_group_2[NUM_LEGS / 2] = {2,3,5};
			double _next_leg_pos[NUM_LEGS][NUM_AXES_PER_LEG];
			double _leg_X_offset[NUM_LEGS] = {-59.09, -108.5, -59.03, 59.03, 108.5, 59.09};
			double _leg_Y_offset[NUM_LEGS] = {-106.04, 0, 106.1, 106.1, 0, -106.04};
			double _home_yaws[NUM_LEGS] = { ((-PI / 2.0) - 1.0),(-PI / 2.0),	((-PI / 2.0) + 1.0), 
											(1.0), 				( PI / 2.0),	(( PI / 2.0) + 1.0)  };
			// double _home_yaws[NUM_LEGS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
			double _max_speed = 1000000;
			_Bool preCheckSafePos(Position pos);
			_Bool postCheckSafeCoords(double x, double y, double z);
			uint8_t inverseKinematics(Position pos);
			uint8_t inverseKinematics(double x, double y, double z, double roll, double pitch, double yaw);
			void moveLegs();
			double _move_progress;
			uint32_t _move_start_time;
			_Bool _moving_flag;
			Position _end_pos;
			Position _start_pos;
			Position _current_pos;
			double _move_time;

	};

#endif
