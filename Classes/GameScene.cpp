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

    //创建萝卜1的动画
    carrot1_animation();
    //创建萝卜2的动画
    carrot2_animation();
    //创建萝卜3的动画
    carrot3_animation();
    //创建人物动画
    player_move();

    last_direction = true;
    score = 0;
    time = 60;
    op_size = Size(40, 40);
    // 显示分数和时间
    scoreLabel = Label::createWithTTF("score: 0", "fonts/arial.ttf", 20);
    scoreLabel->setPosition(Vec2(200, 600));
    this->addChild(scoreLabel, 5);
    timeLabel = Label::createWithTTF("time: 60", "fonts/arial.ttf", 20);
    timeLabel->setPosition(Vec2(600, 600));
    this->addChild(timeLabel, 5);
    targetScoreLabel = Label::createWithTTF("target score: " + std::to_string(target_score), "fonts/arial.ttf", 20);
    targetScoreLabel->setPosition(Vec2(200, 550));
    this->addChild(targetScoreLabel, 5);
    // 创建操作序列(固定在一个位置)
    Sprite* op_tmp;
    for (int i = 0; i < 4; ++i) {
        op_tmp = Sprite::create("up_arrow.jpg");
        op_tmp->setPosition(Vec2(400 + 50 * i, 500));
        op_tmp->setContentSize(op_size);
        op_tmp->setVisible(false);
        operations.push_back(op_tmp);
        this->addChild(op_tmp, 4);
    }
    // 创建背景
    bg1 = Sprite::create("background1.jpg");
    bg1->setAnchorPoint(Vec2(0, 0));
    bg1->setPosition(Vec2(0, 0));
    this->addChild(bg1, -5);
    bg2 = Sprite::create("background1.jpg");
    bg2->setAnchorPoint(Vec2(0, 0));
    bg2->setPosition(Vec2(bg1->getBoundingBox().getMaxX(), 0));
    this->addChild(bg2, -5);
    // 创建人物
    player = player_init();
    
    player->setPosition(Vec2(visibleSize.width / 2, LEVEL));
    this->addChild(player, 3);
    // 创建萝卜
    int mid = visibleSize.width / 2;
    for (int i = mid + SPACE; i < visibleSize.width; i += SPACE) {
        addRadish(i);
    }

    // 初始化bgm
    preloadMusic();     // 预加载音乐
    playBgm();          // 播放背景音乐

    // 添加监听器
    addKeyboardListener();
    // 添加调度器
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
    if (distance < 0) { // 人物向右走，背景向左移动
        if (bg1->getBoundingBox().getMaxX() <= 0)
            set_bg(1);
    }
    else { // 人物向左走，背景向右移动
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
    else { // 得分达到要求，加载下一关
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

// 过关，加载下一关
void GameScene::passButtonCallback(Ref * pSender) {
    auto sc = GameScene::createScene(target_score + 5, game_count + 1);
    Director::getInstance()->pushScene(sc);
}

void GameScene::gameoverButtonCallback(Ref * pSender) {

}

// 默认调度器，实现:
//      1.更新地图(无限滚动)
//      2.萝卜移动
//      3.当最后一个萝卜离最右边界一定距离时新增一个萝卜(萝卜向右增长)
// 玩家按下左右键移动时，实际上移动的不是player，
// 而是背景和萝卜们，看起来就像是玩家在移动。
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

// 调度器： 倒计时
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

// 按下键盘按键和松开按键触发函数，在其中完成事件判断
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

// 在最后一个萝卜右边添加一个新的萝卜
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
// 移除redish_selected并置为null
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
        if (cocos2d::RandomHelper::random_int(1, 10) <= 5) { // 上箭头
            operations[i]->setTexture("up_arrow.jpg");
            operations[i]->setName(NAME_UP);
        }
        else { // 下箭头
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
* 完成一个操作，判断操作序列是否正确的执行完毕，
* 若执行完毕，消除萝卜
* @param key_pressed 表示按下的方向键是上还是下
*/
void GameScene::doOperation(char key_pressed) {
    if (is_pulling && op_done_count < op_count) {
        if (key_pressed == operations[op_done_count]->getName()[0]) {
            operations[op_done_count]->setVisible(false);
            if (++op_done_count == op_count) {
                score_increase();

                //----------------------------------------------------------
                // 在这里执行萝卜向上拔出的动画，执行动画结束后调用removeRadish()
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
    carrot1Animation->setDelayPerUnit(0.05);
    AnimationCache::getInstance()->addAnimation(carrot1Animation, "carrot3Animation");
}

//初始化萝卜1
Sprite* GameScene::carrot1_init() {
    auto texture = Director::getInstance()->getTextureCache()->addImage("carrot1.png");
    auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 37, 30)));
    auto carrot1 = Sprite::createWithSpriteFrame(frame);
    return carrot1;
}

//初始化萝卜2
Sprite* GameScene::carrot2_init() {
    auto texture = Director::getInstance()->getTextureCache()->addImage("carrot2.png");
    auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 41, 30)));
    auto carrot2 = Sprite::createWithSpriteFrame(frame);
    return carrot2;
}

//初始化萝卜3
Sprite* GameScene::carrot3_init() {
    auto texture = Director::getInstance()->getTextureCache()->addImage("carrot3.png");
    auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 61, 30)));
    auto carrot3 = Sprite::createWithSpriteFrame(frame);
    return carrot3;
}

//人物移动动画
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

//初始化人物
Sprite* GameScene::player_init() {
    auto texture = Director::getInstance()->getTextureCache()->addImage("player1.png");
    auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(670, 0, 95.8, 90)));
    auto player1 = Sprite::createWithSpriteFrame(frame);
    return player1;
}

//预加载背景音乐
void GameScene::preloadMusic() {
    auto audio = SimpleAudioEngine::getInstance();
    audio->preloadBackgroundMusic("bgm/bgm_3.mp3");
}


//播放背景音乐
void GameScene::playBgm() {
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("bgm/bgm_3.mp3");
}