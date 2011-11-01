#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "Resource.h"

#include <stack>
#include <vector>

class ResourceManager {
private:
	std::stack<unsigned int> handles_;	// list of free handles
	std::vector<Resource*> *list_;				// list of pointers to resources
	// constructor
	ResourceManager();
	// destructor
	~ResourceManager();
public:
	static ResourceManager* instance();
	
	// returns a pointer to the vector of resources
	inline std::vector<Resource*>* getList() { return this->list_; }

	// returns a resource by filename
	Resource* getElement( char* filePath );

	// return a resource by handle
	Resource* getElement( const unsigned int handle );

	// empty the resource list
	void emptyList();

	// remove a given resource by handle
	void removeResource( const unsigned int handle );

	// add a new resource
	template <class T>
	unsigned int addResource( char *filePath ) {
		// check that filename and list are valid
		if ( this->list_ == NULL || filePath == NULL )
			return -1;

		// if element already exists
		Resource* element = this->getElement( filePath );
		if ( element != NULL ) {
			element->incRefCount();
			return element->getHandle();
		}

		// check for available handle or use new handle
		bool handleAvailable = !this->handles_.empty();
		unsigned int handle;
		if ( handleAvailable ) {
			handle = this->handles_.top();
			this->handles_.pop();
		}
		else
			handle = this->list_->size();

		// create the resource
		Resource *resource = (Resource*)( new T( handle, filePath ) );
		/*if ( this->CreateResource != NULL )
			CreateResource( &resource, handle, filePath );
		else
			resource = new T(handle, filePath);
			*/
		// add the resource to the manager
		if ( handleAvailable )
			(*list_)[handle] = resource;
		else
			this->list_->push_back(resource);

		// return the handle
		return handle;
	};

	// overloaded [] operator for easy access
	Resource* operator [](unsigned int handle);
};

#endif
