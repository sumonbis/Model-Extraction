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