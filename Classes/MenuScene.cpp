#include "MenuScene.h"
#include "GameScene.h"
USING_NS_CC;

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 背景设置
	auto bg = Sprite::create("Start.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width; //获取屏幕宽度
	float winh = mywinsize.height;//获取屏幕高度
	float spx = bg->getTextureRect().getMaxX();
	float spy = bg->getTextureRect().getMaxY();
	bg->setScaleX(winw / spx); //设置精灵宽度缩放比例
	bg->setScaleY(winh / spy);
	this->addChild(bg, 0);

	// 下雨特效
	CCParticleSystem* particleSystem = CCParticleRain::create();
	particleSystem->setSpeed(300.0f);
	this->addChild(particleSystem);

	// 开始按钮
	auto start = MenuItemImage::create("Title.png", "TitleN1.png",
		CC_CALLBACK_1(MenuScene::menuItemStartCallback, this));
	start->setPosition(Vec2(origin.x + visibleSize.width - 200, visibleSize.height / 2 + origin.y));

	auto menu = Menu::create(start, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	return true;
}

void MenuScene::menuItemStartCallback(Ref* pSender) {
	MenuItem* item = (MenuItem*)pSender;
	log("touch Setting %p", item);

	auto sc = GameScene::createScene(20, 1);
	Director::getInstance()->pushScene(sc);
}

