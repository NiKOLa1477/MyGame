#ifndef SCENE1_SCENE_H
#define SCENE1_SCENE_H

#include "cocos2d.h"

class scene1 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();    
    void menuCloseCallback(cocos2d::Ref* pSender);
    void update(float delta);       
    CREATE_FUNC(scene1);    
    
private:
    cocos2d::Sprite* hero;
    cocos2d::Sprite* obst;
    cocos2d::Label* ScoreMes;
    cocos2d::Label* scoreLabel;
    int score;  
    double k;
    bool ch;
};

#endif 