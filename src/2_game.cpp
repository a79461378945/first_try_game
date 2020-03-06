#include "game.h"
#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>


#define PI 3.14159


/**
 * 函数：fire
 * 函数描述：根据本机目前状态发射两个子弹
 * 参数描述：
 *      my:玩家操控的飞机
 *      bullets:当前在运动中的子弹
 */
void fire(Myplane& my,vector<Bullet>& bullets){
    Bullet bul[2];//发射两个子弹
    bul[0].speed=bul[1].speed=my.fire_pos;
    bul[0].center=bul[1].center=my.center;
    if(fabs(bul[0].speed.x)<0.001){//竖直方向发射
        bul[0].speed.x=1;
        bul[1].speed.x=-1;
    }else{
        bul[0].speed.y=1;
        bul[1].speed.y=-1;
    }

    bullets.push_back(bul[0]);
    bullets.push_back(bul[1]);

}

/**
 * 函数：update_blackhole
 * 函数描述：更新黑洞位置
 * 参数描述：
 *      blackholes: 当前在地图上的所有黑洞
 */
void update_blackhole(vector<Blackhole>& blackholes,const argument& arg){
    for(int i=0;i<blackholes.size();i++){
        blackholes[i].update(blackholes);
    }
    for(int i=0;i<blackholes.size();i++){
        blackholes[i].update_pos(arg);
    }
}
/**
 * 函数：update_bullet
 * 函数描述：更新子弹位置，出界的，碰黑洞就消失
 * 参数描述：
 *      my:玩家本机
 *      bullets：当前运动中的子弹
 *      collide_check:地图方格中的子弹数量
 *      sparks：当前未消失的爆炸火花
 *      blackholes:当前地图上的所有黑洞
 */
void update_bullet(Myplane& my,vector<Bullet>& bullets,vector<vector<vector<int>>>& collide_check,
                   vector<Spark>& sparks,vector<Blackhole>& blackholes,const argument& arg){
    static int cnt=0;
    if(!(my.fire_pos==Point(0,0,0))){
        if(cnt==arg.fire_turn){
            cnt=0;
            fire(my,bullets);//two bullet is push into vector<bullets>
        }
        cnt++;
    }
    for(size_t i=0;i<bullets.size();i++){//将所有准备出界的丢弃
        bullets[i].update(blackholes);
    }
    for(size_t i=0;i<bullets.size();i++){//将所有准备出界的丢弃
        while(i<bullets.size() && bullets[i].update_pos(arg)>0){//&&两边不能反了不然越界
            create_sparks(sparks,8,bullets[i].center,1,1,0);//死前产生个火花
            bullets[i]=bullets[bullets.size()-1];
            bullets.pop_back();
        }
    }
    for(int i=0;i<blackholes.size();i++){//碰撞黑洞
        for(int j=0;j<bullets.size();j++){
            while(j<bullets.size() && distance(bullets[j].center,blackholes[i].center)<bullets[j].radius+blackholes[i].radius){//&&两边不能反了不然越界
                create_sparks(sparks,8,bullets[j].center,1,1,0);//死前产生个火花
                blackholes[i].radius-=0.5;//让黑洞变小
                bullets[j]=bullets[bullets.size()-1];
                bullets.pop_back();
            }
        }
    }

    //更新碰撞格子
    for(size_t i=0;i<arg.WIDTH/arg.CSIZE;i++){
        for(size_t j=0;j<arg.HEIGHT/arg.CSIZE;j++){
        //    printf("!!%d,%d size:%d\n",i,j,collide_check[i][j].size());
            collide_check[i][j].clear();
        }
    }

    for(size_t i=0;i<bullets.size();i++){
        int xi=bullets[i].center.x/arg.CSIZE;
        int yi=bullets[i].center.y/arg.CSIZE;
        collide_check[xi][yi].push_back(i);
    }
}



/**
 * 函数：update_fan
 * 函数描述：更新敌人位置，并且根据子弹位置,黑洞位置,来进行碰撞检测，碰撞则消失
 * 参数描述：
 *      my: 玩家飞机
 *      bullets:当前飞行中的子弹集合
 *      fans:当前存活的敌人集合
 *      collide_check:网格中的子弹数量统计
 *      collide_check_fan：网格中的敌人数量统计
 *      sparks: 当前未消失的爆炸火花集合
 *      blackholes: 当前存在的黑洞集合
 */
