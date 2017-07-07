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

    Size size = Director::getInstance()->getVisibleSize();

    return true;
}
