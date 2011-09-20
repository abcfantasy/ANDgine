#ifndef _CONTAINERNODE_H_
#define _CONTAINERNODE_H_

class ContainerNode : public SceneNode {
private:
	std::vector<SceneNode*> objects_;
public:
	ContainerNode();
	~ContainerNode();

	inline std::vector<SceneNode*>* getObjects() { return &objects_; };
	SceneNode* getObjectAt( int pos ) { return this->objects_[pos]; };

	void AddObject( SceneNode *object );
	void render( float deltaT );
	void compile();
};

#endif