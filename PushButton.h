
#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <stdint.h>

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
		PushButtonGeneric(const char *name):name (_name)	{
			pressedStartTime=0L; 
			debounceDelayTime=0u;
			clear();
			state = -1;			
			#if USE_PushButtonCollection
			// register myself to collection			
			PushButtonCollection.registerChild(this);
			#endif			
		}

		// setters/getters
		void setDebounceDelay(unsigned int debounceDelay) {debounceDelayTime = debounceDelay;}
		const char *getName() {return name;} 

		//bool operator==(PushButtonGeneric &rhs) {return (this == &rhs);} // Compare a button object against this

		// get current state & time (useful to monitor in real time the button)
		inline bool isPressed() { return (state!=-1);}     

    virtual void scan() {}
		void updateState(uint8_t newState);
		
		// get past/history state & time (useful to catch action related to button)
    inline uint8_t wasPressed() {return buttonClicked;}			// return the number of press
		inline unsigned long lastHoldTime() {return wasPressed()?pressedDuration:0L;}
		inline unsigned long heldTime() {return (millis() - pressedStartTime); }
		inline unsigned long holdTime() {return isPressed()?heldTime():0L;}
		inline bool heldFor(unsigned long time) { return (holdTime() > time)?true:false;}
		unsigned long getAndClear(); // return lastHoldTime and clear state of button
		void clear() {buttonClicked=0;	pressedDuration=0L;}
protected:
		uint8_t buttonClicked; 
    unsigned long pressedStartTime,pressedDuration;  
    unsigned int debounceDelayTime; // max 65536 ms	
private:
		uint8_t state;
			// State is always interpreted as low level logic, ie keyPressed is true when button exhibits a low level			
			// state=0 means that all keys are pressed
			// state=-1 (0xff) means that all keys are released
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
