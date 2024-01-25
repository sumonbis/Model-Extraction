#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QObject>

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