int update_fan(Myplane& my,vector<Bullet>& bullets,vector<Fan>& fans,vector<vector<vector<int>>>& collide_check,
                vector<vector<vector<int>>>& collide_check_fan,vector<Spark>& sparks,vector<Blackhole>& blackholes,const argument& arg){

    int hit_times=0;
    for(size_t i=0;i<fans.size();i++){
        fans[i].update(fans[i],my,fans,collide_check_fan,bullets,collide_check,blackholes,arg);//更新速度方向和位置
        fans[i].update_pos(arg);

        //与子弹碰撞检测
        int xi=fans[i].center.x/arg.CSIZE;
        int yi=fans[i].center.y/arg.CSIZE;
        int bul_mark=-1;//与子弹碰撞检测结果，碰到的子弹下标
        for(int j=-1;j<2;j++){//九个格子里的子弹
            if(j+xi<0)j++;
            if(j+xi>=arg.WIDTH/arg.CSIZE)break;
            for(int k=-1;k<2;k++){
                if(k+yi<0)k++;
                if(k+yi>=arg.HEIGHT/arg.CSIZE)break;

                for(int l=0;l<collide_check[j+xi][k+yi].size();l++){
                    if(collide_check[j+xi][k+yi][l]<bullets.size()){//有可能已经撞过了没了
                        if(distance(bullets[collide_check[j+xi][k+yi][l]].center,fans[i].center)
                                <bullets[collide_check[j+xi][k+yi][l]].radius+fans[i].radius){
                            bul_mark=collide_check[j+xi][k+yi][l];
                            break;
                        }
                    }

                }
                if(bul_mark!=-1)break;
            }
            if(bul_mark!=-1)break;
        }
        if(bul_mark!=-1){//撞上了子弹，消失 子弹暂时留着，如果现在移动最后的补上去会导致检测出错（因为碰撞格子里的下标是对应着的）
            //针对PINK1的特殊处理
            create_sparks(sparks,8,bullets[bul_mark].center,1,1,0);//死前产生个子弹的火花
            bullets[bul_mark]=Bullet();
            bullets[bul_mark].center=Point(-100,-100,0);//到时候update bullet就会被删除了

            fan_die(sparks,fans,i);
            hit_times++;
            i--;
        //    printf("hit!");
            continue;
        }
        //跟玩家做碰撞检测
        if(distance(my.center,fans[i].center)<(fans[i].radius+my.radius)*3/4){
            return -i-1;
        }

        for(int j=0;j<blackholes.size();j++){//跟黑洞做碰撞检测
            if(i<fans.size() && distance(fans[i].center,blackholes[j].center)<fans[i].radius+blackholes[j].radius){
                fan_die(sparks,fans,i);
                i--;
                blackholes[j].radius+=0.5;//让黑洞变大
                break;
            }
        }
    }
    //清除出界死亡的
    for(int i=0;i<fans.size();i++){
        if(fans[i].center.x<80||fans[i].center.x>arg.WIDTH-80||fans[i].center.y<80||fans[i].center.y>arg.HEIGHT-80){
            fans[i]=fans[fans.size()-1];
            fans.pop_back();
            i--;
        }
    }
    //更新碰撞检测方格
    for(size_t i=0;i<arg.WIDTH/arg.CSIZE;i++){
        for(size_t j=0;j<arg.HEIGHT/arg.CSIZE;j++){
        //    printf("!!%d,%d size:%d\n",i,j,collide_check[i][j].size());
            collide_check_fan[i][j].clear();
        }
    }
    for(size_t i=0;i<fans.size();i++){
        int xi=fans[i].center.x/arg.CSIZE;
        int yi=fans[i].center.y/arg.CSIZE;
        collide_check_fan[xi][yi].push_back(i);
    }
    return hit_times;
}

/**
 * 函数： create_fan
 * 函数描述：每隔一段时间创建敌人及黑洞
 * 参数描述：
 *      @param my 玩家操控的飞机
 *      @param fans 当前的敌人LIST
 *      @param blackholes 当前的黑洞LIST
 */
void create_fan(Myplane& my,vector<Fan>& fans,vector<Blackhole>& blackholes,const argument& arg,level_data& lev){//关卡系统
    static int cnt=0,turn=0;
    if(cnt%arg.fan_turn==0){//帧数到达设定值就是敌人的回合了
        turn++;
        printf("\rturn=%d",turn);
        if(turn==lev.turns[lev.last_turn]){//根据记录加入敌人
            int t=lev.last_turn;
            Fan a(lev.types[t]);
            a.center=lev.positions[t];
            //游戏位置从100到WIDTH/HEIGHT-100 因此要进行一定处理
            if(a.center.x<0){//负值表示从最大处减
                a.center.x=arg.WIDTH-120+a.center.x;
            }else{
                a.center.x+=120;
            }
            if(a.center.y<0){
                a.center.y=arg.HEIGHT-120+a.center.y;
            }else{
                a.center.y+=120;
            }
            printf("x:%lf y:%lf\n",a.center.x,a.center.y);
            for(int i=0;i<lev.numbers[t];i++){
                fans.push_back(a);
            }
            lev.add_last_turn();
            if(lev.last_turn==0)turn=-10;//关卡记录打完了就重新回去
        }

        if(turn%10==0){
            if(blackholes.size()<3){//黑洞
                Blackhole b1(Point(rand()%(arg.WIDTH-200)+100,rand()%(arg.HEIGHT-200)+100,0),Point(rand()%1000/1000.0,rand()%1000/1000.0,0));
                while(distance(b1.center,my.center)<150){
                    b1.center=Point(rand()%(arg.WIDTH-200)+100,rand()%(arg.HEIGHT-200)+100,0);
                }
                blackholes.push_back(b1);
            }
        }
        if(turn>100000000)turn=0;
    }
    cnt++;
}

