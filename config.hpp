#ifndef HEXA_CONFIG
#define HEXA_CONFIG

	#define DILLON
	
	#ifdef DANNY	

		#define PWM_PINS {{2, 3, 4}, {5, 6, 7}, {8, 9, 10},  {11, 12, 13}, {14, 15, 18}, {19, 22, 23}}
		#define ZERO_POINTS {{1.87, 2.30, 2.8}, {2, 2.3, 2.68}, {1.8, 2.2, 2.7}, {2.2, 2.2, 2.5}, {1.93, 2.4, 2.8}, {2.1, 2.1, 2.63}}
		#define MAX_POS {{PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}}
		#define MIN_POS {{-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}}
		#define SCALE_FACT {{0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}}

	#endif

	#ifdef ZACK

		#define PWM_PINS {{2, 3, 4}, {5, 6, 7}, {8, 9, 10},  {11, 12, 13}, {14, 15, 18}, {19, 22, 23}}
		#define ZERO_POINTS {{2.3, 2.6, 2.45}, {2.3, 2.7, 2.05}, {2.85, 2.4, 2.3}, {2.2, 1.85, 2.35}, {2.68, 2.05, 2.3}, {2.25, 2.3, 2.65}}
		#define MAX_POS {{PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}}
		#define MIN_POS {{-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}}
		#define SCALE_FACT {{0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}}
		
	#endif

	#ifdef DILLON

		#define PWM_PINS {{5, 6, 7}, {2, 3, 4}, {8, 9, 10},  {11, 12, 13}, {14, 15, 18}, {19, 22, 23}}
		#define ZERO_POINTS {{1.9, 2.45, 2.7}, {2.05, 2.4, 2.7}, {2.35, 2.7, 2.6}, {2.3, 2.4, 2.65}, {2.1, 2.45, 2.65}, {2.35, 2.45, 2.7}}
		#define MAX_POS {{PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}, {PI, PI, PI}}
		#define MIN_POS {{-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}, {-PI, -PI, -PI}}
		#define SCALE_FACT {{0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}}
		
	#endif

#endif
