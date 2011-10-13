#ifndef _MODELNODE_H_
#define _MODELNODE_H_

#include "Vertex3f.h"
class ModelNode : public SceneNode {
private:
	std::vector<Vertex3f> vertices_;
	int textureHandle;

	void Initialize();
public:
	ModelNode();
	ModelNode( int texture );
	~ModelNode();

	void AddVertex( Vertex3f vertex );
	void render( float deltaT );
	void compile();
};

#endif