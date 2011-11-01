#include "InputManager.h"

/* get the singleton instance */
InputManager* InputManager::instance()
{
	static InputManager im;
	return &im;
}

/* constructor */
InputManager::InputManager(void)
{
	// initialize all keys to false
	for(int i = 0; i < 322; i++) {
		keys_[i] = false;
	}
	// initialize all buttons to false
	for(int i = 0; i < 8; i++) {
		mouseButtons_[i] = false;
	}

	onKeyDown_ = NULL;
	onKeyUp_ = NULL;
	onMouseMove_ = NULL;
	onMouseDown_ = NULL;
	onMouseUp_ = NULL;
}

/* handles input events */
void InputManager::handle(SDL_Event* Event)
{
	switch (Event->type)
	{

	// key pressed
	case SDL_KEYDOWN:
		keys_[Event->key.keysym.sym] = true;	// used for polling
		if ( this->onKeyDown_ != NULL )			// used for events
			this->onKeyDown_(Event->key.keysym.sym,Event->key.keysym.mod);
		break;
	// key released
	case SDL_KEYUP:
		keys_[Event->key.keysym.sym] = false;	// used for polling
		if ( this->onKeyUp_ != NULL )			// used for events
			this->onKeyUp_(Event->key.keysym.sym,Event->key.keysym.mod);
		break;
	// mouse moved
	case SDL_MOUSEMOTION:
		if ( this->onMouseMove_ != NULL )		// used for events
			this->onMouseMove_(Event->motion.x, Event->motion.y, Event->motion.xrel, Event->motion.yrel, Event->motion.state );
		break;
	// mouse button down
	case SDL_MOUSEBUTTONDOWN:
		mouseButtons_[Event->button.button] = true;	// used for polling
		if ( this->onMouseDown_ != NULL )			// used for events
			this->onMouseDown_(Event->button.x, Event->button.y, Event->button.button, Event->button.state);
		break;
	// mouse button up
	case SDL_MOUSEBUTTONUP:
		mouseButtons_[Event->button.button] = false;	// used for polling
		if ( this->onMouseUp_ != NULL )					// used for events
			this->onMouseUp_(Event->button.x, Event->button.y, Event->button.button, Event->motion.state);
		break;
	}
}

/* register key down event */
void InputManager::addKeyDownEvent(KeyCallback OnKeyDown )
{
	this->onKeyDown_ = OnKeyDown;
}

/* regsiter key up event */
void InputManager::addKeyUpEvent(KeyCallback OnKeyUp )
{
	this->onKeyUp_ = OnKeyUp;
}

/* register mouse move event */
void InputManager::addMouseMoveEvent(MouseMoveCallback OnMouseMove )
{
	this->onMouseMove_ = OnMouseMove;
}

/* register mouse button down event */
void InputManager::addMouseDownEvent(MouseButtonCallback OnMouseDown )
{
	this->onMouseDown_ = OnMouseDown;
}

/* register mouse button up event */
void InputManager::addMouseUpEvent(MouseButtonCallback OnMouseUp )
{
	this->onMouseUp_ = OnMouseUp;
}