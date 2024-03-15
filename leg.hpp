#include "axis.hpp"
#include "config.hpp"
#include <stdbool.h>
#include <stdint.h>

#ifndef HEXA_LEG
#define HEXA_LEG

	#define NUM_AXES_PER_LEG 3

	class Leg {
		public:
			Leg();
			void initializeAxes(uint8_t leg_number);
			double current_angles[NUM_AXES_PER_LEG];
			double current_cartesian[NUM_AXES_PER_LEG];
			_Bool rapidMove(double x,  double y, double z);
			_Bool linearMove(double x,  double y, double z, double speed);
			Axis axes[NUM_AXES_PER_LEG];
			void setHomeYaw(double home_yaw);
		private:
			uint8_t _leg_number;
			double _length0 = 63.00;
			double _length1 = 92.00;
			double _length2 = 157.5;
			double _home_yaw;
			void moveAxes();
			_Bool checkSafeCoords(double x, double y, double z);
			_Bool inverseKinematics(double x, double y, double z);
			double _next_angles[NUM_AXES_PER_LEG];
			double _next_cartesian[NUM_AXES_PER_LEG];
	};

#endif
