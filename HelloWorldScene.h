#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class NewScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();           
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(NewScene);

private:
    cocos2d::Sprite* sprite;
};

#endif 
