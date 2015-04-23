#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Camera3D.h"
#include "CameraControl.h"
#include "DrawNode3D.h"

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    initCamera();
    initGrid();
    initPlayers();
    
    scheduleUpdate();
    return true;
}

void HelloWorld::initCamera()
{
    auto size = Director::getInstance()->getWinSize();
    _camera = Camera3D::create(60.f, size.width / size.height, 0.1f, 1000.f);
    addChild(_camera);
    _camera->setCameraFlag(CameraFlag::USER1);
    _camera->setPosition3D(Vec3(0, 2, 0));
    _camera->lookAt(Vec3(0, -1, -1));
    
    _cameraControl = CameraControl::create();
    addChild(_cameraControl);
    _cameraControl->setCamera(_camera);

}

void HelloWorld::initGrid()
{
    _drawNode = DrawNode3D::create();
    _drawNode->setCameraMask(2);
    addChild(_drawNode);
    // top bottom
    float topBottomPos[2] = {0, 10};
    Color4F topBottomcolor[] = {Color4F::GREEN, Color4F::BLUE};
    
    for (int j=0; j<2; ++j)
    {
        for (int i=0; i<=10; ++i)
        {
            _drawNode->drawLine(Vec3(-5 + i, topBottomPos[j], 5), Vec3(-5 + i, topBottomPos[j], -5), topBottomcolor[j]);
        }
        for (int i=0; i<=10; ++i)
        {
            _drawNode->drawLine(Vec3(-5, topBottomPos[j], -5 + i), Vec3(5, topBottomPos[j], -5 + i), topBottomcolor[j]);
        }
    }

    // left right
    float leftRightPos[2] = {-5, 5};
    Color4F leftRightcolor[] = {Color4F::RED, Color4F::RED};
    
    for (int j=0; j<2; ++j)
    {
        for (int i=0; i<=10; ++i)
        {
            _drawNode->drawLine(Vec3(leftRightPos[j], i, 5), Vec3(leftRightPos[j], i, -5), leftRightcolor[j]);
        }
        for (int i=0; i<=10; ++i)
        {
            _drawNode->drawLine(Vec3(leftRightPos[j], 0, -5 + i), Vec3(leftRightPos[j], 10, -5 + i), leftRightcolor[j]);
        }
    }
    
    // front back
    float frontBackPos[2] = {-5, 5};
    Color4F frontBackcolor[] = {Color4F::ORANGE, Color4F::ORANGE};
    
    for (int j=0; j<2; ++j)
    {
        for (int i=0; i<=10; ++i)
        {
            _drawNode->drawLine(Vec3(-5, i, frontBackPos[j]), Vec3(5, i, frontBackPos[j]), frontBackcolor[j]);
        }
        for (int i=0; i<=10; ++i)
        {
            _drawNode->drawLine(Vec3(-5 + i, 0, frontBackPos[j]), Vec3(-5 + i, 10, frontBackPos[j]), frontBackcolor[j]);
        }
    }
}

void HelloWorld::initPlayers()
{
    _octree.setAABB(AABB(Vec3(-5, 0, -5), Vec3(5, 10, 5)));
    for (int i=0; i<20; ++i)
    {
        auto player = Player::create();
        addChild(player);
        player->setCameraMask(2);
        player->setPosition3D(Vec3(CCRANDOM_MINUS1_1() * 5.f, CCRANDOM_0_1() * 10.f, CCRANDOM_MINUS1_1() * 5.f));
        _players.push_back(player);
        _octree.insert(player);
    }

}

void HelloWorld::update(float delta)
{
    auto pos = _camera->getPosition3D();
    _cameraAABB = AABB(pos - Vec3(1, 1, 1), pos + Vec3(1, 1, 1));
    std::vector<MoveObject*> players;
    _octree.query(players, _cameraAABB);
    CCLOG("%d", (int)players.size());
    
    for (int i=0; i<_players.size(); ++i)
    {
        _players[i]->setHit(false);
    }
    
    for (int i=0; i<players.size(); ++i)
    {
        ((Player*)players[i])->setHit(true);
    }
    for (int i=0; i<_players.size(); ++i)
    {
        _players[i]->setFlash(_players[i]->getHit());
    }
    
    
}