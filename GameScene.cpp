#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"
#include "Definitions.h"

USING_NS_CC;

using namespace CocosDenshion;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
	//1 the gioi thu nho co tinh chat vat ly
    auto scene = Scene::createWithPhysics();
	//khung vat ly, duong vien bao quanh
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//gia toc = 0, cac doi tuong k bi roi xuong day
	scene->getPhysicsWorld()->setGravity(Vect(0, 0));
    // 'layer' is an autorelease object
	auto layer = GameScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	// lay kich thuoc man hinh
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size winSize = Director::getInstance()->getWinSize();

	//Tao 1 Sprite, nhan vat game
	auto player = Sprite::create("xx.jpg");
	//Khoi tao vi tri phia ben trai
	player->setPosition(Point(player->getContentSize().width / 2, winSize.height / 2));
	
	//Vat ly cho player dang hinh tron
	auto playerBody = PhysicsBody::createCircle(player->getContentSize().width / 2);
	
	//Dat co = 1,de kiem tra doi tuong khi va cham
 	player->setTag(1);

	//phai co lenh nay
	playerBody->setContactTestBitmask(0x1);

	//loai bo trong luc
	playerBody->setGravityEnable(false);

	//k tac dong va cham
	playerBody->setDynamic(false);

	//dat khung vao nhan vat
	player->setPhysicsBody(playerBody);

	//them vao layer nam trong Scene Game
	this->addChild(player);
	//Goi toi ham gameLogic , ham nay nhiem vu tao ra dam quai voi thoi gian 1 giay 1 quai
	this->schedule(schedule_selector(GameScene::gameLogic), 1.0);

	//Tao doi tuong truyen tai thong tin
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	
	//lang nghe su kien
	auto listener1 = EventListenerTouchOneByOne::create();
   
	//Thiet lap nuot su kien khi touch xay ra
	listener1->setSwallowTouches(true);

	//Bat su kien touch 
	listener1->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener1->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener1->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	//gui cho dispatcher xu li
	dispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

	//tao su kien lang nghe va cham neu xay ra
	auto contactListener = EventListenerPhysicsContact::create();

	//khi co va cham se goi cac ham onContactBegin de xu ly va cham do, chu y dong CC_CALLBACK_1
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);

	//Bo truyen tai ket noi voi doi tuong bat va cham
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    //Am thanh
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background-music-aac.wav");

	//lap vo han tuan hoan 
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background-music-aac.wav", true);

	//diem so
	score = 0;
	__String *tempScore = __String::createWithFormat("%i", score);
	scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);
	scoreLabel->setColor(Color3B::WHITE);
	scoreLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.75 + origin.y));

	this->addChild(scoreLabel, 10000);

    return true;
}

void GameScene::gameLogic(float dt)
{
	this->addTarget();
}

//Tao ra quai va di chuyen cua chung
void GameScene::addTarget()
{
	auto target = Sprite::create("target.png");
	Size winSize = Director::getInstance()->getWinSize();
	
	//Doan nay tinh vung cua quai khong bi khuat khoi man hinh

	int minY = target->getContentSize().height / 2;
	int	maxY = winSize.height - target->getContentSize().height / 2;
	int rangeY = maxY - minY;
	int actualY = (rand() % rangeY) + minY;

	//Dat quai vao vi tri khoang vi tri tren actualY
	target->setPosition(Point(winSize.width + (target->getContentSize().width / 2), actualY));

	//vat ly cho quai
	auto targetBody = PhysicsBody::createCircle(target->getContentSize().width / 2);
	target->setTag(2);
	targetBody->setContactTestBitmask(0x1);
	//loai bo trong luc
	targetBody->setGravityEnable(false);
	//k tac dong va cham
	//targetBody->setDynamic(false);
	target->setPhysicsBody(targetBody);

	this->addChild(target, 1);

	//Tinh toan toc do cua quai
	int minDuration = (int)2.0;
	int maxDuration = (int)4.0;
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() % rangeDuration) + minDuration;
	
	//Quai di chuyen voi toc do trong khoang actualDuration , tu diem xuat hien den Point(0 ,y);
	auto actionMove = MoveTo::create((float)actualDuration, Point(0 - target->getContentSize().width / 2, actualY));
	
	//Het thuc viec di chuyen
	auto actionMoveDone = CallFuncN::create(CC_CALLBACK_1(GameScene::spriteMoveFinished, this));

	//chay 2 hd theo tuan tu
	target->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
}

