#ifndef _RESOURCE_H_
#define _RESOURCE_H_

class Resource
{
protected:
	char *filename_;
	unsigned int index_;
	unsigned long refCount_;

public:
	// constructor and destructor
	Resource( const unsigned int index, const char *szFullPath );
	~Resource();
	
	inline char* getFilename() { return this->filename_; }
	inline unsigned int getIndex() { return this->index_; }
	inline unsigned long getRefCount() { return this->refCount_; }
	inline void incRefCount() { this->refCount_++; }
	inline void decRefCount() { this->refCount_--; }

	// Method that the ResourceManager will call after creating a resource
	// I would have put it in the constructor but you can't call overriden methods in the constructor
	// (quite an interesting read on StackOverflow about that)
	virtual bool load() = 0;
};

#endif