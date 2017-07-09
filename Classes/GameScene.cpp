#include "GameScene.h"
#include "ui/CocosGUI.h"
#include <algorithm>

USING_NS_CC;

using namespace cocos2d::ui;
using namespace CocosDenshion;

const string GameScene::NAME_UP = "W";
const string GameScene::NAME_DOWN = "S";
const int GameScene::LEVEL = 100;
const int GameScene::SPACE = 100;

cocos2d::Scene* GameScene::createScene(int target, int count) {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    layer->set_target_score(target);
    layer->set_game_count(count);
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
    visibleSize = size;
    origin = Director::getInstance()->getVisibleOrigin();

    //�����ܲ�1�Ķ���
    carrot1_animation();
    //�����ܲ�2�Ķ���
    carrot2_animation();
    //�����ܲ�3�Ķ���
    carrot3_animation();
    //�������ﶯ��
    player_move();

    last_direction = true;
    score = 0;
    time = 60;
    op_size = Size(40, 40);
    // ��ʾ������ʱ��
    scoreLabel = Label::createWithTTF("score: 0", "fonts/arial.ttf", 20);
    scoreLabel->setPosition(Vec2(200, 600));
    this->addChild(scoreLabel, 5);
    timeLabel = Label::createWithTTF("time: 60", "fonts/arial.ttf", 20);
    timeLabel->setPosition(Vec2(600, 600));
    this->addChild(timeLabel, 5);
    targetScoreLabel = Label::createWithTTF("target score: " + std::to_string(target_score), "fonts/arial.ttf", 20);
    targetScoreLabel->setPosition(Vec2(200, 550));
    this->addChild(targetScoreLabel, 5);
    // ������������(�̶���һ��λ��)
    Sprite* op_tmp;
    for (int i = 0; i < 4; ++i) {
        op_tmp = Sprite::create("up_arrow.jpg");
        op_tmp->setPosition(Vec2(400 + 50 * i, 500));
        op_tmp->setContentSize(op_size);
        op_tmp->setVisible(false);
        operations.push_back(op_tmp);
        this->addChild(op_tmp, 4);
    }
    // ��������
    bg1 = Sprite::create("background1.jpg");
    bg1->setAnchorPoint(Vec2(0, 0));
    bg1->setPosition(Vec2(0, 0));
    this->addChild(bg1, -5);
    bg2 = Sprite::create("background1.jpg");
    bg2->setAnchorPoint(Vec2(0, 0));
    bg2->setPosition(Vec2(bg1->getBoundingBox().getMaxX(), 0));
    this->addChild(bg2, -5);
    // ��������
    player = player_init();
    
    player->setPosition(Vec2(visibleSize.width / 2, LEVEL));
    this->addChild(player, 3);
    // �����ܲ�
    int mid = visibleSize.width / 2;
    for (int i = mid + SPACE; i < visibleSize.width; i += SPACE) {
        addRadish(i);
    }

    // ��ʼ��bgm
    preloadMusic();     // Ԥ��������
    playBgm();          // ���ű�������

    // ��Ӽ�����
    addKeyboardListener();
    // ��ӵ�����
    schedule(schedule_selector(GameScene::update), 0.04f, kRepeatForever, 0);
    schedule(schedule_selector(GameScene::time_decrease), 1.0f, kRepeatForever, 0);

    return true;
}

void GameScene::set_target_score(int target) {
    target_score = target;
    targetScoreLabel->setString("target score: " + std::to_string(target_score));
}

void GameScene::set_game_count(int count) {
    game_count = count;
}

void GameScene::set_bg(int flag) {
    int x = origin.x + 40 + (visibleSize.width / 2) * 3;
    int y = origin.y + visibleSize.height / 2;
    if (flag < 0)
        bg2->setPosition(Vec2(-visibleSize.width, 0));
    else
        bg1->setPosition(Vec2(bg2->getBoundingBox().getMaxX(), 0));
    Sprite* t = bg1;
    bg1 = bg2;
    bg2 = t;
}

void GameScene::world_move(int distance) {
    bg1->runAction(MoveBy::create(0.04f, Vec2(distance, 0)));
    bg2->runAction(MoveBy::create(0.04f, Vec2(distance, 0)));
	Animate* moveAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("moveAnimation"));
    player->runAction(moveAnimate);
    if (distance < 0) { // ���������ߣ����������ƶ�
        if (bg1->getBoundingBox().getMaxX() <= 0)
            set_bg(1);
    }
    else { // ���������ߣ����������ƶ�
        if (bg1->getBoundingBox().getMinX() >= 0)
            set_bg(-1);
    }
    
    for (Sprite* s : radishes) {
        if (s != NULL) {
            s->setPosition(s->getPosition() + Vec2(distance, 0));
        }
    }
}

