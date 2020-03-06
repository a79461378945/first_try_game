#include "bullet.h"
#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

/**
 * 函数:Bullet::Bullet
 * 函数描述：初始化子弹的属性
 */
Bullet::Bullet(){
    radius=10;
    power=20;
    gravity=0.02;
}

/**
 * 函数：Bullet::draw()
 * 函数描述：画出子弹，子弹为三角形
 */
void Bullet::draw(){
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.657f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(center.x,center.y,center.z);
    glVertex3f(center.x+10,center.y,center.z);
    glVertex3f(center.x,center.y+10,center.z);
    glEnd();
    glPopMatrix();
}
/**
 * 函数： Bullet::update
 * 函数描述：计算黑洞对子弹的吸引力，并据此更新加速度
 * 参数描述：
 *      vector<Blackhole>& blackholes: 当前地图上的所有黑洞list
 */
void Bullet::update(vector<Blackhole>& blackholes){
    for(int i=0;i<blackholes.size();i++){
        Point collide_speed=center-blackholes[i].center;
        collide_speed=collide_speed/pow((0.001+distance(blackholes[i].center,center)),2)*gravity*pow(blackholes[i].radius,2);//吸引力与子弹与黑洞距离的平方成反比，与黑洞半径的平方成正比，并可通过gravity属性调整受影响程度
        add_speed=add_speed+collide_speed;//加上这个物体对本物体的吸引力
    }
}
