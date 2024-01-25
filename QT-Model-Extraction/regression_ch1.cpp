#include "repeater.h"
#include "cyclic_exception.h"
#include "regression_ch1.h"
#include "expert_ch1.h"
#include <unistd.h>
#include <stdio.h>

// Testing framework relies on the assumption that should a command fail, it should generate the CustomCyclicException

// We should not be catching the exception (except for test 0) since we are not generating cycles

// Check that throwing CustomCyclicException works
int regression_test_0(){
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

// Regression Test 1: Initializing just an object (stack) with no connections should not be an issue
int regression_test_1() {
	
	// Expected command to execute without issue
	try {
		// Scenario Setup
		Repeater a(0,"a");
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

// Regression Test 2: Initializing just an object (heap) with no connections should not be an issue
int regression_test_2() {

	Repeater* b = new Repeater(0,"b");

	// Expected command to execute without issue
	try {
		// Scenario Setup
		b->simpleRetransmit();
		throw "Didn't catch cyclic behavior\n";

	} catch (CustomCyclicException& cce) {
		// Caught expected exception
		// printf("We have caught our custom exception\n");
		// printf("%s\n",cce.what());
		return 0;

	} catch (const char* msg) {
		// Didn't catch expected exception
		// printf("%s\n",msg);

		// Clean up heap usage
		b->QObject::~QObject();
		return 1;
	}

	// Something went wrong, expected exit in try/catch block
	return -1;
}

// Regression Test 3: Initializing just an object (heap) with no connections and deleting should not be an issue
int regression_test_3() {
	
	// Expected command to execute without issue
	try {
		// Scenario Setup
		Repeater* b = new Repeater(0,"b");
		b->QObject::~QObject();
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

// Regression Test 4: Connect two objects in one direction
int regression_test_4() {
	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");
	
	// Expected command to execute without issue
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		b.simpleRetransmit();
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

// Regression Test 5: Disconnect before a cycle forms (Disconnect specific)
int regression_test_5() {
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
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);
		QObject::connect(&d, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);
		QObject::connect(&e, &Repeater::signalRetransmit, &f, &Repeater::simpleRetransmit);
		QObject::connect(&f, &Repeater::signalRetransmit, &g, &Repeater::simpleRetransmit);
		QObject::connect(&g, &Repeater::signalRetransmit, &h, &Repeater::simpleRetransmit);


		QObject::disconnect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&h, &Repeater::signalRetransmit, &a, &Repeater::simpleRetransmit);
		b.simpleRetransmit();
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

// Regression Test 6: Disconnect before a cycle forms (Disconnect sender)
int regression_test_6() {
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
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&a, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&a, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);
		QObject::connect(&a, &Repeater::signalRetransmit, &g, &Repeater::simpleRetransmit);

		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);
		QObject::connect(&d, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);
		QObject::connect(&e, &Repeater::signalRetransmit, &f, &Repeater::simpleRetransmit);
		QObject::connect(&f, &Repeater::signalRetransmit, &g, &Repeater::simpleRetransmit);
		QObject::connect(&g, &Repeater::signalRetransmit, &h, &Repeater::simpleRetransmit);

		a.simpleRetransmit();

		QObject::disconnect(&a, &Repeater::signalRetransmit, NULL, NULL);
		QObject::connect(&h, &Repeater::signalRetransmit, &a, &Repeater::simpleRetransmit);
		a.simpleRetransmit();
		b.simpleRetransmit();
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

// Regression Test 7: Disconnect before a cycle forms (Disconnect sender, all signals)
int regression_test_7() {
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
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&a, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&a, &Repeater::signalSecondaryRetransmit, &e, &Repeater::simpleRetransmit);
		QObject::connect(&a, &Repeater::signalSecondaryRetransmit, &g, &Repeater::simpleRetransmit);

		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);
		QObject::connect(&d, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);
		QObject::connect(&e, &Repeater::signalRetransmit, &f, &Repeater::simpleRetransmit);
		QObject::connect(&f, &Repeater::signalRetransmit, &g, &Repeater::simpleRetransmit);
		QObject::connect(&g, &Repeater::signalRetransmit, &h, &Repeater::simpleRetransmit);

		a.simpleRetransmit();
		a.secondaryRetransmit();

		QObject::disconnect(&a, NULL, NULL, NULL);
		QObject::connect(&h, &Repeater::signalRetransmit, &a, &Repeater::simpleRetransmit);
		a.simpleRetransmit();
		b.simpleRetransmit();
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

// Regression Test 8: Destroy repeater before cycle forms
int regression_test_8() {
	// Scenario Setup
	Repeater a(0,"a");
	Repeater* b = new Repeater(0,"b");
	Repeater c(0,"c");
	Repeater d(0,"d");
	Repeater e(0,"e");

	// Expected command to execute without issue
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, b, &Repeater::simpleRetransmit);
		QObject::connect(b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);
		QObject::connect(&d, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);

		b->QObject::~QObject();		
		QObject::connect(&e, &Repeater::signalRetransmit, &a, &Repeater::simpleRetransmit);
		c.simpleRetransmit();
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

// Regression Test 9: Iterate through single repeater but not on actual SIG/SLOT cycle
int regression_test_9() {
	// Scenario Setup
	Repeater a(0,"a");

	// Expected command to execute without issue
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, &a, &Repeater::secondaryRetransmit);
		QObject::connect(&a, &Repeater::signalSecondaryRetransmit, &a, &Repeater::tertiaryRetransmit);
		a.simpleRetransmit();
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

// Regression Test 10: Iterate through multiple repeaters but not on actual SIG/SLOT cycle
int regression_test_10() {
	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");

	// Expected command to execute without issue
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&b, &Repeater::signalRetransmit, &a, &Repeater::secondaryRetransmit);
		QObject::connect(&a, &Repeater::signalSecondaryRetransmit, &b, &Repeater::secondaryRetransmit);
		QObject::connect(&b, &Repeater::signalSecondaryRetransmit, &a, &Repeater::tertiaryRetransmit);
		QObject::connect(&a, &Repeater::signalTertiaryRetransmit, &b, &Repeater::tertiaryRetransmit);
		a.simpleRetransmit();
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

// Regression Test 10: Add connections between two chains
int regression_test_11() {
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
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);

		QObject::connect(&e, &Repeater::signalRetransmit, &f, &Repeater::simpleRetransmit);
		QObject::connect(&f, &Repeater::signalRetransmit, &g, &Repeater::simpleRetransmit);
		QObject::connect(&g, &Repeater::signalRetransmit, &h, &Repeater::simpleRetransmit);

		QObject::connect(&d, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);
		a.simpleRetransmit();
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

void execute_regression_suite() {
	int test_count = 15;
	int passed = 0;
	int tested = 0;
	int results[test_count];
	// Unimplemented
	results[0] = regression_test_0(); 
	results[1] = regression_test_1(); 
	results[2] = regression_test_2(); 
	results[3] = regression_test_3(); 
	results[4] = regression_test_4(); 
	results[5] = regression_test_5(); 
	results[6] = regression_test_6(); 
	results[7] = regression_test_7(); 
	results[8] = regression_test_8(); 
	results[9] = regression_test_9(); 
	results[10] = regression_test_10(); 
	results[11] = regression_test_11(); 

	// Expert tests that test false positives
	results[12] = expert_test_1();
	results[13] = expert_test_2();
	results[14] = expert_test_4();
	

	for (int i=0;i<test_count;i++) {
		if (results[i] == 1) {
			// printf("Reg Test %d Passed\n",i);
			passed += 1;
			tested += 1;
		} else if (results[i] == 0) {
			// printf("Reg Test %d Failed\n",i);
			tested += 1;
		} else {
			// printf("Reg Test %d Unimplemented/Shouldn't reach this point\n",i);
		}
	}

	FILE *fp;
	fp = fopen("regression_results.json", "w");
	fprintf(fp, "{\"RegressionPassed\": %d, \"RegressionTotal\": %d}",passed,tested);
	fclose(fp);

	return;
}
