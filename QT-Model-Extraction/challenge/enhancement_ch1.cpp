#include "repeater.h"
#include "cyclic_exception.h"
#include "enhancement_ch1.h"
#include "expert_ch1.h"
#include <unistd.h>
#include <stdio.h>

// NOTE: Testing framework relies on the assumption that should a command fail, it should generate the CustomCyclicException

// We should be catching the exception since we are generating cycles

// Check that throwing CustomCyclicException works
int enhancement_test_0(){
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

// Enhancement Test 1: Cyclic SIG/SLOT on single Repeater of itself upon connect
int enhancement_test_1() {
	
	// Scenario Setup
	Repeater a(0,"a");
	
	// Expected command to fail and throw out exception
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, &a, &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleRetransmit();

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

// Enhancement Test 2: Cyclic SIG/SLOT on Repeater chain (4) after last connection
int enhancement_test_2() {

	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");
	Repeater d(0,"d");
	
	// Expected command to fail and throw out exception
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);
		QObject::connect(&d, &Repeater::signalRetransmit, &a, &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleRetransmit();

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

// Enhancement Test 3: Cyclic SIG/SLOT formed and then broken (3,0) by disconnecting part of the cycle. Regardless should throw exception moment cyclic is formed
int enhancement_test_3() {

	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");

	// Expected command to fail and throw out exception
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &a, &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleRetransmit();

		// Even if we disconnect, the cycle should have already been formed previously
		QObject::disconnect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
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

	return -1;
}

// Enhancement Test 4: Cyclic SIG/SLOT formed and then broken (3,0) by destroying an object in the cycle. Regardless should throw exception moment cyclic is formed
int enhancement_test_4() {

	// Scenario Setup
	Repeater a(0,"a");
	Repeater* b = new Repeater(0,"b");
	Repeater c(0,"c");

	// Expected command to fail and throw out exception
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, b, &Repeater::simpleRetransmit);
		QObject::connect(b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &a, &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleRetransmit();

		// Even if we destroy, the cycle should have already been formed previously
		b->QObject::~QObject();
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

	return -1;
}

// Enhancement Test 5: Cyclic SIG?SLOT formed within a downstream component (a,b,c,d,e,c)
int enhancement_test_5() {
	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");
	Repeater d(0,"d");
	Repeater e(0,"e");

	// Expected command to fail and throw out exception
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);
		QObject::connect(&d, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);
		QObject::connect(&e, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleRetransmit();
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

// Enhancement Test 6: Cyclic SIG/SLOT formed within a downstream component (a,b,c,d,e,c) and non cyclic object is disconnected
int enhancement_test_6() {
	
	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");
	Repeater d(0,"d");
	Repeater e(0,"e");

	// Delete upstream connection 
	QObject::disconnect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);

	// Expected command to fail and throw out exception
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);
		QObject::connect(&d, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);
		QObject::connect(&e, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleRetransmit();
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

// Enhancement Test 7: Destroy an object that isn't in the planned loop (a,b,c,d,e,c) and non cyclic object is destroyed
int enhancement_test_7() {
	
	// Scenario Setup
	Repeater a(0,"a");
	Repeater* b = new Repeater(0,"b");
	Repeater c(0,"c");
	Repeater d(0,"d");
	Repeater e(0,"e");

	// Expected command to fail and throw out exception
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, b, &Repeater::simpleRetransmit);
		QObject::connect(b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);
		QObject::connect(&d, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);
		b->QObject::~QObject();

		QObject::connect(&e, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleRetransmit();
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

// Enhancement Test 8: Large Cyclic SIG/SLOT
int enhancement_test_8() {

	// Scenario Setup
	// Initialize Repeater Array
	Repeater repeater_array[5000];

	for (int i=0;i<5000;i++) {
		repeater_array[i].setName(QString::number(i));
	}

	try {
		// Initialize 0-4999 connections
		for (int i=0;i<4999;i++) {
			QObject::connect(&repeater_array[i], &Repeater::signalRetransmit, &repeater_array[i+1], &Repeater::simpleRetransmit);
		}

		QObject::connect(&repeater_array[4999], &Repeater::signalRetransmit, &repeater_array[0], &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// repeater_array[0].simpleRetransmit();
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

// Enhancement Test 9: Single Connection forms multiple cycles. Need to detect at least one
int enhancement_test_9() {
	// Scenario Setup
	// Initialize Repeater Array
	Repeater repeater_array[5000];

	// Initialize Objects
	for (int i=0;i<5000;i++) {
		repeater_array[i].setName(QString::number(i));
	}

	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");

	try {
		// Initialize 0-4999 connections
		for (int i=0;i<4999;i++) {
			QObject::connect(&repeater_array[i], &Repeater::signalRetransmit, &repeater_array[i+1], &Repeater::simpleRetransmit);
		}

		// Initialize smaller almost cycle
		QObject::connect(&repeater_array[0], &Repeater::signalRetransmit, &a, &Repeater::simpleRetransmit);
		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);
		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &repeater_array[4999], &Repeater::simpleRetransmit);


		QObject::connect(&repeater_array[4999], &Repeater::signalRetransmit, &repeater_array[0], &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// repeater_array[0].simpleRetransmit();
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

// Enhancement Test 10: Form self-cycle on single repeater but multiple steps
int enhancement_test_10() {
		
	// Scenario Setup
	Repeater a(0,"a");
	
	// Expected command to fail and throw out exception
	try {
		QObject::connect(&a, &Repeater::signalRetransmit, &a, &Repeater::secondaryRetransmit);
		QObject::connect(&a, &Repeater::signalSecondaryRetransmit, &a, &Repeater::tertiaryRetransmit);
		QObject::connect(&a, &Repeater::signalTertiaryRetransmit, &a, &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleRetransmit();

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

// Enhancement Test 11: Go through an object twice, forming cycle on second pass
int enhancement_test_11() {
	// Scenario Setup
	Repeater a(0,"a");
	Repeater b(0,"b");
	Repeater c(0,"c");
	Repeater d(0,"d");
	Repeater e(0,"e");

	try {
		
		QObject::connect(&b, &Repeater::signalRetransmit, &c, &Repeater::simpleRetransmit);
		QObject::connect(&c, &Repeater::signalRetransmit, &d, &Repeater::simpleRetransmit);
		QObject::connect(&d, &Repeater::signalRetransmit, &e, &Repeater::simpleRetransmit);
		QObject::connect(&e, &Repeater::signalRetransmit, &a, &Repeater::secondaryRetransmit);
		QObject::connect(&a, &Repeater::signalSecondaryRetransmit, &e, &Repeater::secondaryRetransmit);
		QObject::connect(&e, &Repeater::signalSecondaryRetransmit, &a, &Repeater::simpleRetransmit);

		QObject::connect(&a, &Repeater::signalRetransmit, &b, &Repeater::simpleRetransmit);

		// Executing the following will infinitely cycle until seg fault
		// a.simpleRetransmit();
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


void execute_enhancement_suite() {
	int test_count = 14;
	int passed = 0;
	int tested = 0;
	int results[test_count];

	results[0] = enhancement_test_0(); 
	results[1] = enhancement_test_1(); 
	results[2] = enhancement_test_2(); 
	results[3] = enhancement_test_3(); 
	results[4] = enhancement_test_4(); 
	results[5] = enhancement_test_5(); 
	results[6] = enhancement_test_6(); 
	results[7] = enhancement_test_7(); 
	results[8] = enhancement_test_8(); 
	results[9] = enhancement_test_9(); 
	results[10] = enhancement_test_10();
	results[11] = enhancement_test_11();

	// Expert tests that are a true positive
	results[12] = expert_test_3();
	results[13] = expert_test_5();
	

	for (int i=0;i<test_count;i++) {
		if (results[i] == 1) {
			// printf("Enh Test %d Passed\n",i);
			passed += 1;
			tested += 1;
		} else if (results[i] == 0) {
			// printf("Enh Test %d Failed\n",i);
			tested += 1;
		} else {
			// printf("Enh Test %d Unimplemented/Shouldn't reach this point\n",i);
		}
	}
	// For testing purposes
	FILE *fp;
	fp = fopen("enhancement_results.json", "w");
	fprintf(fp, "{\"AcceptancePassed\": %d, \"AcceptanceTotal\": %d}",passed,tested);
	fclose(fp);

	return;
}
