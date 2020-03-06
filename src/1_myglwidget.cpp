

#include "myglwidget.h"
#include "game.h"
#include "read_bmp.h"
#include <math.h>

using namespace std;
#define PI 3.141592653

#include <ctime>


GLubyte myplane_texture[64][64][4];
GLuint myplane_texname;
GLubyte (*fan_texture)[64][64][4];
void makeCheck();


/**
 * 函数：MyGLWidget::MyGLWidget
 * 函数描述：MyGLWidget类的初始化，在这里初始化了玩家操纵飞机的初始位置，并计算出相应的初始所在网格
 */
MyGLWidget::MyGLWidget(QWidget *parent)
    :QOpenGLWidget(parent)
{

    //设置窗口大小
    this->setMinimumSize(1920,1080);
    //将时钟信号与游戏进行函数关联
    connect(&timer,SIGNAL(timeout()),this,SLOT(onetime()));
//    vector<vector<vector<int>>> collide_check;//标识每个检查方格内存在的子弹 //[WIDTH/CSIZE][HEIGHT/CSIZE]
 //   vector<vector<vector<int>>> collide_check_fan;//标识每个检查方格内存在的其他同类// [WIDTH/CSIZE][HEIGHT/CSIZE]
  //      vector<vector<Background_point>> background_points;//[WIDTH/BSIZE][HEIGHT/BSIZE]

    //初始化碰撞检测用的数组
    vector<int> t1;
    vector<vector<int>> t2;
    for(int i=0;i<arg.HEIGHT/arg.CSIZE;i++){
        t2.push_back(t1);
    }
    for(int i=0;i<arg.WIDTH/arg.CSIZE;i++){
        collide_check.push_back(t2);
        collide_check_fan.push_back(t2);
    }

    //初始化背景控制点
    vector<Background_point> t3(arg.HEIGHT/arg.BSIZE);
    for(int i=0;i<arg.WIDTH/arg.BSIZE;i++){
        background_points.push_back(t3);
    }

    //设置玩家初始位置
    my.center.x=arg.WIDTH/2;
    my.center.y=arg.HEIGHT/2;

    //加载敌人数据
    Fan::load();

    //加载关卡数据
    lev.load(string("custom_level.txt"));
    score=0;

}

MyGLWidget::~MyGLWidget()
{
    delete[] fan_texture;
}

/**
 * 函数：MyGLWidget::initializeGL
 * 函数描述：1.读入并绑定玩家飞机、敌人的纹理贴图
 *         2.生成一个黑洞
 *         3.初始化背景线条的属性
 */
void MyGLWidget::initializeGL()
{
    glViewport(0, 0, width()+5, height()+5);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    //加载玩家纹理贴图
    read_bmp("image.bmp",myplane_texture);
    //一通操作，就将myplane_texname跟玩家机体的纹理贴图连接起来，之后用myplane_texname表示玩家飞机
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1);

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1,&myplane_texname);
    glBindTexture(GL_TEXTURE_2D,myplane_texname);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0,GL_RGBA, GL_UNSIGNED_BYTE, myplane_texture);

    //加载敌人纹理贴图
    fan_texture=new GLubyte[Fan::picture_name.size()][64][64][4];
    for(int i=0;i<Fan::picture_name.size();i++){
        read_bmp(Fan::picture_name[i].c_str(),fan_texture[i]);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_ALPHA_TEST);

        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        GLuint temp_texname;
        glGenTextures(1,&temp_texname);
        glBindTexture(GL_TEXTURE_2D,temp_texname);
        Fan::tex_name.push_back(temp_texname);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0,GL_RGBA, GL_UNSIGNED_BYTE, fan_texture[i]);
    }

    //初始时给一个黑洞
    blackholes.push_back(Blackhole(Point(100,500,0),Point(0.1,0,0)));
    for(int i=0;i<arg.WIDTH/arg.BSIZE;i++){
        for(int j=0;j<arg.HEIGHT/arg.BSIZE;j++){
            background_points[i][j]=Background_point(Point(i*arg.BSIZE,j*arg.BSIZE,0),500,100);
        }
    }
}


/**
 * 函数：MyGLWidget::onetime
 * 函数描述：一次时钟到时则触发调用的函数，更新地图上的所有元素
 */
void MyGLWidget::onetime(){
    //用来记录下游戏运行时性能的部分代码
    static double time=clock(),cnt;
    static long record_times;
    static FILE *file;
    if(cnt<100){
        cnt++;
    }else{
        if(record_times==0){
            file=fopen("analyze_data.txt","w");
        }
        record_times++;
        cnt=0;
        printf("\rframe rate:%lf,number of enemies:%d,bullets:%d,sparks:%d       ",100/((clock()-time)/CLOCKS_PER_SEC),fans.size(),bullets.size(),sparks.size());
        fprintf(file,"frame rate:%lf,number of enemies:%d,bullets:%d,sparks:%d    \n",100/((clock()-time)/CLOCKS_PER_SEC),fans.size(),bullets.size(),sparks.size());
        time=clock();
    }

    //依次更新玩家飞机、黑洞、子弹、敌人
    my.update(blackholes);
    my.update_pos(arg);
    update_blackhole(blackholes,arg);
    update_bullet(my,bullets,collide_check,sparks,blackholes,arg);//更新子弹位置并且看是否碰壁，产生火花
    int t;
    t=update_fan(my,bullets,fans,collide_check,collide_check_fan,sparks,blackholes,arg);//更新敌人位置，并且看是否碰子弹，死亡产生火花
    //t>0:命中敌人个数 t<0:被下标为-t的敌人碰到，游戏结束 t=0相安无事
    if(t>0){
        score+=t;
        emit score_changed(score);
    }else if(t<0){
        emit game_end();
        Fan a=fans[-t-1];

        fans.clear();
        fans.push_back(a);
        bullets.clear();
        sparks.clear();
        blackholes.clear();

        for(int i=0;i<collide_check.size();i++){
            for(int j=0;j<collide_check[i].size();j++){
                collide_check[i][j].clear();
            }
        }
        for(int i=0;i<collide_check_fan.size();i++){
            for(int j=0;j<collide_check_fan[i].size();j++){
                collide_check_fan[i][j].clear();
            }
        }
        stop_game();
    }
    update_spark(sparks,blackholes,arg);//更新火花位置
    create_fan(my,fans,blackholes,arg,lev);//根据关卡信息增加敌人
    background(background_points,blackholes,bullets,collide_check,arg);//更新背景控制点
    //在上面元素更新完后，开始绘制画面,update()会调用到paintGL()
    update();
}

