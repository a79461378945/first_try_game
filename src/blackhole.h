#ifndef BLACKHOLE_H
#define BLACKHOLE_H

/*
黑洞 能吸引其他东西
 */
#include "entity_and_point.h"
#include "argument.h"
#include <vector>
using namespace std;
struct Blackhole{
    Point center;//中心位置
    Point speed;//速度
    Point add_speed;//加速度
    double radius;//半径,与引力等相关
    Blackhole(const Point& c,const Point& speed); //构造函数，指定初始位置与速度方向
    void draw(); //绘制黑洞
    void update(vector<Blackhole>& blackholes);//更新自己的加速度 只受其他黑洞影响
    int update_pos(const argument& arg);//根据加速度和速度，更新速度和位置
};

#endif // BLACKHOLE_H
