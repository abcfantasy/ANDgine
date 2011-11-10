#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "Resource.h"

#include <stack>
#include <vector>

class ResourceManager {
private:
	std::stack<unsigned int> handles_;	// list of free handles
	std::vector<Resource*> list_;		// list of pointers to resources
	// constructor
	ResourceManager();
	// destructor
	~ResourceManager();

	Resource* find( char *filePath );

public:
	static ResourceManager* instance();
	
	// returns a pointer to the vector of resources
	inline std::vector<Resource*> getList() { return this->list_; }

	// empty the resource list
	void emptyList();

	// remove a given resource by handle
	void remove( const unsigned int handle );
	// remove a given resource
	void remove( Resource *resource );

	// get a resource by filename
	template <class T>
	T* get( char *filePath ) {
		// check that filename and list are valid
		if ( filePath == NULL )
			return NULL;

		// if element already exists
		Resource* element = this->find( filePath );
		if ( element != NULL ) {
			element->incRefCount();
			return (T*)element;
		}

		// check for available handle or use new handle
		bool handleAvailable = !this->handles_.empty();
		unsigned int index;

		if ( handleAvailable ) {
			index = this->handles_.top();
			this->handles_.pop();
		}
		else {
			index = this->list_.size();
			// We "reserve" the handle by inserting a NULL value in its place
			// This ensures that even if some other resource will be added, the handles won't overlap
			this->list_.push_back(NULL);
		}

		// create the resource
		Resource *resource = (Resource*)( new T( index, filePath ) );
		// call its initialization method
		resource->load();
		/*if( resource->load() == false ) {
			this->handles_.push( index );
			return NULL;
		}*/

		this->list_[index] = resource;
		
		// return the resource
		return (T*)resource;
	};

	// get a resource by index
	template <class T>
	Resource* get( unsigned int index ) {
		if ( index < this->list_.size() && index >= 0 )
			return this->list_[index];
		return NULL;
	};

	// overloaded [] operator for easy access
	Resource* operator [](unsigned int handle);
};

#endif
