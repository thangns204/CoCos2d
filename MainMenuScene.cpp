#include "MainMenuScene.h"
#include "Definitions.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//hinh anh Main Menu
	auto backgroungSprite = Sprite::create("Iron.png");
	backgroungSprite->setScale(0.5);
	backgroungSprite->setPosition(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
	this->addChild(backgroungSprite);

	//Tao nut play
	auto menuImage = MenuItemImage::create("Play Button.png", "Play Button Clicked.png", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
	menuImage->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	auto menu = Menu::create(menuImage, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

    return true;

}

//chuyen sang GameScene
void MainMenuScene::GoToGameScene(cocos2d::Ref *sender)
{
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
