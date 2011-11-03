#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "Resource.h"

#include <stack>
#include <vector>

class ResourceManager {
private:
	std::stack<unsigned int> handles_;	// list of free handles
	std::vector<Resource*> list_;				// list of pointers to resources
	// constructor
	ResourceManager();
	// destructor
	~ResourceManager();
public:
	static ResourceManager* instance();
	
	// returns a pointer to the vector of resources
	inline std::vector<Resource*> getList() { return this->list_; }

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
		if ( filePath == NULL )
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

		// We "reserve" the handle by inserting a NULL value in its place
		// This ensures that even if some other resource will be added, the handles won't overlap
		if ( handleAvailable ) {
			handle = this->handles_.top();
			this->handles_.pop();
			list_[handle] = NULL;
		}
		else {
			handle = this->list_.size();
			this->list_.push_back(NULL);
		}

		// create the resource
		this->list_[handle] = (Resource*)( new T( handle, filePath ) );

		// return the handle
		return handle;
	};

	// overloaded [] operator for easy access
	Resource* operator [](unsigned int handle);
};

#endif
