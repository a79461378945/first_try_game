#include "entity_and_point.h"
#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

/**
 * 函数：Entity::update_pos
 * 函数描述：定时被调用，通过v = v0+at重新计算速度，再根据Δs = v*Δt更新实体位置
 */
int Entity::update_pos(const argument& arg){//最低位标识x是否溢出，第二位标识y，若x溢出则返回值+1，y溢出+2
    int mark=0;
    //int x=(speed.x)/pow((speed.x*speed.x+speed.y*speed.y),0.5)*speed_len;
    //int y=(speed.y)/pow((speed.x*speed.x+speed.y*speed.y),0.5)*speed_len;
    speed.x+=add_speed.x*0.5;
    speed.y+=add_speed.y*0.5;
    int max_rate=25;
    if(pow(speed.x*speed.x+speed.y*speed.y,0.5)>power*max_rate){
        speed.x=speed.x/pow(speed.x*speed.x+speed.y*speed.y,0.5)*power*max_rate;
        speed.y=speed.y/pow(speed.x*speed.x+speed.y*speed.y,0.5)*power*max_rate;
    }
    add_speed.x=add_speed.y=0;
    if(center.x+speed.x>=100 && center.x+speed.x<arg.WIDTH-100){
        center.x+=speed.x;
    }else{
        mark+=1;
    }
    if(center.y+speed.y>=100 && center.y+speed.y<arg.HEIGHT-100){
        center.y+=speed.y;
    }else{
        mark+=2;
    }
    return mark;
}

Point Point::operator+(const Point t)const{
    return Point(x+t.x,y+t.y,z+t.z);
}
Point Point::operator-(const Point t)const{
    return Point(x-t.x,y-t.y,z-t.z);
}
bool Point::operator==(const Point t)const{
    return fabs(x-t.x)<0.00001 && fabs(y-t.y)<0.00001 && fabs(z-t.z)<0.00001;
}
Point operator/(const double a,const Point t){
    return Point(a/t.x,a/t.y,a/t.z);
}
Point operator/(const Point t,const double a){
    return Point(t.x/a,t.y/a,t.z/a);
}
Point operator*(const Point t,const double a){
    return Point(t.x*a,t.y*a,t.z*a);
}
Point operator*(const double a,const Point t){
    return Point(t.x*a,t.y*a,t.z*a);
}
/**
 * 函数： distance
 * 函数描述：计算两点间距离
 * 参数描述：
 *       a、b:a和b两个点
 * 返回值： a b两点间距离
 */
double distance(const Point& a,const Point& b){
    return pow( (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y),0.5);
}
