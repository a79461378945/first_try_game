#ifndef SPARK_H
#define SPARK_H
#include "entity_and_point.h"
#include "blackhole.h"
#include <vector>
using namespace std;

/**
 * 类：Spark
 * 类描述：表示爆炸后的火花
 */
struct Spark{
    Point center;//中心位置
    Point speed;//速度
    Point add_speed;//加速度
    double red,green,blue;//火花的颜色rgb
    double fiction;//摩擦系数
    double gravity;//受引力影响程度
    Spark(const Point& c,const Point& speed,double r=1,double g=1,double b=1);
    void draw();
    void update(vector<Blackhole>& blackholes);
    int update_pos(const argument& arg);//根据加速度和速度，更新速度和位置
};
#endif // SPARK_H
