#include "axis.hpp"

#ifndef HEXA_CONFIG
#define HEXA_CONFIG

	#define DILLON
	
	#ifdef DANNY	
		#define PWM_PINS {{5, 6, 7}, {2, 3, 4}, {8, 9, 10},  {11, 12, 13}, {14, 15, 18}, {19, 22, 23}}
		#define ZERO_POINTS {{2, 2.3, 2.68}, {1.87, 2.30, 2.8}, {1.8, 2.2, 2.7}, {2.2, 2.2, 2.5}, {1.93, 2.4, 2.8}, {2.1, 2.1, 2.63}}
		#define MAX_POS {{PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}}
		#define MIN_POS {{-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}}
		#define SCALE_FACT {{0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}}
		#define REVERSE_AXIS {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}}

	#endif

	#ifdef ZACK
	
		#define PWM_PINS {{5, 6, 7}, {4, 3, 2}, {9, 10, 8},  {12, 13, 11}, {15, 14, 18}, {23, 19, 22}}
		#define ZERO_POINTS {{2.35, 2.4, 1.4}, {2.4, 2.25, 1.5}, {2.3, 2.4, 1.6}, {2.2, 2.35, 1.4}, {1.95, 2.3, 1.53}, {2.1, 2.35, 1.55}}
		#define MAX_POS {{PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}}
		#define MIN_POS {{-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}}
		#define SCALE_FACT {{0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}}
		#define REVERSE_AXIS {{false, false, true}, {false, false, true}, {false, false, true}, {false, false, true}, {false, false, true}, {false, false, true}}
		
	#endif

	#ifdef DILLON
		#define PWM_PINS {{5, 6, 7}, {2, 3, 4}, {8, 9, 10},  {11, 12, 13}, {14, 15, 18}, {19, 22, 23}}
		#define ZERO_POINTS {{1.9, 2.45, 2.7}, {2.35, 2.7, 2.6}, {2.05, 2.4, 2.7}, {2.3, 2.4, 2.65}, {2.1, 2.45, 2.65}, {2.35, 2.45, 2.7}}
		#define MAX_POS {{PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}}
		#define MIN_POS {{-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}}
		#define SCALE_FACT {{0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}}
		#define REVERSE_AXIS {{false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}, {false, false, false}}
	#endif

#endif