/**
 * 函数： MyGLWidget::paintGL
 * 函数描述：定时被调用，用于绘制地图上包括背景、飞机、敌人、子弹、黑洞、爆炸火花在内的所有元素
 */
void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //todo: 后面考虑根据玩家位置更新视角
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //依次绘制各种元素
    my.draw();
    for(size_t i=0;i<bullets.size();i++){
        bullets[i].draw();
    }
    for(size_t i=0;i<fans.size();i++){
        fans[i].draw(fans[i]);
    }
    for(size_t i=0;i<sparks.size();i++){
        sparks[i].draw();
    }
    for(size_t i=0;i<blackholes.size();i++){
        blackholes[i].draw();
    }

    //下面部分就是绘制背景控制点相连的画面
    glPushMatrix();
    glBegin(GL_LINES);
    for(int i=0;i<arg.WIDTH/arg.BSIZE;i++){
        for(int j=0;j<arg.HEIGHT/arg.BSIZE-1;j++){
            if(i%4==0 ){
                glColor3f(0,0,1);
            }else{
                glColor3f(0,0,0.5);
            }
            glVertex3d(background_points[i][j].center.x,background_points[i][j].center.y,background_points[i][j].center.z);
            glVertex3d(background_points[i][j+1].center.x,background_points[i][j+1].center.y,background_points[i][j+1].center.z);
        }
    }
    for(int i=0;i<arg.WIDTH/arg.BSIZE-1;i++){
        for(int j=0;j<arg.HEIGHT/arg.BSIZE;j++){
            if( j%4==0){
                glColor3f(0,0,1);
            }else{
                glColor3f(0,0,0.5);
            }
            glVertex3d(background_points[i][j].center.x,background_points[i][j].center.y,background_points[i][j].center.z);
            glVertex3d(background_points[i+1][j].center.x,background_points[i+1][j].center.y,background_points[i+1][j].center.z);
        }
    }
    glEnd();
    glPopMatrix();

    //最后绘制边界
    draw_border(arg);
    glColor3f(1,1,1);
}

/**
 * 函数： MyGLWidget::resizeGL
 * 函数描述：调整视口、投影坐标
 * 参数描述：
 *  width：视口宽
 *  height：视口高
 */
void MyGLWidget::resizeGL(int width, int height)
{
    /*
    glViewport(0, 0, width+5, height+5);
	update();
    */
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,(float)width/height,1,30);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-3.6);
}

/*
弹起wasd 就停止原来方向加速
*/
void MyGLWidget::keyReleaseEvent(QKeyEvent *e){

    if (e->key()==Qt::Key_W){
        my.direction=my.direction-Point(0,5,0);
    }else if (e->key()==Qt::Key_S){
        my.direction=my.direction-Point(0,-5,0);
    }else if(e->key()==Qt::Key_A){
        my.direction=my.direction-Point(-5,0,0);
    }else if(e->key()==Qt::Key_D){
        my.direction=my.direction-Point(5,0,0);
    }
}
void MyGLWidget::keyPressEvent(QKeyEvent *e) {
    //wasd上左下右加速
    if (e->key()==Qt::Key_W){
        my.direction=my.direction+Point(0,5,0);
    }else if (e->key()==Qt::Key_S){
        my.direction=my.direction+Point(0,-5,0);
    }else if(e->key()==Qt::Key_A){
        my.direction=my.direction+Point(-5,0,0);
    }else if(e->key()==Qt::Key_D){
        my.direction=my.direction+Point(5,0,0);
    }
    //空格暂停或开始
    if(e->key()==Qt::Key_Space){
        if(timer.isActive()){
            stop_game();
        }else{
            start_game();
        }
    }
    //上下左右改变开火方向
    switch (e->key()) {
    case(Qt::Key_Up):
        my.fire_pos=Point(0,10,0);
        break;
    case(Qt::Key_Down):
        my.fire_pos=Point(0,-10,0);
        break;
    case(Qt::Key_Left):
        my.fire_pos=Point(-10,0,0);
        break;
    case(Qt::Key_Right):
        my.fire_pos=Point(10,0,0);
        break;
    }
}

void MyGLWidget::start_game(){
    showFullScreen();//开始游戏时游戏画面弹出来
    this->timer.start(18);
}
void MyGLWidget::stop_game(){
//    Qt::WindowFlags m_flags = NULL;
//    setWindowFlags(m_flags);
    this->timer.stop();
    my.direction=Point();
    emit stoped();
}

