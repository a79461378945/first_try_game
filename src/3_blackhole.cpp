#include "blackhole.h"
#include "entity_and_point.h"
#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#define PI 3.14159
Blackhole::Blackhole(const Point& c,const Point& speed){
    center=c;
    this->speed=speed;
    add_speed=Point();
    radius=16;
}

/**
 * 函数：Blackhole::draw
 * 函数描述：绘制黑洞
 */
void Blackhole::draw(){
    int cir=20;
    glPushMatrix();
    glColor3f(1.0f, 0, 0);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<cir;i++){
        glVertex3f(center.x+radius*cos(2*PI*i/cir),center.y+radius*sin(2*PI*i/cir),center.z);
    }

    glEnd();
    glPopMatrix();
}

/**
 * 函数： Blackhole::update
 * 函数描述: 本黑洞与其他黑洞的交互，包括黑洞与黑洞间的吸引力、碰撞反弹及吞噬
 * 参数描述：
 *      blackholes ：当前地图上存在的黑洞的list
 */
void Blackhole::update(vector<Blackhole>& blackholes){
    if(radius<15 || radius>80){
        for(int i=0;i<blackholes.size();i++){
            if(distance(blackholes[i].center,center)<10){
                blackholes[i]=blackholes[blackholes.size()-1];
                blackholes.pop_back();
                return;
            }
        }
    }
    for(int i=0;i<blackholes.size();i++){
        if(distance(blackholes[i].center,center)<radius+blackholes[i].radius && distance(blackholes[i].center,center)>10){
            Point collide_speed=center-blackholes[i].center;

            Point p=blackholes[i].center-center;
            speed=speed+p*(-1)*(speed.x*p.x+speed.y*p.y)/pow((p.x*p.x+p.y*p.y),0.5);//弹性碰撞
            p=p*(-1);

            blackholes[i].speed=blackholes[i].speed+p*(-1)*(blackholes[i].speed.x*p.x+blackholes[i].speed.y*p.y)/pow((p.x*p.x+p.y*p.y),0.5);//弹性碰撞

            double dis=distance(collide_speed,Point());//强制分开
            collide_speed.x = (collide_speed.x)/pow((collide_speed.x*collide_speed.x+collide_speed.y*collide_speed.y),0.5)*(radius+blackholes[i].radius-dis)/1.9999;
            collide_speed.y = (collide_speed.y)/pow((collide_speed.x*collide_speed.x+collide_speed.y*collide_speed.y),0.5)*(radius+blackholes[i].radius-dis)/1.9999;
            center=center+collide_speed;
            blackholes[i].center=blackholes[i].center-collide_speed;
        }
    }


    for(int i=0;i<blackholes.size();i++){
        if(distance(blackholes[i].center,center)<(radius+blackholes[i].radius)*10){
            Point collide_speed=blackholes[i].center-center;
            collide_speed=collide_speed/pow((0.001+distance(blackholes[i].center,center)),2)*10;//吸引力
            add_speed=add_speed+collide_speed;//加上这个物体对本物体的吸引力
        }
    }
    if(pow((add_speed.x*add_speed.x+add_speed.y*add_speed.y),0.5)>15){//避免出现除以0，调整一下加速度大小
        add_speed.x = (add_speed.x)/pow((add_speed.x*add_speed.x+add_speed.y*add_speed.y),0.5)*5;
        add_speed.y = (add_speed.y)/pow((add_speed.x*add_speed.x+add_speed.y*add_speed.y),0.5)*5;
    }
}

/**
 * 函数： Blackhole::update_pos
 * 函数描述：与Entity的update_pos类似，重新计算速度，再根据当前速度更新黑洞位置
 * 返回值描述：返回值int最低位标识x是否溢出，第二位标识y，若x溢出则返回值+1，y溢出+2
 */
int Blackhole::update_pos(const argument& arg){
    int mark=0;
    speed.x+=add_speed.x*0.5;
    speed.y+=add_speed.y*0.5;
    add_speed.x=add_speed.y=0;
    if(pow((speed.x*speed.x+speed.y*speed.y),0.5)>5){//避免出现除以0，调整一下加速度大小
        speed.x = (speed.x)/pow((speed.x*speed.x+speed.y*speed.y),0.5);
        speed.y = (speed.y)/pow((speed.x*speed.x+speed.y*speed.y),0.5);
    }
    if(center.x+speed.x>=100 && center.x+speed.x<arg.WIDTH-100){
        center.x+=speed.x;
    }else{
        speed.x=-speed.x;
    }
    if(center.y+speed.y>=100 && center.y+speed.y<arg.HEIGHT-100){
        center.y+=speed.y;
    }else{
        speed.y=-speed.y;
    }
    return mark;
}
