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
 
