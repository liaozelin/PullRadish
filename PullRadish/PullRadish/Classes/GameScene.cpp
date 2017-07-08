#include "GameScene.h"

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }

	//屏幕的大小和原点
    Size size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//创建萝卜1的动画
	carrot1_animation();

	//创建萝卜2的动画
	carrot2_animation();

	//创建萝卜3的动画
	carrot3_animation();

	/*测试动画是否有问题（可删）
	Sprite* carrot1 = carrot1_init();
	Animate* carrot1Animate = Animate::create(AnimationCache::getInstance()->getAnimation("carrot1Animation"));
	carrot1->runAction(RepeatForever::create(carrot1Animate));
	carrot1->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(carrot1, 1);

	Sprite* carrot2 = carrot2_init();
	Animate* carrot2Animate = Animate::create(AnimationCache::getInstance()->getAnimation("carrot2Animation"));
	carrot2->runAction(RepeatForever::create(carrot2Animate));
	carrot2->setPosition(150 + origin.x, origin.y + 250);
	this->addChild(carrot2, 1);

	Sprite* carrot3 = carrot3_init();
	Animate* carrot3Animate = Animate::create(AnimationCache::getInstance()->getAnimation("carrot3Animation"));
	carrot3->runAction(RepeatForever::create(carrot3Animate));
	carrot3->setPosition(200 + origin.x, origin.y + 300);
	this->addChild(carrot3, 1);*/

    return true;
}


//创建萝卜1动画
void GameScene::carrot1_animation() {
	Animation* carrot1Animation = Animation::create();
	auto texture1 = Director::getInstance()->getTextureCache()->addImage("carrot1.png");
	for (int i = 0; i < 80; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 37, 1 * i + 20)));
		carrot1Animation->addSpriteFrame(frame);
	}
	carrot1Animation->setDelayPerUnit(0.05);
	AnimationCache::getInstance()->addAnimation(carrot1Animation, "carrot1Animation");
}


//创建萝卜2动画
void GameScene::carrot2_animation() {
	Animation* carrot1Animation = Animation::create();
	auto texture1 = Director::getInstance()->getTextureCache()->addImage("carrot2.png");
	for (int i = 0; i < 80; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 41, 1 * i + 20)));
		carrot1Animation->addSpriteFrame(frame);
	}
	carrot1Animation->setDelayPerUnit(0.05);
	AnimationCache::getInstance()->addAnimation(carrot1Animation, "carrot2Animation");
}


//创建萝卜3动画
void GameScene::carrot3_animation() {
	Animation* carrot1Animation = Animation::create();
	auto texture1 = Director::getInstance()->getTextureCache()->addImage("carrot3.png");
	for (int i = 0; i < 80; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 61, 1 * i + 20)));
		carrot1Animation->addSpriteFrame(frame);
	}
	//设置萝卜3拔出的速度最慢
	carrot1Animation->setDelayPerUnit(0.05);
	AnimationCache::getInstance()->addAnimation(carrot1Animation, "carrot3Animation");
}


//初始化萝卜1
Sprite* GameScene::carrot1_init() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("carrot1.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 37, 20)));
	auto carrot1 = Sprite::createWithSpriteFrame(frame);
	return carrot1;
}


//初始化萝卜2
Sprite* GameScene::carrot2_init() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("carrot2.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 41, 20)));
	auto carrot2 = Sprite::createWithSpriteFrame(frame);
	return carrot2;
}

//初始化萝卜3
Sprite* GameScene::carrot3_init() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("carrot3.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 61, 20)));
	auto carrot3 = Sprite::createWithSpriteFrame(frame);
	return carrot3;
}


