#include "repeater.h"
#include "cyclic_exception.h"
#include "expert_ch1.h"
#include <unistd.h>

// NOTE: Testing framework relies on the assumption that should a command fail, it should generate the CustomCyclicException

// Check that throwing CustomCyclicException works
int expert_test_0(){
	try {
		throw CustomCyclicException();
		throw "Bad exception. We missed our CustomCyclicException";

	} catch (CustomCyclicException& cce) {
		// printf("We have caught our custom exception\n");
		// printf("%s\n",cce.what());
		return 1;

	} catch (const char* msg) {
		// printf("%s\n",msg);
		return 0;
	}

	// Something went wrong, expected exit in try/catch block
	return -1;
}

// Expert Test 1: (No Cycle) Psuedo-Cycle of objects/connections however the actual slot will never emit the Signal.
// A simple detection of whether a EMIT SIG exists in a function may not be able to properly detect this.
int expert_test_1(){
	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");
	Repeater d(0,"d");

	// Expected command to execute without issue
	try {
		QObject::connect(&a, &Repeater::signalFalseRetransmit, &b, &Repeater::falseRetransmit);
		QObject::connect(&b, &Repeater::signalFalseRetransmit, &c, &Repeater::falseRetransmit);
		QObject::connect(&c, &Repeater::signalFalseRetransmit, &d, &Repeater::falseRetransmit);

		// While we have connections in a psuedo-cycle, no transmission will result in a cascading signal from it.
		QObject::connect(&d, &Repeater::signalFalseRetransmit, &a, &Repeater::falseRetransmit);
		a.falseRetransmit(0);
		a.falseRetransmit(1);
		a.falseRetransmit(-1);
		throw "Didn't catch cyclic behavior\n";

	} catch (CustomCyclicException& cce) {
		// Caught expected exception
		// printf("We have caught our custom exception\n");
		// printf("%s\n",cce.what());
		return 0;

	} catch (const char* msg) {
		// Didn't catch expected exception
		// printf("%s\n",msg);
		return 1;
	}

	// Something went wrong, expected exit in try/catch block
	return -1;
}

// Expert Test 2: (No Cycle) We have a cycle of objects/connections that send signals to each for a finite amount of iterations (500)
// If assume a signal will always emit from a slot then cases where
int expert_test_2() {
	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");
	Repeater d(0,"d");
	Repeater e(0,"e");
	Repeater f(0,"f");
	Repeater g(0,"g");
	Repeater h(0,"h");

	// Expected command to execute without issue
	try {

		QObject::connect(&a, &Repeater::signalConditionalIncrement, &b, &Repeater::conditionalIncrement);
		QObject::connect(&b, &Repeater::signalConditionalIncrement, &c, &Repeater::conditionalIncrement);
		QObject::connect(&c, &Repeater::signalConditionalIncrement, &d, &Repeater::conditionalIncrement);
		QObject::connect(&d, &Repeater::signalConditionalIncrement, &e, &Repeater::conditionalIncrement);
		QObject::connect(&e, &Repeater::signalConditionalIncrement, &f, &Repeater::conditionalIncrement);
		QObject::connect(&f, &Repeater::signalConditionalIncrement, &g, &Repeater::conditionalIncrement);
		QObject::connect(&g, &Repeater::signalConditionalIncrement, &h, &Repeater::conditionalIncrement);

		// Finite cycle will form and you can execute to see that it eventually ends
		QObject::connect(&h, &Repeater::signalConditionalIncrement, &a, &Repeater::conditionalIncrement);
		a.conditionalIncrement();
		throw "Didn't catch cyclic behavior\n";
	} catch (CustomCyclicException& cce) {
		// Caught expected exception
		// printf("We have caught our custom exception\n");
		// printf("%s\n",cce.what());
		return 0;

	} catch (const char* msg) {
		// Didn't catch expected exception
		// printf("%s\n",msg);
		return 1;
	}

	// Something went wrong, expected exit in try/catch block
	return -1;
}

