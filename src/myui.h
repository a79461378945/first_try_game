#ifndef MYUI_H
#define MYUI_H

#include <QWidget>

namespace Ui {
class myui;
}

class myui : public QWidget
{
    Q_OBJECT

public:
    explicit myui(QWidget *parent = nullptr);
    ~myui();
signals:
    void start_game();//发出开始游戏的信号
    void stop_game();//暂停信号
public slots:
    void change_score(int score);//更新显示的分数
    void showup();//将该窗口显示出来（游戏暂停时用）
    void end();//结束游戏
private slots:
    void on_start_button_clicked();

    void on_stop_button_clicked();

private:
    Ui::myui *ui;
};

#endif // MYUI_H
