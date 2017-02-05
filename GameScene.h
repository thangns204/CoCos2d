#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually

	//Tao quai
	void addTarget();
	void gameLogic(float dt);
	void spriteMoveFinished(cocos2d::Node* sender);

	//su kien touch
	void onTouchEnded(cocos2d::Touch* touches, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touches, cocos2d::Event* event);
	bool onTouchBegan(cocos2d::Touch* touches, cocos2d::Event* event);

	//thiet ke vat ly, va cham
	bool onContactBegin(const PhysicsContact& contact);

	CREATE_FUNC(GameScene);
	 
private:

	unsigned int score;

	cocos2d::Label *scoreLabel; 

};

#endif // __GAME_SCENE_H__
