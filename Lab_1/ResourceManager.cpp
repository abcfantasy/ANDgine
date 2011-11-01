#include "ResourceManager.h"

ResourceManager::ResourceManager() {
	this->list_ = new std::vector<Resource*>;
}

ResourceManager::~ResourceManager() {
	emptyList();
	delete this->list_;
	this->list_ = NULL;
}

ResourceManager* ResourceManager::instance() {
	static ResourceManager rm;
	return &rm;
};

// returns a resource by filename
Resource* ResourceManager::getElement( char* filePath ) {
	// check that name and path are valid, and list is not empty
	if ( filePath != NULL || this->list_ == NULL || this->list_->empty() )
		return NULL;

	// iterate through the list
	for ( std::vector<Resource*>::iterator i = this->list_->begin(); i != this->list_->end(); i++ ) {
		if ( (*i) != NULL && (*i)->getFilename() == filePath/*&& (*i)->getPath() == path*/ )
			return (*i);
	}

	// return null if not found
	return NULL;
}

Resource* ResourceManager::getElement( const unsigned int handle ) {
	// if handle is valid
	if ( handle < this->list_->size() && handle >= 0 )
		return ( *list_ )[handle];

	// return null if not found
	return NULL;
}

void ResourceManager::emptyList() {
	for ( std::vector<Resource*>::iterator i = this->list_->begin(); i != this->list_->end(); i++ ) {
		delete(*i);
		*i = NULL;
	}

	while (!this->handles_.empty())
		this->handles_.pop();

	// use vector swapping trick to release previously used memory
	this->list_->clear();
	this->list_->swap(*list_);
}

void ResourceManager::removeResource( const unsigned int handle ) {
	// ensure the handle is valid
	if ( handle < 0 || this->list_ == NULL || handle > this->list_->size() || (*list_)[handle] == NULL )
		return;

	Resource* resource = (*list_)[handle];

	// decrement the reference count
	resource->decRefCount();

	// if reference count is 0, destroy it
	if ( resource->getRefCount() == 0 ) {
		// add the handle to the stack of free handles
		this->handles_.push( handle );
		// delete resource
		delete resource;
		// mark slot as NULL
		(*list_)[handle] = NULL;
	}
}

Resource* ResourceManager::operator [](unsigned int handle) {
		if ( handle < this->list_->size() && handle >= 0 )
			return (*list_)[handle];

		return NULL;
	}