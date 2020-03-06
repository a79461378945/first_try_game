#ifndef ARGUMENT_H
#define ARGUMENT_H
/*
整合一下游戏宽、高，
WIDTH 宽
HEIGHT 高
CSIZE 一个碰撞检测的方格的长
BSIZE 背景点之间间隔
fire_turn 多少帧画面射击一次
fan_turn 多少帧画面对应敌人的一个turn
*/

class argument
{
public:
    argument();
    int WIDTH;
    int HEIGHT;
    int CSIZE;
    int BSIZE;
    int fire_turn;
    int fan_turn;
};

#endif // ARGUMENT_H
