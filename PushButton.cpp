#include <Arduino.h>
#include "PushButton.h"


// ******************************************************************************************************
//                CLASS PushButtonGeneric
// ******************************************************************************************************
uint16_t PushButtonGeneric::getAndClear()
{
  uint16_t held = pressedDuration;
  if (!wasPressed()) return 0u;  
  clear();
  return held;
}

void PushButtonGeneric::updateState(uint32_t newState)
{	
	if (newState!=state) {
		//state has changed
		if (newState) {    
			//button just pressed , wait for release and count duration			
			pressedStartTime = millis();
	 	} else {
		 // Button released, compute duration 
		 if (heldTime()>debounceDelayTime) {
		 		pressedDuration = heldTime();
				if (pressedState == state) 					
					buttonClicked++;
				else 
					buttonClicked=1;
				pressedState = state;						
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
	 	state = (read==LOW)?-1:0;
	else 
		state = (read==HIGH)?-1:0;	
	 
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
