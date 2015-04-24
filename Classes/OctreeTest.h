#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Octree.h"
#include "Player.h"

class Camera3D;
class DrawNode3D;
class CameraControl;

class OctreeTest : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(OctreeTest);
    
    void update(float delta);
    
private:
    void initCamera();
    void initGrid();
    void initPlayers();
    
private:
    CameraControl* _cameraControl;
    Camera3D* _camera;
    Label *_labelPos, *_labelRot;
    
    DrawNode3D* _drawNode;
    std::vector<Player*> _players;
    Octree _octree;
    
    AABB _cameraAABB;
};

#endif // __HELLOWORLD_SCENE_H__
