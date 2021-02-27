#include "HelloWorldScene.h"
#include "scene1.h"

USING_NS_CC;

Scene* NewScene::createScene()
{
    return NewScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool NewScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
   
    auto visibleSize = Director::getInstance()->getVisibleSize();    

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(NewScene::menuCloseCallback, this));
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = visibleSize.width - closeItem->getContentSize().width/2;
        float y = closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto bg = Sprite::create("bg1.png");
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bg, 0);

    auto ScoreMes = Label::createWithTTF("Record: ", "fonts/Marker Felt.ttf", 24);   
    ScoreMes->setPosition(Vec2(ScoreMes->getContentSize().width / 2,
        Director::getInstance()->getVisibleSize().height - ScoreMes->getContentSize().height));
    ScoreMes->enableGlow(Color4B::RED);
    this->addChild(ScoreMes, 1); 
    char text[10];
    sprintf(text, "%d", UserDefault::getInstance()->getIntegerForKey("score"));   
    auto scoreLabel = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 24);
    scoreLabel->setPosition(Vec2(scoreLabel->getContentSize().width / 2 + ScoreMes->getContentSize().width,
        Director::getInstance()->getVisibleSize().height - ScoreMes->getContentSize().height));
    scoreLabel->enableGlow(Color4B::RED);
    this->addChild(scoreLabel, 1);

    auto play = Label::createWithTTF("P l a y !", "fonts/Marker Felt.ttf", 24);
    if (play == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        play->setPosition(Vec2(visibleSize.width/2,
                                visibleSize.height - play->getContentSize().height - visibleSize.height / 5));
        this->addChild(play, 1);
    }
    play->enableGlow(Color4B::RED);

    auto bgPl = Sprite::create("Planet.png");
    if (bgPl == nullptr)
    {
        problemLoading("'Planet.png'");
    }
    else
    {
        bgPl->setScale(0.25f, 0.25f);
        bgPl->setPosition(Point(0,0));
        this->addChild(bgPl, 0);
    }

    auto bgPl2 = Sprite::create("Planet1.png");
    if (bgPl2 == nullptr)
    {
        problemLoading("'Planet.png'");
    }
    else
    {
        bgPl2->setScale(0.2f, 0.2f);
        bgPl2->setPosition(visibleSize.width, visibleSize.height);
        this->addChild(bgPl2, 0);
    }

    auto hero = Sprite::create("Hero.png");
    if (hero == nullptr)
    {
        problemLoading("'CloseSelected.png'");
    }
    else
    {
        hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(hero, 0);
    }
      
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        auto bounds = event->getCurrentTarget()->getBoundingBox();
        Vec2 loc = event->getCurrentTarget()->getPosition();
        if (bounds.containsPoint(touch->getLocation())) {
            auto NewScene = scene1::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1, NewScene, Color3B(255, 255, 255)));            
        }
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, play);

    return true;
}

void NewScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}