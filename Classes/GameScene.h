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

    //�ܲ�1�Ķ���
    void carrot1_animation();
    //�ܲ�2�Ķ���
    void carrot2_animation();
    //�ܲ�3�Ķ���
    void carrot3_animation();
    //�����ƶ�����
    void player_move();
    //��ʼ���ܲ�1
    Sprite* carrot1_init();
    //��ʼ���ܲ�2
    Sprite* carrot2_init();
    //��ʼ���ܲ�3
    Sprite* carrot3_init();
    //��ʼ������
    Sprite* player_init();

    void set_target_score(int target);
    void set_game_count(int count);
    /*
    * ��������������Ϸ�������߻��ұߣ���ʵ�ֱ������޹���
    * @param: flag Ϊ��ʱ�ƶ�����ߣ�Ϊ��ʱ�ƶ����ұ�
    */
    void set_bg(int flag);
    /*
    * ��������Ϸ������һ�������ƶ�
    * @param: distance �ƶ��ķ���(����Ϊ��)�;���
    */
    void world_move(int distance);

    void score_increase();
    void game_over();

    void passButtonCallback(Ref * pSender);
    void gameoverButtonCallback(Ref * pSender);

    // Ĭ�ϵ�������ʵ��:
    //      1.���µ�ͼ(���޹���)
    //      2.�ܲ��ƶ�
    //      3.�����һ���ܲ������ұ߽�һ������ʱ����һ���ܲ�(�ܲ���������)
    // ��Ұ������Ҽ��ƶ�ʱ��ʵ�����ƶ��Ĳ���player��
    // ���Ǳ������ܲ��ǣ�������������������ƶ���
    void update(float dt);
    // �������� ����ʱ
    void time_decrease(float dt);

private:
    void preloadMusic();
    void playBgm();

    void addKeyboardListener();

    // ���¼��̰������ɿ�������������������������¼��ж�
    void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

    // �����һ���ܲ��ұ����һ���µ��ܲ���������������Ļ�
    void addRadish(int x);
    // �Ƴ�redish_selected����Ϊnull
    void removeRadish();

    void showOperations();
    void hideOperations();
    /*
     * ���һ���������жϲ��������Ƿ���ȷ��ִ����ϣ�
     * ��ִ����ϣ������ܲ�
     * @param key_pressed ��ʾ���µķ�������ϻ�����
     */
    void doOperation(char key_pressed);

    static const string NAME_UP;
    static const string NAME_DOWN;

    static const int LEVEL; // ��ƽ��(�߶�)
    static const int SPACE;

    Size visibleSize;
    Vec2 origin;

    Action* playerAction;
    
    list<Sprite*> radishes;  // �ܲ��б�
    Sprite* radish_selected;  // �������ѡ�е��ܲ�

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
    bool is_showing; // ������ʾ�Ƿ��Ѿ����֣���ֹ�ظ����ɲ���
    bool last_direction; // Ϊ�����ң�Ϊ������
    char key_pressed;

    // ÿ�������ߵ�һ���ܲ���ʱ���������op_count(2~4)������
    // ������ɻ����뿪�ܲ�ʱ�����⼸������
    // ��ʼ��ʱ����tag���ԣ����·����ʱ��ɸ���tag�ж��Ƿ���ȷ
    int op_count;
    int op_done_count;
    vector<Sprite*> operations; // �����ĸ�����
    Size op_size;

    Sprite* player;

    // ʵ�ֱ������߹���
    Sprite* bg1;
    Sprite* bg2;
};

