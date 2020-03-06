
#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "fan.h"
extern GLubyte fan_texture[64][64][4];
extern GLuint fan_texname;
vector<string> Fan::type_name;
vector<string> Fan::picture_name;
vector<GLuint> Fan::tex_name;
vector<int>    Fan::radiuses;
vector<double> Fan::repulsives;
vector<double> Fan::powers;
vector<double> Fan::fictions;
vector<double> Fan::gravitys;
vector<double> Fan::follow_rate;
vector<double> Fan::dodge_others_rate;
vector<double> Fan::dodge_bullets_rate;
/**
 * 函数： Fan::Fan
 * 函数描述：初始化敌人属性，不同的敌人有不同的初始属性，在一定程度上决定了其表现。
 * 参数描述：
 *      type：指定敌人类型
 */
Fan::Fan(int type){
    gravity=1.5;
    type_of_fan = type;
    radius=radiuses[type];
    power=powers[type];
    fiction=fictions[type];
    repulsive=repulsives[type];
    gravity=gravitys[type];
    draw=draw_fan;
    update=update_fan;
}
void Fan::load(){
    FILE* ifile=NULL;
    fopen_s(&ifile,"fan_data.txt","r");
    if(ifile==NULL){
        radiuses.push_back(32);
        powers.push_back(4.0);
        fictions.push_back(0.5);
        repulsives.push_back(500);
        gravitys.push_back(1);
        follow_rate.push_back(1);
        dodge_others_rate.push_back(0);
        dodge_bullets_rate.push_back(0);
        printf("can not open plane.txt,using default fan\n");
        return;
    }
    int number_of_type=0;
    fscanf(ifile," number of type:%d ",&number_of_type);
    char ts1[200]="",ts2[200]="";
    for(int i=0;i<number_of_type;i++){
        fscanf(ifile," type name:%s ",ts1);
        type_name.push_back(string(ts1));
        fscanf(ifile," picture name:%s ",ts2);
        picture_name.push_back(string(ts2));
        int t1;
        double t2;
        fscanf(ifile," radius=%d",&t1);
        radiuses.push_back(t1);


        fscanf(ifile," power=%lf",&t2);
        powers.push_back(t2);
        fscanf(ifile," fiction=%lf",&t2);
        fictions.push_back(t2);
        fscanf(ifile," repulsive=%lf",&t2);
        repulsives.push_back(t2);
        fscanf(ifile," gravity=%lf",&t2);
        gravitys.push_back(t2);
        fscanf(ifile," follow_rate=%lf",&t2);
        follow_rate.push_back(t2);
        fscanf(ifile," dodge_others_rate=%lf",&t2);
        dodge_others_rate.push_back(t2);
        fscanf(ifile," dodge_bullets_rate=%lf",&t2);
        dodge_bullets_rate.push_back(t2);

        printf("reading from fan_data:type name:%s picture name:%s radius=%d power=%lf fiction=%lf repulsive=%lf gravity=%lf\n"
               ,type_name[i].c_str(),picture_name[i].c_str(),radiuses[i],powers[i],fictions[i],repulsives[i],gravitys[i]);
        printf("follow_rate=%lf dodge_others_rate=%lf dodge_bullets_rate=%lf\n",follow_rate[i],dodge_others_rate[i],dodge_bullets_rate[i]);
    }

    fclose(ifile);
}



void draw_fan(Fan& fan1){

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);//开启纹理贴图 为后面做准备
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
    glBindTexture(GL_TEXTURE_2D,Fan::tex_name[fan1.type_of_fan]);

    double angel=asin(fan1.speed.y/pow((fan1.speed.y*fan1.speed.y+fan1.speed.x*fan1.speed.x),0.5))*180/3.14159;//根据角度旋转
    if(fan1.speed.x<0.001){
        angel=180-angel;
    }
    glTranslatef(fan1.center.x,fan1.center.y,0);
    glRotatef(-90+angel,0,0,1);
    glTranslatef(-fan1.center.x,-fan1.center.y,0);

    glBegin(GL_QUADS);//纹理贴图
    glVertex3f(fan1.center.x-32,fan1.center.y-32,1);
    glTexCoord2f(0,0);
    glVertex3f(fan1.center.x-32,fan1.center.y+32,1);
    glTexCoord2f(1,0);
    glVertex3f(fan1.center.x+32,fan1.center.y+32,1);
    glTexCoord2f(1,1);
    glVertex3f(fan1.center.x+32,fan1.center.y-32,1);
    glTexCoord2f(0,1);
    glEnd();
    glDisable(GL_TEXTURE_2D);


    glPopMatrix();
}


