//
//  Player.h
//  granny
//
//  Created by DONG on 15/4/23.
//
//

#ifndef __granny__Player__
#define __granny__Player__

#include "Octree.h"
#include "cocos2d.h"
USING_NS_CC;

class Player : public MoveObject, public Node
{
public:
    CREATE_FUNC(Player);
    bool init();
    virtual void setPosition3D(const Vec3 &position);
    void setFlash(bool bFlash);
    
private:
    CC_SYNTHESIZE(Vec3, _size, Size);
    Sprite3D* _sprite;
    bool _bFlash;
    CC_SYNTHESIZE(bool, _bHit, Hit);
};

#endif /* defined(__granny__Player__) */
