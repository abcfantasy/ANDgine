#include "ResourceManager.h"

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
	emptyList();
}

ResourceManager* ResourceManager::instance() {
	static ResourceManager rm;
	return &rm;
};

// returns a resource by filename
Resource* ResourceManager::find( char* filePath ) {
	// check that name and path are valid, and list is not empty
	if ( filePath != NULL || this->list_.empty() )
		return NULL;

	// iterate through the list
	for ( std::vector<Resource*>::iterator i = this->list_.begin(); i != this->list_.end(); i++ ) {
		if ( (*i) != NULL && (*i)->getFilename() == filePath )
			return (*i);
	}

	// return null if not found
	return NULL;
}

void ResourceManager::emptyList() {
	for ( std::vector<Resource*>::iterator i = this->list_.begin(); i != this->list_.end(); i++ ) {
		delete (*i);
		*i = NULL;
	}

	while (!this->handles_.empty())
		this->handles_.pop();

	// use vector swapping trick to release previously used memory
	this->list_.clear();
	//this->list_.swap(list_);
}

// remove a given resource
void ResourceManager::remove( const unsigned int index ) {
	// ensure the handle is valid
	if ( index < 0 || index > this->list_.size() || list_[index] == NULL )
		return;

	Resource* resource = list_[index];

	// decrement the reference count
	resource->decRefCount();

	// if reference count is 0, destroy it
	if ( resource->getRefCount() == 0 ) {
		// add the handle to the stack of free handles
		this->handles_.push( index );
		// delete resource
		delete resource;
		// mark slot as NULL
		list_[index] = NULL;
	}
}

// remove a given resource by handle
void ResourceManager::remove( Resource *resource ) {
	this->remove( resource->getIndex() );
}

Resource* ResourceManager::operator [](unsigned int handle) {
	if ( handle < this->list_.size() && handle >= 0 )
		return list_[handle];

	return NULL;
}