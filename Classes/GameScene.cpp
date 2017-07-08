#include "GameScene.h"
#include <algorithm>
USING_NS_CC;

using namespace std;
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

	//��Ļ�Ĵ�С��ԭ��
    Size size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//�����ܲ�1�Ķ���
	carrot1_animation();

	//�����ܲ�2�Ķ���
	carrot2_animation();

	//�����ܲ�3�Ķ���
	carrot3_animation();

	//�������ﶯ��
	player_move();

	//��������
	auto bg_game = Sprite::create("background1.jpg");
	bg_game->setPosition(Vec2(origin.x + size.width/2, origin.y + size.height/2));
	this->addChild(bg_game, 0);

	//��ʼ������
	player = player_init();
	Animate* moveAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("moveAnimation"));
	player->runAction(RepeatForever::create(moveAnimate));
	player->setPosition(250 + origin.x, origin.y + 102);
	this->addChild(player, 1);

	preloadMusic();     // Ԥ��������
	playBgm();          // ���ű�������


	//���Զ����Ƿ������⣨��ɾ��
	Sprite* carrot1 = carrot1_init();
	Animate* carrot1Animate = Animate::create(AnimationCache::getInstance()->getAnimation("carrot1Animation"));
	//carrot1->runAction(RepeatForever::create(carrot1Animate));
	carrot1->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(carrot1, 1);

	Sprite* carrot2 = carrot2_init();
	Animate* carrot2Animate = Animate::create(AnimationCache::getInstance()->getAnimation("carrot2Animation"));
	//carrot2->runAction(RepeatForever::create(carrot2Animate));
	carrot2->setPosition(150 + origin.x, origin.y + 102);
	this->addChild(carrot2, 1);

	Sprite* carrot3 = carrot3_init();
	Animate* carrot3Animate = Animate::create(AnimationCache::getInstance()->getAnimation("carrot3Animation"));
	//carrot3->runAction(RepeatForever::create(carrot3Animate));
	carrot3->setPosition(200 + origin.x, origin.y + 102);
	this->addChild(carrot3, 1);



    return true;
}


//�����ܲ�1����
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


//�����ܲ�2����
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


//�����ܲ�3����
void GameScene::carrot3_animation() {
	Animation* carrot1Animation = Animation::create();
	auto texture1 = Director::getInstance()->getTextureCache()->addImage("carrot3.png");
	for (int i = 0; i < 80; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 61, 1 * i + 20)));
		carrot1Animation->addSpriteFrame(frame);
	}
	//�����ܲ�3�γ����ٶ�����
	carrot1Animation->setDelayPerUnit(0.05);
	AnimationCache::getInstance()->addAnimation(carrot1Animation, "carrot3Animation");
}


//��ʼ���ܲ�1
Sprite* GameScene::carrot1_init() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("carrot1.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 37, 20)));
	auto carrot1 = Sprite::createWithSpriteFrame(frame);
	return carrot1;
}


//��ʼ���ܲ�2
Sprite* GameScene::carrot2_init() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("carrot2.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 41, 20)));
	auto carrot2 = Sprite::createWithSpriteFrame(frame);
	return carrot2;
}

//��ʼ���ܲ�3
Sprite* GameScene::carrot3_init() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("carrot3.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 61, 20)));
	auto carrot3 = Sprite::createWithSpriteFrame(frame);
	return carrot3;
}

//�����ƶ�����
void GameScene::player_move() {
	Animation* moveAnimation = Animation::create();
	auto texture1 = Director::getInstance()->getTextureCache()->addImage("player.png");
	for (int i = 0; i < 4; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(90 * i, 0, 90, 93)));
		moveAnimation->addSpriteFrame(frame);
	}
	moveAnimation->setDelayPerUnit(1);
	AnimationCache::getInstance()->addAnimation(moveAnimation, "moveAnimation");
}

//��ʼ������
Sprite* GameScene::player_init() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("player.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 90, 93)));
	auto player1 = Sprite::createWithSpriteFrame(frame);
	return player1;
}

//Ԥ���ر�������
void GameScene::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("bgm/bgm_3.mp3");
}


//���ű�������
void GameScene::playBgm() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("bgm/bgm_3.mp3");
}