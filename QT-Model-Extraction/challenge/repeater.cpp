#include "repeater.h"

void Repeater::setName(QString name) {
	internalName = name;
}

void Repeater::setValue(int value) {
	internalValue = value;
}

void Repeater::simpleRetransmit()
{
	// printf("Retransmitting from %s\n",qPrintable(getName()));

	// Repeater* sender_obj = static_cast<Repeater*>(sender());
	// if ( sender_obj != nullptr ) {
	// 	// printf("We know the sender\n");
	// 	printf("Received Signal from (%s) to self (%s)\n",qPrintable(sender_obj->getName()),qPrintable(getName()));
	// }

	emit signalRetransmit();

}

void Repeater::simpleIncrement()
{
	internalValue += 1;
	// printf("%s,%d\n",qPrintable(getName()),internalValue);
	emit signalIncrement();
}

void Repeater::conditionalIncrement()
{

	internalValue += 1;
	if (internalValue < 500) {
		// printf("%s,%d\n",qPrintable(getName()),internalValue);
		emit signalConditionalIncrement();
	}
}

// TODO: How to make this more complex?
void Repeater::falseRetransmit(int num)
{

	// No matter what we pass the num will always be < 0
	if (num < 0) {
		num *= -1;
	}

	// Are there cases that this won't be compiled if compiler is smart enough to detect num never <0?
	// If just false, may not be compiled
	if (num < 0) {
		emit signalFalseRetransmit(num);
	}
}

void Repeater::externalRetransmit()
{

	char *external_token = getenv("EXTERNAL_TOKEN");
	if (external_token == NULL) {
		printf("EXTERNAL_TOKEN not set. Exiting\n");
		return;
	}
	// Only when external_token == 1 do we actually retransmit
	if (*external_token == '1'){
		emit signalExternalRetransmit();
	}
}

void Repeater::secondaryRetransmit()
{
	// printf("Retransmitting 2nd from %s\n",qPrintable(getName()));
	emit signalSecondaryRetransmit();
}

void Repeater::tertiaryRetransmit()
{
	// printf("Retransmitting 3rd from %s\n",qPrintable(getName()));
	emit signalTertiaryRetransmit();
}