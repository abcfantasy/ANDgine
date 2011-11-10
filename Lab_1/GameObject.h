#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Model.h"

// Class that contains information related to a Game Object, be it a static object, a dynamic one, an NPC, etc.
// It will probably be extended by other classes
// Currently it has a Model; it will probably contain animation details, sound files, scripts, etc.
class GameObject {
private:
	// The object's model
	Model *model_;

public:
	// Basic constructors/destructor
	GameObject() : model_( NULL ) {};
	GameObject( GameObject &gameObject ) : model_( gameObject.getModel() ) {};
	GameObject( Model *model ) : model_( model ) {};
	~GameObject();

	// Getters and setters
	inline Model* getModel() { return this->model_; };
	inline void setModel( Model *model ) { this->model_ = model; };
	// Loads the model from a file through the Resource Manager
	void setModel( char *fileName );
	
	// Renders the object's model
	void render();
};

#endif