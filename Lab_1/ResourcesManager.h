#ifndef _RESOURCESMANAGER_H_
#define _RESOURCESMANAGER_H_

#include <string>
#include <stack>
#include <vector>

template<class T>
class ResourcesManager
{
private:
	std::stack<unsigned int> handles_;	// list of free handles
	std::vector<T*> *list_;				// list of pointers to resources
	void (*CreateResource)(T **resource, const unsigned int handle, char *filePath ); // application specific resource creation
public:
	// constructor
	ResourcesManager(void(*CreateResourceFunction)(T **resource, 
		const unsigned int handle, char *filePath ) = NULL)
	{
		this->list_ = new std::vector<T*>;
		this->CreateResource = CreateResourceFunction;
	}

	// destructor
	~ResourcesManager()
	{
		emptyList();
		delete this->list_;
	}

	// returns a pointer to the vector of resources
	inline std::vector<T*> *getList() { return this->list_; }

	// returns a resource by filename
	T* getElement( char* filePath )
	{
		// check that name and path are valid, and list is not empty
		if ( filePath != NULL || this->list_ == NULL || this->list_->empty() )
			return NULL;

		// iterate through the list
		for ( std::vector<T*>::iterator i = this->list_->begin(); i != this->list_->end(); i++ )
		{
			if ( (*i) != NULL && (*i)->getFilename() == filePath/*&& (*i)->getPath() == path*/ )
				return (*i);
		}

		// return null if not found
		return NULL;
	}

	// return a resource by handle
	T* getElement( const unsigned int handle )
	{
		// if handle is valid
		if ( handle < this->list_->size() && handle >= 0 )
			return ( *list_ )[handle];

		// return null if not found
		return NULL;
	}

	// empty the resource list
	void emptyList()
	{
		for ( std::vector<T*>::iterator i = this->list_->begin(); i != this->list_->end(); i++ )
		{
			delete(*i);
			*i = NULL;
		}

		while (!this->handles_.empty())
			this->handles_.pop();

		// use vector swapping trick to release previously used memory
		this->list_->clear();
		this->list_->swap(*list_);
	}

	// remove a given resource by handle
	void removeResource( const unsigned int handle )
	{
		// ensure the handle is valid
		if ( handle < 0 || this->list_ == NULL || handle > this->list_->size() || (*list_)[handle] == NULL )
			return;

		T* resource = (*list_)[handle];

		// decrement the reference count
		resource->decRefCount();

		// if reference count is 0, destroy it
		if ( resource->getRefCount() == 0 )
		{
			// add the handle to the stack of free handles
			this->handles_.push( handle );
			// delete resource
			delete resource;
			// mark slot as NULL
			(*list_)[handle] = NULL;
		}
	}

	// add a new resource
	unsigned int addResource( char *filePath )
	{
		// check that filename and list are valid
		if ( this->list_ == NULL || filePath == NULL )
			return -1;

		// if element already exists
		T* element = this->getElement( filePath );
		if ( element != NULL )
		{
			element->incRefCount();
			return element->getHandle();
		}

		// check for available handle or use new handle
		bool handleAvailable = !this->handles_.empty();
		unsigned int handle;
		if ( handleAvailable )
		{
			handle = this->handles_.top();
			this->handles_.pop();
		}
		else
			handle = this->list_->size();

		// create the resource
		T *resource = NULL;
		if ( this->CreateResource != NULL )
			CreateResource( &resource, handle, filePath );
		else
			resource = new T(handle, filePath);

		// add the resource to the manager
		if ( handleAvailable )
			(*list_)[handle] = resource;
		else
			this->list_->push_back(resource);

		// return the handle
		return handle;
	}

	// overloaded [] operator for easy access
	T* operator [](unsigned int handle)
	{
		if ( handle < this->list_->size() && handle >= 0 )
			return (*list_)[handle];

		return NULL;
	}
};

#endif
