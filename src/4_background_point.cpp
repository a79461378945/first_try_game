#include "background_point.h"
#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

Background_point::Background_point(const Point& origin,double gravity_hole,double gravity_bul){
    speed=add_speed=Point(); //速度与加速度均为0
    origin_pos=center=origin;
    gravity_blackhole=gravity_hole;
    gravity_bullet=gravity_bul;
    fiction=0.1;
}

/**
 * 函数： Background_point::update
 * 函数描述：更新当前网格内的背景的加速度，受黑洞引力和子弹排斥力影响
 * 参数描述：
 *  @param blackholes ： 地图上存在的黑洞集合
 *  @param bullets ： 地图上存在的子弹集合
 *  @param collide_check_bullet ： 包含每个网格内的子弹数量信息
 */
void Background_point::update(vector<Blackhole>& blackholes,vector<Bullet>& bullets,vector<vector<vector<int>>>& collide_check_bullet,const argument& arg){
    //黑洞引力
    for(int i=0;i<blackholes.size();i++){
        Point collide_speed=blackholes[i].center-center;
        double dis=distance(blackholes[i].center,center);
        double kdis=dis*pow(15*blackholes[i].radius,-3);//15倍半径以外无引力
        if(dis>0 && 1/(dis*dis)>kdis){
            collide_speed=collide_speed*  (1/(dis*dis)-kdis)  *pow(blackholes[i].radius,2);//吸引力
            add_speed=add_speed+collide_speed*5000;
        }

    }
//    for(int i=0;i<blackholes.size();i++){
//        if(distance(blackholes[i].center,center)>0.5 && distance(blackholes[i].center,center)<blackholes[i].radius*15){
//            Point collide_speed=blackholes[i].center-center;//吸引到上空
//            collide_speed=collide_speed/pow((0.001+distance(blackholes[i].center,center)),2)*gravity_blackhole*pow(blackholes[i].radius,2);//吸引力
//            add_speed=add_speed+collide_speed;//加上这个物体对本物体的吸引力
//        }
//    }
    //子弹排斥力
    int xi=center.x/arg.CSIZE;
    int yi=center.y/arg.CSIZE;
    for(int j=-1;j<2;j++){//九个格子里的其他fan
        if(j+xi<0)j++;//下溢出，则加一
        if(j+xi>=arg.WIDTH/arg.CSIZE)break;//超出范围了break
        for(int k=-1;k<2;k++){
            if(k+yi<0)k++;//同上
            if(k+yi>=arg.HEIGHT/arg.CSIZE)break;//同上
            for(int l=0;l<collide_check_bullet[j+xi][k+yi].size();l++){//对格子里所有物体进行检测
                if(collide_check_bullet[j+xi][k+yi][l]<bullets.size()){//做个检查，避免越界
//                    if(distance(bullets[collide_check_bullet[j+xi][k+yi][l]].center,center)
//                            <bullets[collide_check_bullet[j+xi][k+yi][l]].radius+100){//中心距离小于两者半径之和则说明已经碰撞
//                        Point collide_speed=center-bullets[collide_check_bullet[j+xi][k+yi][l]].center;
//                        collide_speed=collide_speed/pow((0.001+distance(bullets[collide_check_bullet[j+xi][k+yi][l]].center,center)),1)*gravity_bullet*30;//排斥力
//                        add_speed=add_speed+collide_speed;//加上这个物体对本物体的排斥力
//                    }
                    Point collide_speed=center-bullets[collide_check_bullet[j+xi][k+yi][l]].center;
                    double dis=distance(bullets[collide_check_bullet[j+xi][k+yi][l]].center,center);
                    int ra=10;
                    double kdis=dis*pow(15*ra,-3);//15倍半径以外无引力
                    if(dis>0 && 1/(dis*dis)>kdis){
                        collide_speed=collide_speed*  (1/(dis*dis)-kdis)  *pow(ra,2);//排斥力
                        add_speed=add_speed+collide_speed*100;
                    }
                }

            }
        }
    }
    //
    add_speed=add_speed+(origin_pos-center)*distance(origin_pos,center)*distance(origin_pos,center);
    add_speed.x-= speed.x*fiction;
    add_speed.y-= speed.y*fiction;
    add_speed.z-= speed.z*fiction;
}

/**
 * 函数： Background_point::update_pos
 * 函数描述：与Entity类的update_pos类似，通过加速度更新速度，再根据速度更新位置。
 */
int Background_point::update_pos(){
    speed.x+=add_speed.x*0.5;
    speed.y+=add_speed.y*0.5;
    add_speed.x=add_speed.y=0;
    if(pow((speed.x*speed.x+speed.y*speed.y),0.5)<2 && distance(center,origin_pos)<5){//避免出现除以0，调整一下速度大小
        center=origin_pos;
    }else{
        speed.x = (speed.x)/pow((speed.x*speed.x+speed.y*speed.y),0.5);
        speed.y = (speed.y)/pow((speed.x*speed.x+speed.y*speed.y),0.5);
        center=center+speed*0.5;
    }

    return 0;
}
