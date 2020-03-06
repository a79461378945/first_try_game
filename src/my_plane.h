#ifndef MY_PLANE_H
#define MY_PLANE_H

#include "entity_and_point.h"
#include "blackhole.h"
#include <vector>
using namespace std;

/**
 * 类：MyPlane
 * 类描述：继承自实体类Entity，对象表示玩家操控的飞机
 */
struct Myplane:public Entity{
    Myplane();
    Point fire_pos; //子弹方向
    Point direction; //飞机运动方向
    void draw();
    void update(vector<Blackhole>& blackholes);

};
#endif // MY_PLANE_H
