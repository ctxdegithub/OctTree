//
//  Octree.cpp
//  granny
//
//  Created by DONG on 15/4/23.
//
//

#include "Octree.h"


MoveObject::MoveObject()
{
    MoveObject(AABB(Vec3::ZERO, Vec3::ZERO), nullptr);
}

MoveObject::MoveObject(const AABB& aabb)
{
    MoveObject(aabb, nullptr);
}

MoveObject::MoveObject(const AABB& aabb, void* userData)
{
    _aabb = aabb;
    _userData = userData;
}

const AABB& MoveObject::getAABB() const
{
    return _aabb;
}

void MoveObject::setAABB(const AABB& aabb)
{
    _aabb = aabb;
}

void* MoveObject::getUserData()
{
    return _userData;
}

void MoveObject::setUserData(void* userData)
{
    _userData = userData;
}

#pragma Octree

Octree::Octree()
{
    _aabb = AABB(Vec3::ZERO, Vec3::ZERO);
    init();
}

Octree::Octree(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
    _aabb = AABB(Vec3(minX, minY, minZ), Vec3(maxX, maxY, maxZ));
    init();
}

Octree::Octree(const Vec3& min, const Vec3& max)
{
    _aabb = AABB(min, max);
    init();
}

Octree::Octree(const AABB& aabb)
{
    _aabb = aabb;
    init();
}

void Octree::init()
{
    _halfSize = _aabb._max - _aabb._min;
    _halfSize *= 0.5f;
    _maxNode = 2;
    _userData.reserve(_maxNode);
    _leftTopFront = nullptr;
}

void Octree::setAABB(const cocos2d::AABB &aabb)
{
    _aabb = aabb;
    _halfSize = _aabb._max - _aabb._min;
    _halfSize *= 0.5f;
}

const AABB& Octree::getAABB() const
{
    return _aabb;
}

bool Octree::insert(MoveObject* data)
{
    if (!_aabb.intersects(data->getAABB()))
    {
        return false;
    }
    
    if (_userData.size() < _maxNode)
    {
        _userData.push_back(data);
        return true;
    }
    
    if (_leftTopFront == nullptr)
    {
        subdivide();
    }
    
    if (_leftTopFront->insert(data)) return true;
    if (_leftTopBack->insert(data)) return true;
    if (_rightTopFront->insert(data)) return true;
    if (_rightTopBack->insert(data)) return true;
  
    if (_leftBottomFront->insert(data)) return true;
    if (_leftBottomBack->insert(data)) return true;
    if (_rightBottomFront->insert(data)) return true;
    if (_rightBottomBack->insert(data)) return true;
    
    return false; // should never happen
}

void Octree::query(std::vector<MoveObject*>& result, const AABB& aabb)
{
    if (!_aabb.intersects(aabb))
    {
        return;
    }
    
    for (int i=0; i<_userData.size(); ++i) {
        result.push_back(_userData[i]);
    }
    
    if (_leftTopFront != nullptr)
    {
        _leftTopFront->query(result, aabb);
        _leftTopBack->query(result, aabb);
        _rightTopFront->query(result, aabb);
        _rightTopBack->query(result, aabb);
        
        _leftBottomFront->query(result, aabb);
        _leftBottomBack->query(result, aabb);
        _rightBottomFront->query(result, aabb);
        _rightBottomBack->query(result, aabb);
    }
}

void Octree::subdivide()
{
    _leftTopFront = new Octree(_aabb.getCenter() + Vec3(-_halfSize.x, 0, 0),
                                _aabb.getCenter() + Vec3(0, _halfSize.y, _halfSize.z));
    
    _leftTopBack = new Octree(_aabb.getCenter() + Vec3(-_halfSize.x, 0, -_halfSize.z),
                                _aabb.getCenter() + Vec3(0, _halfSize.y, 0));
    
    _rightTopFront = new Octree(_aabb.getCenter() + Vec3(0, 0, 0),
                                _aabb.getCenter() + Vec3(_halfSize.x, _halfSize.y, _halfSize.z));
    
    _rightTopBack = new Octree(_aabb.getCenter() + Vec3(0, 0, -_halfSize.z),
                                _aabb.getCenter() + Vec3(_halfSize.x, _halfSize.y, 0));
    
    
    _leftBottomFront = new Octree(_aabb.getCenter() + Vec3(-_halfSize.x, -_halfSize.y, 0),
                                _aabb.getCenter() + Vec3(0, 0, _halfSize.z));
    
    _leftBottomBack = new Octree(_aabb.getCenter() + Vec3(-_halfSize.x, -_halfSize.y, -_halfSize.z),
                               _aabb.getCenter() + Vec3(0, 0, 0));
    
    _rightBottomFront = new Octree(_aabb.getCenter() + Vec3(0, -_halfSize.y, 0),
                                _aabb.getCenter() + Vec3(_halfSize.x, 0, _halfSize.z));
    
    _rightBottomBack = new Octree(_aabb.getCenter() + Vec3(0, -_halfSize.y, -_halfSize.z),
                                   _aabb.getCenter() + Vec3(_halfSize.x, 0, 0));
    
    CCLOG("left top front: %f, %f, %f-%f,%f,%f", _leftTopFront->getAABB()._min.x, _leftTopFront->getAABB()._min.y, _leftTopFront->getAABB()._min.z
          , _leftTopFront->getAABB()._max.x, _leftTopFront->getAABB()._max.y, _leftTopFront->getAABB()._max.z);
}

void Octree::clear()
{
    _userData.clear();
    if (_leftTopFront != NULL)
    {
        _leftTopFront->clear();
        delete _leftTopFront;
        _leftTopBack->clear();
        delete _leftTopBack;
        _rightTopFront->clear();
        delete _rightTopFront;
        _rightTopBack->clear();
        delete _rightTopBack;
        
        _leftBottomFront->clear();
        delete _leftBottomFront;
        _leftBottomBack->clear();
        delete _leftBottomBack;
        _rightBottomFront->clear();
        delete _rightBottomFront;
        _rightBottomBack->clear();
        delete _rightBottomBack;
    }
}
