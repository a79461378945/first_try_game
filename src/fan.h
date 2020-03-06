#ifndef FAN_H
#define FAN_H

#include "entity_and_point.h"
#include "my_plane.h"
#include "bullet.h"
#include "blackhole.h"
#include <vector>
#include <string>
#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
using namespace std;

/**
 * 类：Fan
 * 类描述：继承Entity的一个基类，每个对象表示一个敌人
 */

struct Fan:public Entity{ //敌人
    double repulsive;//排斥力系数，越大则相互越不容易重叠
    void (*draw)(Fan& fan1);//绘制自身的函数指针，可以给不同函数

    //更新自身加速度的函数指针，若有需要可以另写函数赋值即可
    void (*update)(Fan& fan1,const Myplane& my,vector<Fan>& fans,vector<vector<vector<int>>>& collide_check_fan
                    ,vector<Bullet>& bullets,vector<vector<vector<int>>>& collide_check_bullet,vector<Blackhole>& blackholes,const argument& arg);//根据玩家目前位置以及子弹、其他fan位置更新加速度

    Fan(int type=0);//选择fan的类型
    /*新增*/
    float angle=0;
    int type_of_fan; //指定敌人的种类

    static vector<string> type_name;//敌人种类
    static vector<string> picture_name;//敌人的贴图文件名
    static vector<GLuint> tex_name;//敌人的贴图名

    //各种敌人对应的属性
    static vector<int> radiuses;
    static vector<double> repulsives;
    static vector<double> powers;
    static vector<double> fictions;
    static vector<double> gravitys;

    //各种敌人的运动模式参数
    static vector<double> follow_rate;//追击玩家的比重
    static vector<double> dodge_others_rate;//远离其他敌人的比重
    static vector<double> dodge_bullets_rate;//躲子弹的比重
    //todo:提供用户自定义控制敌人运动模式的方法 中缀表达式求值
    static void load();//加载上述static的参数

};

//默认的绘制方法
void draw_fan(Fan& fan1);
//默认的更新方法
void update_fan(Fan& fan1,const Myplane& my,vector<Fan>& fans,vector<vector<vector<int>>>& collide_check_fan,vector<Bullet>& bullets,
                   vector<vector<vector<int>>>& collide_check_bullet,vector<Blackhole>& blackholes,const argument& arg);

#endif // FAN_H
