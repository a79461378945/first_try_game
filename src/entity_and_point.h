#ifndef ENTITY_AND_POINT_H
#define ENTITY_AND_POINT_H
#include "argument.h"
/**
 * 类：Point
 * 类描述：基类，可描述空间中的点或向量
 * 类属性：x,y,z 均为double类型，合起来描述点/向量的位置。
 */
struct Point{
    double x,y,z;
    Point(double x1=0,double y1=0,double z1=0){
        x=x1;
        y=y1;
        z=z1;
    }
    Point operator+(const Point t)const;
    Point operator-(const Point t)const;
    bool operator==(const Point t)const;
};
Point operator/(const double a,const Point t);
Point operator/(const Point t,const double a);
Point operator*(const Point t,const double a);
Point operator*(const double a,const Point t);



/**
 * 类：Entity
 * 类描述: 基类，描述一个可交互的实体
 *
 */
struct Entity{
    Point center;//中心位置
    Point speed;//速度
    Point add_speed;//加速度
    int radius;//半径，碰撞检测用
    double power;//动力的大小
    double fiction;//摩擦系数
    double rotate_angle;//旋转角度 可能用到
    double gravity;//受引力影响程度
    int update_pos(const argument& arg);//根据加速度和速度，更新速度和位置
};
double distance(const Point& a,const Point& b);
#endif // ENTITY_AND_POINT_H
