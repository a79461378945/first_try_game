#include "myui.h"
#include "ui_myui.h"

myui::myui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myui)
{
    ui->setupUi(this);
}

myui::~myui()
{
    delete ui;
}

void myui::on_start_button_clicked()
{
    emit start_game();
}

void myui::on_stop_button_clicked()
{
    emit stop_game();
}

void myui::change_score(int score){
    ui->score->setNum(score);
}
void myui::showup(){
    Qt::WindowFlags m_flags = NULL;
    setWindowFlags(m_flags);
    show();
}
void myui::end(){
    ui->label->setText(QString::fromLocal8Bit("游戏结束最终分数"));
}