void GameScene::score_increase() {
    score++;
    char str[15];
    sprintf(str, "score: %d", score);
    scoreLabel->setString(str);
}

void GameScene::game_over() {
    unschedule(schedule_selector(GameScene::update));
    auto layer = Scene::create();
    layer->setAnchorPoint(Vec2(0, 0));
    layer->setPosition(Vec2(0, 0));

    char str[50];
    sprintf(str, "Your score: %d    Target score: %d", score, target_score);
    Label* res_score = Label::createWithTTF(str, "fonts/arial.ttf", 30);
    res_score->setTextColor(Color4B::BLACK);
    res_score->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 50));
    layer->addChild(res_score);

    if (score < target_score) {
        Label* result = Label::createWithTTF("Game over!", "fonts/arial.ttf", 40);
        result->setTextColor(Color4B::BLACK);
        result->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100));
        layer->addChild(result);

        auto nameField = TextField::create("Input Your Name Here", "Arial", 30);
        nameField->setTextColor(Color4B::BLACK);
        nameField->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        layer->addChild(nameField);

        auto button = MenuItemFont::create("Submit", CC_CALLBACK_1(GameScene::gameoverButtonCallback, this));
        button->setColor(Color3B::BLACK);
        button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));
        auto menu = Menu::create(button, NULL);
        menu->setPosition(Vec2::ZERO);
        layer->addChild(menu);
    }
    else { // �÷ִﵽҪ�󣬼�����һ��
        Label* result = Label::createWithTTF("You Passed Game " + to_string(game_count), "fonts/arial.ttf", 40);
        result->setTextColor(Color4B::BLACK);
        result->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100));
        layer->addChild(result);

        auto button = MenuItemFont::create("Next", CC_CALLBACK_1(GameScene::passButtonCallback, this));
        button->setColor(Color3B::BLACK);
        button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        auto menu = Menu::create(button, NULL);
        menu->setPosition(Vec2::ZERO);
        layer->addChild(menu);
    }

    this->addChild(layer, 10);
}

// ���أ�������һ��
void GameScene::passButtonCallback(Ref * pSender) {
    auto sc = GameScene::createScene(target_score + 5, game_count + 1);
    Director::getInstance()->pushScene(sc);
}

void GameScene::gameoverButtonCallback(Ref * pSender) {

}

// Ĭ�ϵ�������ʵ��:
//      1.���µ�ͼ(���޹���)
//      2.�ܲ��ƶ�
//      3.�����һ���ܲ������ұ߽�һ������ʱ����һ���ܲ�(�ܲ���������)
// ��Ұ������Ҽ��ƶ�ʱ��ʵ�����ƶ��Ĳ���player��
// ���Ǳ������ܲ��ǣ�������������������ƶ���
void GameScene::update(float dt) {
    if (is_over) return;

    if (is_moving) {
        if (key_pressed == 'A') world_move(8);
        else if (key_pressed == 'D') world_move(-8);
    }

    for (Sprite* s : radishes) {
        if (player->getPosition().getDistance(s->getPosition()) < 10) {
            radish_selected = s;
            showOperations();
            is_pulling = true;
            break;
        }
    }
    if (radish_selected != NULL &&
            player->getPosition().getDistance(radish_selected->getPosition()) >= 30) {
        radish_selected = NULL;
        hideOperations();
        is_pulling = false;
    }

    Sprite* last_radish = radishes.back();
    Vec2 pos = last_radish->getPosition();
    if (pos.x + SPACE < visibleSize.width) {
        addRadish(pos.x + SPACE);
    }
}

// �������� ����ʱ
void GameScene::time_decrease(float dt) {
    if (time == 0) return;
    if (--time == 0) {
        is_over = true;
        unschedule(schedule_selector(GameScene::time_decrease));
        game_over();
    }
    char str[15];
    sprintf(str, "time: %d", time);
    timeLabel->setString(str);
}

void GameScene::addKeyboardListener() {
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

// ���¼��̰������ɿ�������������������������¼��ж�
void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    switch (code) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_CAPITAL_A:
    case EventKeyboard::KeyCode::KEY_A:
        key_pressed = 'A';
        is_moving = true;
        if (last_direction) {
            player->setFlippedX(true);
            last_direction = false;
        }
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_CAPITAL_D:
    case EventKeyboard::KeyCode::KEY_D:
        key_pressed = 'D';
        is_moving = true;
        if (!last_direction) {
            player->setFlippedX(false);
            last_direction = true;
        }
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_CAPITAL_W:
    case EventKeyboard::KeyCode::KEY_W:
        key_pressed = 'W';
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_CAPITAL_S:
    case EventKeyboard::KeyCode::KEY_S:
        key_pressed = 'S';
        break;
    }
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
    switch (code) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_CAPITAL_A:
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_CAPITAL_D:
    case EventKeyboard::KeyCode::KEY_D:
        is_moving = false;
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_CAPITAL_W:
    case EventKeyboard::KeyCode::KEY_W:
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_CAPITAL_S:
    case EventKeyboard::KeyCode::KEY_S:
        doOperation(key_pressed);
        break;
    }
}

