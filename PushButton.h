
#ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

//#include <stdint.h>

class PushButtonGeneric;// forward declaration 

// Add this define prior including pushbutton.h to enable Collection
// #define USE_PushButtonCollection 	1		

// ******************************************************************************************************
//                CLASS PushButtonCollection
// ******************************************************************************************************
#if USE_PushButtonCollection

#ifndef PushButtonCollectionCountMax
	#define PushButtonCollectionCountMax 32
#endif

class PushButtonCollectionClass 
{
private:	
	uint8_t count;		
	PushButtonGeneric *childs[PushButtonCollectionCountMax];
public:	
	// constructor
	PushButtonCollectionClass() {count=0;index=0;}
	bool registerChild(PushButtonGeneric *dev);
	
	void scan();
	uint8_t getCount() { return count;}
};

extern PushButtonCollectionClass PushButtonCollection;
#endif  //USE_PushButtonCollection

// ******************************************************************************************************
//                CLASS PushButtonGeneric
// ******************************************************************************************************
class PushButtonGeneric {
public:
		PushButtonGeneric(const char *_name, bool logic_low=true):name (_name)	{
			pressedStartTime=0L; 
			debounceDelayTime=0u;
			clear();			
			state = 0;			
			#if USE_PushButtonCollection
			// register myself to collection			
			PushButtonCollection.registerChild(this);
			#endif		
				
		}

		// setters/getters
		void setDebounceDelay(uint16_t debounceDelay) {debounceDelayTime = debounceDelay;}
		const char *getName() {return name;} 
		uint32_t getState() {return state;}
		uint32_t getPressedState() {return pressedState;}

		//bool operator==(PushButtonGeneric &rhs) {return (this == &rhs);} // Compare a button object against this

		// get current state & time (useful to monitor in real time the button)
		inline bool isPressed() { return (state!=0);}     

    virtual void scan() {}
		void updateState(uint32_t newState);
		
		// get past/history state & time (useful to catch action related to button)
    inline uint8_t wasPressed() {return buttonClicked;}			// return the number of press
		inline uint16_t lastHoldTime() {return wasPressed()?pressedDuration:0L;}
		inline uint16_t heldTime() {return (millis() - pressedStartTime); }
		inline uint16_t holdTime() {return isPressed()?heldTime():0L;}
		inline bool heldFor(unsigned long time) { return (holdTime() > time)?true:false;}
		uint16_t getAndClear(); // return lastHoldTime and clear state of button
		void clear() {buttonClicked=0u;	pressedDuration=0u;pressedState=0uL;}

protected:
		uint8_t buttonClicked; 
    uint32_t pressedStartTime;
		uint16_t pressedDuration;  
		uint32_t pressedState;
    uint16_t debounceDelayTime; // max 65536 ms	
		
private:
		uint32_t state;
			// State is always interpreted as high level logic 			
			//    keyPressed is true when button exhibits a high level			
			//    state=0 means that no keys are pressed
			//    state!=0  means that one keys is pressed
			// when a single key is used, be sure to toggle between 0 and -1
		
		const char * name;	
};


// ******************************************************************************************************
//                CLASS PushButton
// ******************************************************************************************************
class PushButton: public PushButtonGeneric {
public:

  // Constructor
	PushButton(uint8_t buttonPin, const char *_name="PIN", bool logic_low=true):PushButtonGeneric(_name)  {
		pin = buttonPin;		
		pinMode(pin, (logic_low)?INPUT_PULLUP:INPUT);								
		logicLow = logic_low;
	}

	void scan() override;    		
	uint8_t getPin(void) const   { return pin; }
    		
private:
  uint8_t pin;
	bool logicLow;
};



#endif
// PUSHBUTTON_H
