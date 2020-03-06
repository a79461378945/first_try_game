#ifndef BACKGROUND_POINT_H
#define BACKGROUND_POINT_H

/*
背景控制点 游戏时背景由这些点连线所得
 */
#include "entity_and_point.h"
#include "blackhole.h"
#include "bullet.h"
#include "argument.h"
#include <vector>
using namespace std;
struct Background_point{
    Point center;//中心位置
    Point origin_pos;//原始位置
    Point speed;//速度
    Point add_speed;//加速度
    double gravity_blackhole;//黑洞给的引力的系数
    double gravity_bullet;//子弹排斥力的系数
    double fiction;//摩擦力系数
    Background_point(){}
    Background_point(const Point& origin,double gravity_hole,double gravity_bul);

    //根据黑洞、子弹分布更新加速度
    void update(vector<Blackhole>& blackholes,vector<Bullet>& bullets,vector<vector<vector<int>>>& collide_check_bullet,const argument& arg);
    int update_pos();//根据加速度和速度，更新速度和位置
};

#endif // BACKGROUND_POINT_H
