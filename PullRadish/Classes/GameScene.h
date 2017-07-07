#pragma once

#include "cocos2d.h"
#include <string>
#include "SimpleAudioEngine.h"

using std::string;

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    /*
     * 将背景放置在游戏界面的右边，以实现背景无限滚动
     * @param: bg 要操作的背景指针
     */
    void set_bg_right(Sprite* bg);
    /*
     * 将背景放置在游戏界面的左边，以实现背景无限滚动
     * @param: bg 要操作的背景指针
     */
    void set_bg_left(Sprite* bg);

    void score_increase();

    // 默认调度器，实现:
    //      1.更新地图(无限滚动)
    //      2.萝卜移动
    //      3.当最后一个萝卜离最右边界一定距离时新增一个萝卜(萝卜向右增长)
    // 玩家按下左右键移动时，实际上移动的不是player，
    // 而是背景和萝卜们，看起来就像是玩家在移动。
    void update(float dt);
    // 调度器： 倒计时
    void time_decrease(float dt);

private:
    void preloadMusic();
    void playBgm();

    void addKeyboardListener();

    // 按下键盘按键和松开按键触发函数，在其中完成事件判断
    void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

    // 在最后一个萝卜右边添加一个新的萝卜
    void addRadish();
    // 移除redish_selected并置为null
    void removeRadish();

    void showOperations();
    void hideOperations();

private:
    static const int TAG_UP;
    static const int TAG_DOWN;

    Size visibleSize;
    
    list<Sprite*> radishes;  // 萝卜列表
    Sprite* radish_selected;  // 标记人物选中的萝卜

    Label* score;
    Label* time;

    bool is_over;
    bool is_pulling;
    bool is_moving;
    char key_pressed;

    // 每次人物走到一个萝卜上时，随机生成op_count(2~4)个操作
    // 操作完成或者离开萝卜时隐藏这几个操作
    // 初始化时设置tag属性，按下方向键时便可根据tag判断是否正确
    int op_count;
    Sprite* op1;
    Sprite* op2;
    Sprite* op3;
    Sprite* op4;

    Sprite* player;

    // 实现背景无线滚动
    Sprite* bg1;
    Sprite* bg2;
    Sprite* bg_used;
};

