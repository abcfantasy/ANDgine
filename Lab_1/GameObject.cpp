#include "GameObject.h"
#include "ResourceManager.h"

GameObject::~GameObject() {
	ResourceManager::instance()->remove( this->model_ );
};

void GameObject::setModel( char *fileName ) {
	this->model_ = ResourceManager::instance()->get<Model>( fileName );
};

void GameObject::render() {
	if( this->model_ != NULL )
		this->model_->render();
};