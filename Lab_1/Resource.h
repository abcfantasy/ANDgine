#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>

class Resource
{
private:
	char *name_;
	char *path_;
	char *filename_;
	unsigned int handle_;
	unsigned long refCount_;

public:
	// constructor and destructor
	Resource( const unsigned int handle, char *szFullPath )
	{
		// store variables
		if ( szFullPath != NULL )
			this->filename_ = szFullPath;

		/*if ( path != NULL )
			this->path_ = path;

		if ( name != NULL && path != NULL ) 
		{
			std::strcpy( this->filename_, path );
			std::strcat( this->filename_, name );
		}*/

		this->handle_ = handle;

		// start reference count
		this->refCount_ = 1;
	}
	~Resource() {};

	 
	//inline char* getName() { return this->name_; }
	//inline char* getPath() { return this->path_; }
	inline char* getFilename() { return this->filename_; }
	inline unsigned int getHandle() { return this->handle_; }
	inline unsigned long getRefCount() { return this->refCount_; }
	inline void incRefCount() { this->refCount_++; }
	inline void decRefCount() { this->refCount_--; }
};

#endif