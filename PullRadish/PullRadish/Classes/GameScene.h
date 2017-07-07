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
     * ��������������Ϸ������ұߣ���ʵ�ֱ������޹���
     * @param: bg Ҫ�����ı���ָ��
     */
    void set_bg_right(Sprite* bg);
    /*
     * ��������������Ϸ�������ߣ���ʵ�ֱ������޹���
     * @param: bg Ҫ�����ı���ָ��
     */
    void set_bg_left(Sprite* bg);

    void score_increase();

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

    // �����һ���ܲ��ұ����һ���µ��ܲ�
    void addRadish();
    // �Ƴ�redish_selected����Ϊnull
    void removeRadish();

    void showOperations();
    void hideOperations();

private:
    static const int TAG_UP;
    static const int TAG_DOWN;

    Size visibleSize;
    
    list<Sprite*> radishes;  // �ܲ��б�
    Sprite* radish_selected;  // �������ѡ�е��ܲ�

    Label* score;
    Label* time;

    bool is_over;
    bool is_pulling;
    bool is_moving;
    char key_pressed;

    // ÿ�������ߵ�һ���ܲ���ʱ���������op_count(2~4)������
    // ������ɻ����뿪�ܲ�ʱ�����⼸������
    // ��ʼ��ʱ����tag���ԣ����·����ʱ��ɸ���tag�ж��Ƿ���ȷ
    int op_count;
    Sprite* op1;
    Sprite* op2;
    Sprite* op3;
    Sprite* op4;

    Sprite* player;

    // ʵ�ֱ������߹���
    Sprite* bg1;
    Sprite* bg2;
    Sprite* bg_used;
};