// Regression Test 3: (Cycle) We retrieve an external environmental variable at compile time. Set to 1, forms a cycle
int expert_test_3() {
	// Scenario Setup
	setenv("EXTERNAL_TOKEN","1",1);
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");

	// Expected command to execute without issue
	try {
		QObject::connect(&a, &Repeater::signalExternalRetransmit, &b, &Repeater::externalRetransmit);
		QObject::connect(&b, &Repeater::signalExternalRetransmit, &c, &Repeater::externalRetransmit);

		// Setting EXTERNAL_TOKEN to 1 properly forms a cycle
		QObject::connect(&c, &Repeater::signalExternalRetransmit, &a, &Repeater::externalRetransmit);
		
		// Executing the following will infinitely cycle until seg fault
		// a.externalRetransmit();
		throw "Didn't catch cyclic behavior\n";
	} catch (CustomCyclicException& cce) {
		// Caught expected exception
		// printf("We have caught our custom exception\n");
		// printf("%s\n",cce.what());
		return 1;

	} catch (const char* msg) {
		// Didn't catch expected exception
		// printf("%s\n",msg);
		return 0;
	}

	// Something went wrong, expected exit in try/catch block
	return -1;
}

// Regression Test 4: (No Cycle) We retrieve an external environmental variable at compile time. Set to 1, forms a cycle
int expert_test_4() {
	// Scenario Setup
	setenv("EXTERNAL_TOKEN","0",1);
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");

	// Expected command to execute without issue
	try {
		QObject::connect(&a, &Repeater::signalExternalRetransmit, &b, &Repeater::externalRetransmit);
		QObject::connect(&b, &Repeater::signalExternalRetransmit, &c, &Repeater::externalRetransmit);

		// Setting EXTERNAL_TOKEN to 0 causes no cycle to form
		QObject::connect(&c, &Repeater::signalExternalRetransmit, &a, &Repeater::externalRetransmit);
		a.externalRetransmit();
		throw "Didn't catch cyclic behavior\n";

	} catch (CustomCyclicException& cce) {
		// Caught expected exception
		// printf("We have caught our custom exception\n");
		// printf("%s\n",cce.what());
		return 0;

	} catch (const char* msg) {
		// Didn't catch expected exception
		// printf("%s\n",msg);
		return 1;
	}

	// Something went wrong, expected exit in try/catch block
	return -1;
}

// Regression Test 5: (Cycle) Compared to Expert Test 2, we have a changing state but never check on it
int expert_test_5() {
	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");
	Repeater d(0,"d");
	Repeater e(0,"e");
	Repeater f(0,"f");
	Repeater g(0,"g");
	Repeater h(0,"h");

	// Expected command to execute without issue
	try {

		QObject::connect(&a, &Repeater::signalIncrement, &b, &Repeater::simpleIncrement);
		QObject::connect(&b, &Repeater::signalIncrement, &c, &Repeater::simpleIncrement);
		QObject::connect(&c, &Repeater::signalIncrement, &d, &Repeater::simpleIncrement);
		QObject::connect(&d, &Repeater::signalIncrement, &e, &Repeater::simpleIncrement);
		QObject::connect(&e, &Repeater::signalIncrement, &f, &Repeater::simpleIncrement);
		QObject::connect(&f, &Repeater::signalIncrement, &g, &Repeater::simpleIncrement);
		QObject::connect(&g, &Repeater::signalIncrement, &h, &Repeater::simpleIncrement);

		// Infinite cycle but changing internal state (change does not affect any condition check)
		QObject::connect(&h, &Repeater::signalIncrement, &a, &Repeater::simpleIncrement);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleIncrement();
		throw "Didn't catch cyclic behavior\n";
	} catch (CustomCyclicException& cce) {
		// Caught expected exception
		// printf("We have caught our custom exception\n");
		// printf("%s\n",cce.what());
		return 1;

	} catch (const char* msg) {
		// Didn't catch expected exception
		// printf("%s\n",msg);
		return 0;
	}

	// Something went wrong, expected exit in try/catch block
	return -1;
}

void execute_expert_suite() {
	int test_count = 6;
	int results[test_count];
	results[0] = expert_test_0(); 
	results[1] = expert_test_1(); 
	results[2] = expert_test_2(); 
	results[3] = expert_test_3(); 
	results[4] = expert_test_4(); 
	results[5] = expert_test_5();
	for (int i=0;i<test_count;i++) {
		if (results[i] == 1) {
			printf("Exp Test %d Passed\n",i);
		} else if (results[i] == 0) {
			printf("Exp Test %d Failed\n",i);
		} else {
			printf("Exp Test %d Unimplemented/Shouldn't reach this point\n",i);
		}
	}
	return;
}