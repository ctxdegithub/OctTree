//
//  Player.cpp
//  granny
//
//  Created by DONG on 15/4/23.
//
//

#include "Player.h"

void Player::setPosition3D(const cocos2d::Vec3 &position)
{
    Node::setPosition3D(position);
    
    _aabb.set(position - _size * 0.5f, position + _size * 0.5f);
}

bool Player::init()
{
    if (!Node::init())
    {
        return false;
    }
    _bFlash = false;
    _bHit = false;
    
    _sprite = Sprite3D::create("sphere.c3b");
    _sprite->setScale(0.02f);
    addChild(_sprite);
    
    _size = _sprite->getMesh()->getAABB()._max - _sprite->getMesh()->getAABB()._min;
    _size *= 0.02f;
    setPosition3D(Vec3::ZERO);
    return true;
}

void Player::setFlash(bool bFlash)
{
    if (!_bFlash && bFlash)
    {
        _bFlash = true;
        runAction(RepeatForever::create(Blink::create(0.5f, 2)));
    }
    if (!bFlash)
    {
        _bFlash = false;
        stopAllActions();
        setVisible(true);
    }
}

void Player::setSelected(bool bSel)
{
    if (bSel)
    {
        _sprite->setColor(Color3B::RED);
    }
    else
    {
        _sprite->setColor(Color3B::WHITE);
    }
}