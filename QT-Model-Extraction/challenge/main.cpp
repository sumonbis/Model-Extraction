#include <iostream>
#include "repeater.h"
#include "enhancement_ch1.h"
#include "regression_ch1.h"
#include "expert_ch1.h"

#include <stdlib.h>
#include <unistd.h>  

int main(int argc , char **argv) {

	if (argc != 2 || atoi(argv[1]) < 0 || atoi(argv[1]) > 1) {
		printf("Usage: ./repeater < 0 | 1 >\n");
		return 1;
	}

	// Each of these tests should run without issue, not detecting any cycles
	if (atoi(argv[1]) == 0) {
		printf("Executing E4_CH1 Regression Tests\n");
		execute_regression_suite();
		return 0;
	}

	// Each of these tests should detect a cycle and throw the custom CCE exception provided
	if (atoi(argv[1]) == 1) {
		printf("Executing E4_CH1 Enhancement Tests\n");
		execute_enhancement_suite();
		return 0;
	}

	return 1;
}
