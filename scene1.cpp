#include "scene1.h"
#include "HelloWorldScene.h"
#include <cocos/editor-support/cocostudio/SimpleAudioEngine.h>
#define ccsf(...) CCString::createWithFormat(__VA_ARGS__)->getCString()
USING_NS_CC;

Scene* scene1::createScene()
{
    return scene1::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool scene1::init()
{        
    if (!Scene::initWithPhysics())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();    
    
    score = 0;
    k = 1;
    ch = false;
    ScoreMes = Label::createWithTTF("Score: ", "fonts/Marker Felt.ttf", 24);
    if (ScoreMes == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        ScoreMes->setPosition(Vec2(ScoreMes->getContentSize().width / 2,
            Director::getInstance()->getVisibleSize().height - ScoreMes->getContentSize().height));
        this->addChild(ScoreMes, 1);
    }

    auto bg = Sprite::create("bg2.png");        
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 20));
    this->addChild(bg, 0);
   
    hero = Sprite::create("Hero.png");
    if (hero == nullptr)
    {
        problemLoading("'Hero.png'");
    }
    else
    {           
        hero->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 2));        
        this->addChild(hero, 0);
    }      

    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
        Vec2 loc = event->getCurrentTarget()->getPosition();
        float w = event->getCurrentTarget()->getContentSize().width / 2;
        float h = event->getCurrentTarget()->getContentSize().height / 2;
        switch (keyCode) {              
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W:   
        {
            auto moveUp = RepeatForever::create(MoveBy::create(1, Point(0, +140)));
            moveUp->setTag(1);
            event->getCurrentTarget()->runAction(moveUp);
            break;
        }
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_S: 
        {
            auto moveDn = RepeatForever::create(MoveBy::create(1, Point(0, -140)));
            moveDn->setTag(2);
            event->getCurrentTarget()->runAction(moveDn);            
            break;
        }
        }
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, hero);
   
    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        Vec2 loc = event->getCurrentTarget()->getPosition();
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W:
        {
            event->getCurrentTarget()->stopActionByTag(1);
            break;
        }
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_S:
        {
            event->getCurrentTarget()->stopActionByTag(2);
            break;
        }        
        }
    };

    obst = Sprite::create("meteor_1.png");
    obst->setScale(0.05f, 0.05f);
    obst->setPosition(Vec2(visibleSize.width + obst->getContentSize().width / 2, cocos2d::random(0, (int)Director::getInstance()->getVisibleSize().height)));      
    addChild(obst);    

    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->preloadBackgroundMusic("Kerry Song.mp3"); 
    audio->playBackgroundMusic("Kerry Song.mp3");
    audio->setBackgroundMusicVolume(0.9f);

    auto eventListener1 = EventListenerKeyboard::create();
    eventListener1->onKeyPressed = [audio](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_SPACE:
            if (audio->isBackgroundMusicPlaying())
                audio->pauseBackgroundMusic();
            else
                audio->resumeBackgroundMusic();
            break;
        case EventKeyboard::KeyCode::KEY_R:
            audio->stopBackgroundMusic();
            audio->playBackgroundMusic("Kerry song.mp3");            
            break;
        case EventKeyboard::KeyCode::KEY_F4:            
            audio->stopBackgroundMusic();
            audio->playBackgroundMusic("Mal's song.mp3");                       
            break;        
        case EventKeyboard::KeyCode::KEY_INSERT:            
            audio->setBackgroundMusicVolume(1.0f);
            break;
        case EventKeyboard::KeyCode::KEY_END:
            audio->setBackgroundMusicVolume(0.9f);
            break;
        case EventKeyboard::KeyCode::KEY_DELETE:            
            audio->setBackgroundMusicVolume(0.8f);
            break;
        }
    };
    this->_eventDispatcher->addEventListenerWithFixedPriority(eventListener1, 2);

    this->scheduleUpdate();
    return true;
}

void scene1::update(float delta) {    
    if (hero->getPosition().y + 1 + hero->getContentSize().height / 2 > Director::getInstance()->getVisibleSize().height)
        hero->stopActionByTag(1);
    if (hero->getPosition().y - 1 - hero->getContentSize().height / 2 < 0)
        hero->stopActionByTag(2);
    
    if (!ch && CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() == 1.0f)
    {
        ch = true;
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.9f);
    }
    if (ch && CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() == 0.8f)
        ch = false;

    auto heroW = (hero->getBoundingBox().size.width / 2);
    auto obstW = (obst->getBoundingBox().size.width / 2);
    auto heroH = (hero->getBoundingBox().size.height / 2);
    auto obstH = (obst->getBoundingBox().size.height / 2);
        
    auto obstPos = obst->getPosition();
    obstPos.x -= 250 * delta * k;        
    if (obstPos.x < 0 - (obst->getBoundingBox().size.width / 2))
    {
        int obSprite = cocos2d::random(0, 2);
        this->removeChild(obst); 
        switch (obSprite)
        {
        case 0:
            obst = Sprite::create("meteor_1.png");
            break;
        case 1:
            obst = Sprite::create("meteor_2.png");
            break;
        case 2:
            obst = Sprite::create("meteor_3.png");          
            break;
        }            
        obst->setScale(0.05f, 0.05f);
        obst->setPosition(Vec2(Director::getInstance()->getVisibleSize().width + obst->getContentSize().width / 2, cocos2d::random(0, (int)Director::getInstance()->getVisibleSize().height)));
        addChild(obst);
        obstPos.x = this->getBoundingBox().getMaxX() + obstW;
        obstPos.y = cocos2d::random(0, (int)Director::getInstance()->getVisibleSize().height);
        score++;
        if (score % 10 == 0)
            k += 0.05;
    }
    obst->setPosition(obstPos);
        
    auto heroPos = hero->getPosition();
    if (heroPos.x - heroW <= obstPos.x + obstW &&
        heroPos.x + heroW >= obstPos.x - obstW &&
        heroPos.y - heroH <= obstPos.y + obstH &&
        heroPos.y + heroH >= obstPos.y - obstH)
    {
        if (CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() != 1.0f)
        {
            if (UserDefault::getInstance()->getIntegerForKey("score") < score && !ch)
            {
                UserDefault::getInstance()->setIntegerForKey("score", score);
                UserDefault::getInstance()->flush();
            }
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            auto NewScene = NewScene::createScene();
            Director::getInstance()->replaceScene(NewScene);
        }  
    }       
        
    char text[10];
    sprintf(text, "%d", score);        
    this->removeChild(scoreLabel, 1);
    scoreLabel = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 24);
    scoreLabel->setPosition(Vec2(scoreLabel->getContentSize().width / 2 + ScoreMes->getContentSize().width,
       Director::getInstance()->getVisibleSize().height - ScoreMes->getContentSize().height));
    this->addChild(scoreLabel, 1);        
    }

void scene1::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}