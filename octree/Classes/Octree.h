//
//  Octree.h
//  granny
//
//  Created by DONG on 15/4/23.
//
//

#ifndef __granny__Octree__
#define __granny__Octree__

#include "cocos2d.h"
USING_NS_CC;

class MoveObject
{
public:
    MoveObject();
    MoveObject(const AABB& aabb);
    MoveObject(const AABB& aabb, void* userData);
    const AABB& getAABB() const;
    void setAABB(const AABB& aabb);
    void* getUserData();
    void setUserData(void* userData);
    
protected:
    AABB _aabb;
    void* _userData;
};

class Octree
{
public:
    Octree();
    Octree(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
    Octree(const AABB& aabb);
    Octree(const Vec3& min, const Vec3& max);
    void setAABB(const AABB& aabb);
    const AABB& getAABB() const;
    
    bool insert(MoveObject* data);
    void query(std::vector<MoveObject*>& result, const AABB& aabb);
    void clear();
    
private:
    void init();
    void subdivide();
    
private:
    AABB _aabb;
    Vec3 _halfSize;
    
    Octree *_leftTopFront, *_leftTopBack;
    Octree *_rightTopFront, *_rightTopBack;
    Octree *_leftBottomFront, *_leftBottomBack;
    Octree *_rightBottomFront, *_rightBottomBack;
    
    std::vector<MoveObject*> _userData;
    int _maxNode;
};




#endif /* defined(__granny__Octree__) */
