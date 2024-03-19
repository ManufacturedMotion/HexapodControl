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
			_Bool linearMove(Position next_pos);
			void moveLegAxisToPos(uint8_t leg_number, uint8_t axis_number, double target_position);
			_Bool moveLegToPos(uint8_t leg_number, double x, double y, double z);
			void moveToZeros();
			void sit();
			void stand();
			Leg legs[NUM_LEGS];
			void runSpeed();
		private:
			uint8_t _leg_group_1[NUM_LEGS / 2] = {1,4,6}; // Divide legs into two self-stable groups
			uint8_t _leg_group_2[NUM_LEGS / 2] = {2,3,5};
			double _next_leg_pos[NUM_LEGS][NUM_AXES_PER_LEG];
			double _leg_X_offset[NUM_LEGS] = {-59.09, -108.5, -59.03, 59.03, 108.5, 59.09};
			double _leg_Y_offset[NUM_LEGS] = {-106.04, 0, 106.1, 106.1, 0, -106.04};
			double _home_yaws[NUM_LEGS] = { ((-PI / 2.0) - 1.0),(-PI / 2.0),	((-PI / 2.0) + 1.0), 
											(1.0), 				( PI / 2.0),	(( PI / 2.0) + 1.0)  };
			_Bool preCheckSafePos(Position pos);
			_Bool postCheckSafeCoords(double x, double y, double z);
			uint8_t inverse_kinematics(Position pos);

	};

#endif
