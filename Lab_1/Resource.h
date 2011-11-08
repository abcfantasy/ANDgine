#ifndef _RESOURCE_H_
#define _RESOURCE_H_

class Resource
{
protected:
	char *filename_;
	unsigned int handle_;
	unsigned long refCount_;

public:
	// constructor and destructor
	Resource( const unsigned int handle, char *szFullPath ) : filename_( szFullPath ), handle_( handle ), refCount_( 1 ) {};
	~Resource() {};
	
	inline char* getFilename() { return this->filename_; }
	inline unsigned int getHandle() { return this->handle_; }
	inline unsigned long getRefCount() { return this->refCount_; }
	inline void incRefCount() { this->refCount_++; }
	inline void decRefCount() { this->refCount_--; }

	// Method that the ResourceManager will call after creating a resource
	// I would have put it in the constructor but you can't call overriden methods in the constructor
	// (quite an interesting read on StackOverflow about that)
	virtual void load() = 0;
};

#endif