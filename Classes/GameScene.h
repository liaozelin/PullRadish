#pragma once

#include "cocos2d.h"
#include <string>
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int target, int count);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    //萝卜1的动画
    void carrot1_animation();
    //萝卜2的动画
    void carrot2_animation();
    //萝卜3的动画
    void carrot3_animation();
    //人物移动动画
    void player_move();
    //初始化萝卜1
    Sprite* carrot1_init();
    //初始化萝卜2
    Sprite* carrot2_init();
    //初始化萝卜3
    Sprite* carrot3_init();
    //初始任人物
    Sprite* player_init();

    void set_target_score(int target);
    void set_game_count(int count);
    /*
    * 将背景放置在游戏界面的左边或右边，以实现背景无限滚动
    * @param: flag 为负时移动到左边，为正时移动到右边
    */
    void set_bg(int flag);
    /*
    * 将整个游戏界面往一个方向移动
    * @param: distance 移动的方向(负数为左)和距离
    */
    void world_move(int distance);

    void score_increase();
    void game_over();

    void passButtonCallback(Ref * pSender);
    void gameoverButtonCallback(Ref * pSender);

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

    // 在最后一个萝卜右边添加一个新的萝卜，如果满足条件的话
    void addRadish(int x);
    // 移除redish_selected并置为null
    void removeRadish();

    void showOperations();
    void hideOperations();
    /*
     * 完成一个操作，判断操作序列是否正确的执行完毕，
     * 若执行完毕，消除萝卜
     * @param key_pressed 表示按下的方向键是上还是下
     */
    void doOperation(char key_pressed);

    static const string NAME_UP;
    static const string NAME_DOWN;

    static const int LEVEL; // 地平线(高度)
    static const int SPACE;

    Size visibleSize;
    Vec2 origin;

    Action* playerAction;
    
    list<Sprite*> radishes;  // 萝卜列表
    Sprite* radish_selected;  // 标记人物选中的萝卜

    Label* scoreLabel;
    Label* timeLabel;
    Label* targetScoreLabel;
    int score;
    int time;
    int target_score;

    int game_count;

    bool is_over;
    bool is_pulling;
    bool is_moving;
    bool is_showing; // 操作提示是否已经出现，防止重复生成操作
    bool last_direction; // 为真向右，为假向左
    char key_pressed;

    // 每次人物走到一个萝卜上时，随机生成op_count(2~4)个操作
    // 操作完成或者离开萝卜时隐藏这几个操作
    // 初始化时设置tag属性，按下方向键时便可根据tag判断是否正确
    int op_count;
    int op_done_count;
    vector<Sprite*> operations; // 储存四个操作
    Size op_size;

    Sprite* player;

    // 实现背景无线滚动
    Sprite* bg1;
    Sprite* bg2;
};