/**
 * 函数： fan_die
 * 函数描述：敌人死亡后更新现存敌人数据，产生爆炸火花
 * 参数描述：
 *  @param sparks ： 地图上的爆炸火花集合
 *  @param fans ： 地图上的敌人集合
 *  @param idx ：死亡的敌人编号
 */
void fan_die(vector<Spark>& sparks,vector<Fan>& fans,const int idx){
    Point center=fans[idx].center;

    switch (fans[idx].type_of_fan) {
    case 0://这里是我用到的参数
        create_sparks(sparks,8,center,0,1,0,rand()%90/90.0/180*PI);
        break;
    case 1:
        create_sparks(sparks,6+rand()%6,center,0,1,0,fans[idx].angle/180*PI);
        break;
    case 2:
        create_sparks(sparks,6+rand()%6,center,1,0,0,fans[idx].angle/180*PI);
        break;
    }
    fans[idx].center=Point();//通过后续清除
}

/**
 * 函数： create_sparks
 * 函数描述: 产生爆炸火花
 * 参数描述：
 *  @param sparks：当前火花集合
 *  @param num：产生的数量
 *  @param center:敌人死亡时的位置
 *  @param r
 *  @param g
 *  @param b ： 爆炸火花的颜色
 *  @param angle：调整火花位置及运动方向
 */
void create_sparks(vector<Spark>& sparks,const int num,const Point& center,double r,double g,double b,double angle){//后面可以各自写函数各自产生不同火花
    for(int i=0;i<num;i++){
        sparks.push_back(Spark(Point(cos((2*PI)*i/num+angle)*10+center.x,sin((2*PI)*i/num+angle)*10+center.y,0),Point(cos((2*PI)*i/num+angle),sin((2*PI)*i/num+angle),0)*(rand()%8),r,g,b));
    }
}

/**
 * 函数：update_spark
 * 函数描述：更新当前所有的火花状态，删除地图上停止运动的火花
 * 参数描述：
 *  @param sparks：在地图上的火花集合
 *  @param blackholes：当前地图上存在的黑洞集合
 */
void update_spark(vector<Spark>& sparks,vector<Blackhole>& blackholes,const argument& arg){
    for(int i=0;i<sparks.size();i++){
        sparks[i].update(blackholes);
        sparks[i].update_pos(arg);
        if(distance(sparks[i].speed,Point(0,0,0))<0.1){//即将消失
            sparks[i]=sparks[sparks.size()-1];
            sparks.pop_back();
            i--;
        }
    }
}

/**
 * 函数： background
 * 函数描述：更新背景点的加速度、速度、位置
 * 参数描述：
 *      background_points：背景点集合
 *      blackholes：黑洞集合
 *      bullets：子弹集合
 *      collide_check_bullet：每个网格内的子弹数
 */
void background(vector<vector<Background_point>>& background_points,vector<Blackhole>& blackholes,vector<Bullet>& bullets,vector<vector<vector<int>>>& collide_check_bullet,const argument& arg){
    for(int i=0;i<arg.WIDTH/arg.BSIZE;i++){
        for(int j=0;j<arg.HEIGHT/arg.BSIZE;j++){
            background_points[i][j].update(blackholes,bullets,collide_check_bullet,arg);
            background_points[i][j].update_pos();
        }
    }

}
/*
绘制边界
*/
void draw_border(const argument& arg){
    glColor3f(1,1,1);
    glBegin(GL_QUADS);//左边边框
    glVertex3f(70,80,0);
    glVertex3f(70,arg.HEIGHT-70,0);
    glVertex3f(80,arg.HEIGHT-70,0);
    glVertex3f(80,80,0);
    glEnd();
    glBegin(GL_QUADS);//右边边框
    glVertex3f(arg.WIDTH-80,70,0);
    glVertex3f(arg.WIDTH-70,70,0);
    glVertex3f(arg.WIDTH-70,arg.HEIGHT-80,0);
    glVertex3f(arg.WIDTH-80,arg.HEIGHT-80,0);
    glEnd();
    glBegin(GL_QUADS);//上边边框
    glVertex3f(80,arg.HEIGHT-80,0);
    glVertex3f(80,arg.HEIGHT-70,0);
    glVertex3f(arg.WIDTH-70,arg.HEIGHT-70,0);
    glVertex3f(arg.WIDTH-70,arg.HEIGHT-80,0);
    glEnd();
    glBegin(GL_QUADS);//下边边框
    glVertex3f(70,70,0);
    glVertex3f(70,80,0);
    glVertex3f(arg.WIDTH-80,80,0);
    glVertex3f(arg.WIDTH-80,70,0);
    glEnd();
}
