#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QObject>
#include <unistd.h>
#include <stdio.h>
#include "cyclic_exception.h"

class Repeater : public QObject
{
    Q_OBJECT

	public:
		Repeater() { internalValue = 0; internalName = "No man"; }
		Repeater(int value) { internalValue = value; internalName = ""; }
		Repeater(int value, QString name) { internalValue = value; internalName = name; }
		
		void setName(QString name);
		
		QString getName() const { return internalName; }
		int getValue() const { return internalValue; }
		
	public slots:
		void setValue(int value);
		void simpleRetransmit();
		void simpleIncrement();
		void conditionalIncrement();
		void falseRetransmit(int num);
		void externalRetransmit();
		void secondaryRetransmit();
		void tertiaryRetransmit();
		
	signals:
		void signalRetransmit();
		void signalIncrement();
		void signalConditionalIncrement();
		void signalFalseRetransmit(int num);
		void signalExternalRetransmit();
		void signalSecondaryRetransmit();
		void signalTertiaryRetransmit();

	private:
		int internalValue;
		QString internalName;
};


void Repeater::setName(QString name) {
	internalName = name;
}

void Repeater::setValue(int value) {
	internalValue = value;
}

void Repeater::simpleRetransmit()
{
	emit signalRetransmit();
}

void Repeater::simpleIncrement()
{
	internalValue += 1;
	// printf("%s,%d\n",qPrintable(getName()),internalValue);
	emit signalIncrement();
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