Point dodge_others(Fan& fan1,vector<Fan>& fans,vector<vector<vector<int>>>& collide_check_fan,const argument& arg){
    //根据附近fan的位置调整加速度，以此模拟互相挤压，从而减少重叠
    Point result;
    int xi=fan1.center.x/arg.CSIZE;
    int yi=fan1.center.y/arg.CSIZE;
    for(int j=-1;j<2;j++){//九个格子里的其他fan
        if(j+xi<0)j++;//下溢出，则加一
        if(j+xi>=arg.WIDTH/arg.CSIZE)break;//超出范围了break
        for(int k=-1;k<2;k++){
            if(k+yi<0)k++;//同上
            if(k+yi>=arg.HEIGHT/arg.CSIZE)break;//同上
            for(int l=0;l<collide_check_fan[j+xi][k+yi].size();l++){//对格子里所有物体进行检测
                if(collide_check_fan[j+xi][k+yi][l]<fans.size()){//做个检查，避免越界
                    if(distance(fans[collide_check_fan[j+xi][k+yi][l]].center,fan1.center)
                            <fans[collide_check_fan[j+xi][k+yi][l]].radius+fan1.radius){//中心距离小于两者半径之和则说明已经碰撞
                        Point collide_speed=fan1.center-fans[collide_check_fan[j+xi][k+yi][l]].center;
                        collide_speed=collide_speed/(0.001+distance(fans[collide_check_fan[j+xi][k+yi][l]].center,fan1.center))*fan1.repulsive;//排斥力
                        result=result+collide_speed;//加上这个物体对本物体的排斥力
                    }
                }
            }
        }
    }
    return result;
}
Point dodge_bullets(Fan& fan1,vector<Bullet>& bullets,vector<vector<vector<int>>>& collide_check_bullet,const argument& arg){
    int xi=fan1.center.x/arg.CSIZE;
    int yi=fan1.center.y/arg.CSIZE;
    Point result;
    for(int j=-1;j<2;j++){//九个格子里的bullets
        if(j+xi<0)j++;//下溢出，则加一
        if(j+xi>=arg.WIDTH/arg.CSIZE)break;//超出范围了break
        for(int k=-1;k<2;k++){
            if(k+yi<0)k++;//同上
            if(k+yi>=arg.HEIGHT/arg.CSIZE)break;//同上
            for(int l=0;l<collide_check_bullet[j+xi][k+yi].size();l++){//对格子里所有物体进行检测
                if(collide_check_bullet[j+xi][k+yi][l]<bullets.size()){//做个检查，避免越界
                    if(distance(bullets[collide_check_bullet[j+xi][k+yi][l]].center,fan1.center)
                            <bullets[collide_check_bullet[j+xi][k+yi][l]].radius+fan1.radius+100){//中心距离小于两者半径之和则说明已经碰撞
                        Point collide_speed=fan1.center-bullets[collide_check_bullet[j+xi][k+yi][l]].center;
                        collide_speed=collide_speed/(0.001+distance(bullets[collide_check_bullet[j+xi][k+yi][l]].center,fan1.center))*fan1.repulsive;//排斥力
                        result=result+collide_speed;//加上这个物体对本物体的排斥力
                    }
                }
            }
        }
    }
    return result;
}
Point follow_player(Fan& fan1,const Myplane& my,const argument& arg){
    return my.center-fan1.center;
}

void update_fan(Fan& fan1,const Myplane& my,vector<Fan>& fans,vector<vector<vector<int>>>& collide_check_fan,vector<Bullet>& bullets,
                   vector<vector<vector<int>>>& collide_check_bullet,vector<Blackhole>& blackholes,const argument& arg){

    //先清零
    fan1.add_speed=Point();
    //追主机，互相远离，躲子弹 三个影响动力方向
    if(fabs(Fan::follow_rate[fan1.type_of_fan])>0.01){
        fan1.add_speed=fan1.add_speed+follow_player(fan1,my,arg)*Fan::follow_rate[fan1.type_of_fan];
    }
    if(fabs(Fan::dodge_others_rate[fan1.type_of_fan])>0.01){
        fan1.add_speed=fan1.add_speed+dodge_others(fan1,fans,collide_check_fan,arg)*Fan::dodge_others_rate[fan1.type_of_fan];
    }
    if(fabs(Fan::dodge_bullets_rate[fan1.type_of_fan])>0.01){
        fan1.add_speed=fan1.add_speed+dodge_bullets(fan1,bullets,collide_check_bullet,arg)*Fan::dodge_bullets_rate[fan1.type_of_fan];
    }

    //将加速度的大小设定为动力大小
    if((fan1.add_speed.x*fan1.add_speed.x+fan1.add_speed.y*fan1.add_speed.y)>0.01){//避免出现除以0
        fan1.add_speed.x = (fan1.add_speed.x)/pow((fan1.add_speed.x*fan1.add_speed.x+fan1.add_speed.y*fan1.add_speed.y),0.5)*fan1.power;
        fan1.add_speed.y = (fan1.add_speed.y)/pow((fan1.add_speed.x*fan1.add_speed.x+fan1.add_speed.y*fan1.add_speed.y),0.5)*fan1.power;
    }

    //黑洞引力
    for(int i=0;i<blackholes.size();i++){
        Point collide_speed=blackholes[i].center-fan1.center;
        double dis=distance(blackholes[i].center,fan1.center);
        double kdis=dis*pow(15*blackholes[i].radius,-3);//15倍半径以外无引力
        if(dis>0 && 1/(dis*dis)>kdis){
            collide_speed=collide_speed*  (1/(dis*dis)-kdis)  *fan1.gravity*pow(blackholes[i].radius,2);//吸引力
            fan1.add_speed=fan1.add_speed+collide_speed;
        }
    }
    //最后算上摩擦力
    fan1.add_speed.x-= fan1.speed.x*fan1.fiction;
    fan1.add_speed.y-= fan1.speed.y*fan1.fiction;

}

