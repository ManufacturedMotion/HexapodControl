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
			void moveLegToPos(int leg_number, int axis_number, double target_position);
			void moveToZeros();
			void sit();
			void stand();
			Leg legs[NUM_LEGS];
		private:
			uint8_t _leg_group_1[NUM_LEGS / 2] = {1,4,6}; // Divide legs into two self-stable groups
			uint8_t _leg_group_2[NUM_LEGS / 2] = {2,3,5};
			double _next_leg_pos[NUM_LEGS][NUM_AXES_PER_LEG];
			double _leg_X_offset[NUM_LEGS];
			double _leg_Y_offset[NUM_LEGS];
			_Bool preCheckSafePos(Position pos)
			_Bool postCheckSafeCoords(double x, double y, double x)

	};

#endif
