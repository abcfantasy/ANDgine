#include "Resource.h"
#include <string.h>

// constructor and destructor
Resource::Resource( const unsigned int index, const char *szFullPath ) : index_( index ), refCount_( 1 ) {
	int filename_length = ( strlen( szFullPath ) + 1 ) * sizeof( char );
	filename_ = new char[ filename_length ];
	strcpy_s( filename_, filename_length, szFullPath );
};
Resource::~Resource() {
	delete[] filename_;
};