// �����һ���ܲ��ұ����һ���µ��ܲ�
void GameScene::addRadish(int x) {
    int t = cocos2d::RandomHelper::random_int(1, 3);
    Sprite* new_radish;
    if (t == 1) new_radish = carrot1_init();
    else if (t == 2) new_radish = carrot2_init();
    else new_radish = carrot3_init();
    new_radish->setPosition(Vec2(x, LEVEL));
    this->addChild(new_radish, 2);
    radishes.push_back(new_radish);
}
// �Ƴ�redish_selected����Ϊnull
void GameScene::removeRadish() {
    if (radishes.size() == 0) return;
    radishes.remove(radish_selected);
    radish_selected->removeFromParentAndCleanup(false);
    radish_selected = NULL;
}

void GameScene::showOperations() {
    if (is_showing) return;
    is_showing = true;
    op_count = cocos2d::RandomHelper::random_int(2, 4);
    op_done_count = 0;
    for (int i = 0; i < op_count; ++i) {
        if (cocos2d::RandomHelper::random_int(1, 10) <= 5) { // �ϼ�ͷ
            operations[i]->setTexture("up_arrow.jpg");
            operations[i]->setName(NAME_UP);
        }
        else { // �¼�ͷ
            operations[i]->setTexture("down_arrow.jpg");
            operations[i]->setName(NAME_DOWN);
        }
        operations[i]->setContentSize(op_size);
        operations[i]->setVisible(true);
    }
}
void GameScene::hideOperations() {
    if (!is_showing) return;
    is_showing = false;
    for (int i = 0; i < op_count; ++i) {
        operations[i]->setVisible(false);
    }
    op_count = 0;
}
/*
* ���һ���������жϲ��������Ƿ���ȷ��ִ����ϣ�
* ��ִ����ϣ������ܲ�
* @param key_pressed ��ʾ���µķ�������ϻ�����
*/
void GameScene::doOperation(char key_pressed) {
    if (is_pulling && op_done_count < op_count) {
        if (key_pressed == operations[op_done_count]->getName()[0]) {
            operations[op_done_count]->setVisible(false);
            if (++op_done_count == op_count) {
                score_increase();

                //----------------------------------------------------------
                // ������ִ���ܲ����ϰγ��Ķ�����ִ�ж������������removeRadish()
                removeRadish();
                is_pulling = false;
                is_showing = false;
            }
        }
        else {
            op_done_count = 0;
            for (int i = 0; i < op_count; ++i) {
                operations[i]->setVisible(true);
            }
        }
    }
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
    carrot1Animation->setDelayPerUnit(0.05);
    AnimationCache::getInstance()->addAnimation(carrot1Animation, "carrot3Animation");
}

//��ʼ���ܲ�1
Sprite* GameScene::carrot1_init() {
    auto texture = Director::getInstance()->getTextureCache()->addImage("carrot1.png");
    auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 37, 30)));
    auto carrot1 = Sprite::createWithSpriteFrame(frame);
    return carrot1;
}

//��ʼ���ܲ�2
Sprite* GameScene::carrot2_init() {
    auto texture = Director::getInstance()->getTextureCache()->addImage("carrot2.png");
    auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 41, 30)));
    auto carrot2 = Sprite::createWithSpriteFrame(frame);
    return carrot2;
}

//��ʼ���ܲ�3
Sprite* GameScene::carrot3_init() {
    auto texture = Director::getInstance()->getTextureCache()->addImage("carrot3.png");
    auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 61, 30)));
    auto carrot3 = Sprite::createWithSpriteFrame(frame);
    return carrot3;
}

//�����ƶ�����
void GameScene::player_move() {
    Animation* moveAnimation = Animation::create();
    auto texture1 = Director::getInstance()->getTextureCache()->addImage("player1.png");
    for (int i = 0; i < 8; i++)
    {
        auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(95.8 * i, 0, 95.8, 90)));
        moveAnimation->addSpriteFrame(frame);
    }
    moveAnimation->setDelayPerUnit(0.05);
    AnimationCache::getInstance()->addAnimation(moveAnimation, "moveAnimation");
}

//��ʼ������
Sprite* GameScene::player_init() {
    auto texture = Director::getInstance()->getTextureCache()->addImage("player1.png");
    auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(670, 0, 95.8, 90)));
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