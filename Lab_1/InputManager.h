#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "SDL.h"

typedef void (*KeyCallback)( SDLKey, SDLMod );									// key, mod
typedef void (*MouseMoveCallback)( Uint16, Uint16, Sint16, Sint16, Uint8 );		// x, y, relx, rely, state
typedef void (*MouseButtonCallback)( Uint16, Uint16, Uint8, Uint8 );			// x, y, button, state

/*	InputManager
 *
 *	Singleton class that handles input.
 *
 *	For polling, use getKeys() and getMouseButtons() to get an array of booleans representing
 *	the state of the mouse buttons or keys (true if pressed, false otherwise).
 *
 *	For using event callbacks, call the methods addKeyDownEvent(), addMouseMoveEvent(), etc...
 *
 *	InputManager.handle() method must always be called in the game loop.
 */
class InputManager
{
private:
	// polling variables
	bool keys_[322];		// 322 to handle enough keys on the keyboard
	bool mouseButtons_[8];	// handles all mouse buttons incl. wheel
	int lastMouseX;		// last mouse x
	int lastMouseY;		// last mouse y

	// event callbacks
	KeyCallback onKeyDown_;
	KeyCallback onKeyUp_;
	MouseMoveCallback onMouseMove_;
	MouseButtonCallback onMouseDown_;
	MouseButtonCallback onMouseUp_;

protected:
	InputManager(void);

public:
	//~InputManager(void);
	
	// singleton
	static InputManager* instance();

	// handle input
	void handle(SDL_Event* Event);
	void handleMouse();

	// used for polling
	inline bool* getKeys() { return this->keys_; }
	inline bool* getMouseButtons() { return this->mouseButtons_; }
	inline int	getLastMouseX() { return this->lastMouseX; }
	inline int	getLastMouseY() { return this->lastMouseY; }
	void getMouseAngle( float *angle_y, float *angle_z );

	// events
	void addKeyDownEvent( KeyCallback OnKeyDown );
	void addKeyUpEvent( KeyCallback OnKeyUp );
	void addMouseMoveEvent( MouseMoveCallback OnMouseMove );
	void addMouseDownEvent( MouseButtonCallback OnMouseDown );
	void addMouseUpEvent( MouseButtonCallback OnMouseUp );
};

#endif