#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "ContainerNode.h"

class FrameIndexBuffer;

class SceneManager {
private:
	ContainerNode sceneGraph_;
	FrameIndexBuffer *frameIndexBuffer_;
	int timeLastFrame_;

	SceneManager();
	~SceneManager();

public:
	static SceneManager* instance();

	inline ContainerNode* getSceneGraph() { return &this->sceneGraph_; };

	void initializeScene();
	void renderScene();
};

#endif