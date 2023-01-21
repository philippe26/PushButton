#include <Arduino.h>
#include "PushButton.h"


// ******************************************************************************************************
//                CLASS PushButtonGeneric
// ******************************************************************************************************
unsigned long PushButtonGeneric::getAndClear()
{
  unsigned long held = pressedDuration;
  if (!wasPressed()) return 0L;  
  clear();
  return held;
}

void PushButtonGeneric::updateState(uint8_t newState)
{	
	if (newState!=state) {
		//state has changed
		if (isPressed()) {    
			//button just pressed , wait for release and count duration
			//buttonPressed = true;
			pressedStartTime = millis();
	 	} else {
		 // Button released, compute duration 
		 if (heldTime()>debounceDelayTime) {
		 		pressedDuration = heldTime();		 
					//buttonPressed=false;
				buttonClicked++;
		 }
	 }
	 state = newState;	 
	}
}

// ******************************************************************************************************
//                CLASS PushButton
// ******************************************************************************************************
void PushButton::scan()
{
	uint8_t read=digitalRead(pin);	 
	uint8_t state; 
	if (logicLow) 
	 	state = (read==LOW)?0:-1;
	else 
		state = (read==HIGH)?0:-1;	
	 
	updateState(state);
}

// ******************************************************************************************************
//                CLASS PushButtonCollectionClass
// ******************************************************************************************************

#if USE_PushButtonCollection
void PushButtonCollectionClass::scan()
{
	for(uint8_t ndx=0; ndx<count; ndx++)
		childs[ndx]->scan();    
}

bool PushButtonCollectionClass::registerChild(PushButtonGeneric *dev) 
{
	if (count<PushButtonCollectionCountMax) {
			childs[count++]=dev;
			return true;
	} 
	return false;
}
#endif
