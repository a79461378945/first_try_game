#include "spark.h"

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

Spark::Spark(const Point& c,const Point& speed,double r,double g,double b){
    center=c;
    this->speed=speed;
    add_speed=0;
    red=r;
    green=g;
    blue=b;
    fiction=0.1;
    gravity=10;
}
/**
 * 函数：Spark::draw
 * 函数描述：绘制爆炸火花运动轨迹
 */
void Spark::draw(){
    Point l=center-speed*4; //拖动
    glPushMatrix();
    glColor3f(red, green, blue);
    glBegin(GL_LINE_LOOP);
    glVertex3f(center.x,center.y,center.z);
    glVertex3f(l.x,l.y,l.z);
    glEnd();
    glPopMatrix();
}

/**
 * 函数： Spark::update
 * 函数描述：更新爆炸火花的加速度，加速度受黑洞引力以及摩擦力fv的影响。
 * 参数描述：
 *   blackholes：提供当前地图上存在的黑洞的list
 */
void Spark::update(vector<Blackhole>& blackholes){
    fiction=0.1;
    for(int i=0;i<blackholes.size();i++){
        if(distance(blackholes[i].center,center)<blackholes[i].radius*7.5 && distance(blackholes[i].center,center)>blackholes[i].radius*0.1){
            fiction=0;
            Point collide_speed=blackholes[i].center-center;
            collide_speed=collide_speed/pow((0.001+distance(blackholes[i].center,center)),2)*10*gravity;//吸引力
            add_speed=add_speed+collide_speed;//加上这个物体对本物体的吸引力
        }

    }
    add_speed.x-= speed.x*fiction;
    add_speed.y-= speed.y*fiction;
}

/**
 * 函数：Spark::update_pos
 * 函数描述：定时被调用，通过v = v0+at重新计算速度，再根据Δs = v*Δt更新实体位置
 */
int Spark::update_pos(const argument& arg){
    speed.x+=add_speed.x*0.5;
    speed.y+=add_speed.y*0.5;
    add_speed.x=add_speed.y=0;
    if(fabs(fiction)>0.001){
        if(center.x+speed.x>=90 && center.x+speed.x<arg.WIDTH-90){
            center.x+=speed.x;
        }else{
            speed.x=-speed.x;
        }
        if(center.y+speed.y>=90 && center.y+speed.y<arg.HEIGHT-90){
            center.y+=speed.y;
        }else{
            speed.y=-speed.y;
        }
    }else{
        center.x+=speed.x;
        center.y+=speed.y;
    }
    return 0;
}