void GameScene::spriteMoveFinished(Node* sender)
{
	// Hàm này có mỗi công việc là loại bỏ Target ( đang là Sprite) ra khỏi layer của game
	// Ép kiểu Contrỏ Sprite của 1 Node*
	auto sprite = (Sprite *)sender;
	this->removeChild(sprite, true);
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	return true;//Phai tra ve true
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{
	//Khong xu ly gi ca
}

void GameScene::onTouchEnded(Touch* touches, Event* event)
{
	//lay toa do diem cham
	Point location = touches->getLocationInView();
	location = Director::getInstance()->convertToGL(location);

	Size winSize = Director::getInstance()->getWinSize();

	//Tao vien dan la Sprite, dat vi tri dau tien cua nhan vat
	auto projectile = Sprite::create("Projectile.png");
	projectile->setPosition(Point(20, winSize.height / 2));

	auto projectileBody = PhysicsBody::createCircle(projectile->getContentSize().width / 2);
	projectile->setTag(3);
	projectileBody->setContactTestBitmask(0x1);
	//loai bo trong luc
	projectileBody->setGravityEnable(false);
	//k tac dung
	//projectileBody->setDynamic(false);
	projectile->setPhysicsBody(projectileBody);

	// Đoạn này tính toán điểm cuối cùng của viên đạn thông qua vị trí đầu và vị trí Touch, 
	//hình ảnh bên dưới sẽ minh họa cho điều này. Ở đây áp dụng 1 vài công thức toán học rất cơ bản thôi nhé.
	//Không phức tạp lắm

	//lay toa do diem cham tru di toa do vien dan
	int offX = location.x - projectile->getPosition().x;
	int offY = location.y - projectile->getPosition().y;


	// Không cho phép bắn ngược và bắn thẳng đứng xuống dưới ( bên dưới nhân vật )
	if (offX <= 0) return;

	// Thỏa mãn điều trên thì tạo hình viên đạn trên màn
	this->addChild(projectile, 1);

	//Tính toán tọa độ điểm cuối thông qua toa độ điểm đầu và khoảng offX, offY
	// Tọa độ tuyệt đối realX = chiều rộng màn hình + 1/2 chiều rộng viên đạn, vừa lúc bay ra khỏi màn hình 
	int realX = winSize.width + (projectile->getContentSize().width / 2);

	//Ti le giua offY va offX
	float ratio = (float)offY / (float)offX;
	//int realY = ((realX - projectile->getPosition().x) * ratio) + projectile->getPosition().y; (realX - projectile->getPosition().x);
	int realY = ((realX - projectile->getPosition().x) * ratio) + projectile->getPosition().y;
	
	//Toa do diem cuoi
	auto realDest = Point(realX, realY);

	//Chieu dai duong di vien dan
	int offrealX = realX - projectile->getPosition().x;
	int offrealY = realY - projectile->getPosition().y;
	float lenght = sqrtf((offrealX*offrealX) + (offrealY*offrealY));

	//Thiet lap van toc vien dan 
	float velocity = 480 / 1;

	//thoi gian bay
	float realMoveDuration = lenght / velocity;

	// Di chuyển viên đạn tới điểm cuối với thời gian, và tọa độ đã tính ở trên. Khi qua viền màn hình thì biến mất
	projectile->runAction(Sequence::create(MoveTo::create(realMoveDuration, realDest),
		CallFuncN::create(CC_CALLBACK_1(GameScene::spriteMoveFinished, this)), NULL));

	//Am thanh

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("pew-pew-lei.wav");

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pew-pew-lei.wav");

}

bool GameScene::onContactBegin(const PhysicsContact& contact)
{
	//lay doi tuong va cham thu 1 , ep kieu SPrite
	auto bullet = (Sprite*)contact.getShapeA()->getBody()->getNode();
	//lay gia tri co cua no quai hay nahn vat hay dan
	int tag = bullet->getTag();

	//lay doi tuong va cham thu 2, ep kieu Sprite
	auto target = (Sprite*)contact.getShapeB()->getBody()->getNode();
	//lay gia tri co
	int tag1 = target->getTag();

	//Neu va cham xay ra giua dan va quai thi xoa dan va quai khoi man hinh
	if ((tag == 2 & tag1 == 3) || (tag == 3 & tag1 == 2))
	{
		this->removeChild(bullet, true);//xoa dan
		this->removeChild(target, true);//xoa dan
		
		//DIEM SO
		score++;
		__String *tempScore = __String::createWithFormat("%i", score);
		scoreLabel->setString(tempScore->getCString());
	}
	if ((tag == 1 & tag1 == 2) || (tag == 2 & tag1 == 1))
	{
		auto scene = GameOverScene::createScene(score);

		Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
		
	}
	return true;
}

