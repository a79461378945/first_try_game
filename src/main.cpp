#pragma execution_character_set("utf-8")
#include "myglwidget.h"
#include "myui.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MyGLWidget m;//游戏本体
    myui ui;//提供一点信息交互的界面

    //界面的开始、结束游戏按钮触发的信号 连到  游戏本体开始游戏、应用程序退出的槽
    QObject::connect(&ui,SIGNAL(start_game()),&m,SLOT(start_game()));
    QObject::connect(&ui,SIGNAL(stop_game()),&a,SLOT(quit()));

    //游戏本体 暂停游戏时-界面弹出  分数改变时-界面分数也跟着修改  游戏结束-界面上信息稍微修改
    QObject::connect(&m,SIGNAL(stoped()),&ui,SLOT(showup()));
    QObject::connect(&m,SIGNAL(score_changed(int)),&ui,SLOT(change_score(int)));
    QObject::connect(&m,SIGNAL(game_end()),&ui,SLOT(end()));

    //一开始只需要个界面就好
    ui.show();
    return a.exec();
}

