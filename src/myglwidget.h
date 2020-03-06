#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "my_plane.h"
#include "bullet.h"
#include "fan.h"
#include "spark.h"
#include "blackhole.h"
#include "background_point.h"
#include "argument.h"
#include "level_data.h"
using namespace std;
/*
 游戏主体的类
*/
class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();
    QTimer timer;//定时触发更新画面
signals:
    void score_changed(int score);//分数改变的信号
    void stoped();//游戏暂停的信号
    void game_end();//游戏结束的信号
protected:
    void initializeGL();//初始化opengl相关的资源 主要是纹理贴图
    void paintGL();//绘制画面
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);//按键
    void keyReleaseEvent(QKeyEvent *e);//弹起

public slots:
    void onetime();//每次定时器触发就调用一次的 更新的函数
    void start_game();//启动游戏（即定时器启动）
    void stop_game();//暂停游戏（定时器停下）

private:
    argument arg;//游戏环境的配置
    level_data lev;//关卡配置
    int score;//获得的分数
    vector<vector<vector<int>>> collide_check;//标识每个检查方格内存在的子弹 //[WIDTH/CSIZE][HEIGHT/CSIZE]
    vector<vector<vector<int>>> collide_check_fan;//标识每个检查方格内存在的其他同类// [WIDTH/CSIZE][HEIGHT/CSIZE]
    Myplane my; //操纵的飞机
    vector<Bullet> bullets;//子弹
    vector<Fan> fans;//敌人
    vector<Spark> sparks;//火花
    vector<Blackhole> blackholes;//黑洞
    vector<vector<Background_point>> background_points;//背景控制点 [WIDTH/BSIZE][HEIGHT/BSIZE]


};


#endif // MYGLWIDGET_H
