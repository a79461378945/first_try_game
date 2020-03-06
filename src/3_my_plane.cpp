#include "my_plane.h"
#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

extern GLuint myplane_texname;
Myplane::Myplane(){
    FILE* ifile=NULL;
    fopen_s(&ifile,"plane.txt","r");
    if(ifile==NULL){
        radius=32;
        power=4.0;
        fiction=0.5;
        gravity=0.2;
        printf("can not open plane.txt,using default plane\n");
        return;
    }
    fscanf(ifile,"radius=%d power=%lf fiction=%lf gravity=%lf",&radius,&power,&fiction,&gravity);
    printf("reading from plane:radius=%d power=%lf fiction=%lf gravity=%lf\n",radius,power,fiction,gravity);
    fclose(ifile);
}

/**
 * 函数： Myplane::draw
 * 函数描述：绘制玩家操控的飞机
 */
void Myplane::draw(){
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);//开启纹理贴图 为后面做准备
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
    glBindTexture(GL_TEXTURE_2D,myplane_texname);

    double angel=asin(speed.y/pow((speed.y*speed.y+speed.x*speed.x),0.5))*180/3.14159;//根据角度旋转
    if(speed.x<0.001){
        angel=180-angel;
    }
    glTranslatef(center.x,center.y,0);
    glRotatef(-90+angel,0,0,1);
    glTranslatef(-center.x,-center.y,0);

    glBegin(GL_QUADS);//纹理贴图
    glVertex3f(center.x-32,center.y-32,1);
    glTexCoord2f(0,0);
    glVertex3f(center.x-32,center.y+32,1);
    glTexCoord2f(1,0);
    glVertex3f(center.x+32,center.y+32,1);
    glTexCoord2f(1,1);
    glVertex3f(center.x+32,center.y-32,1);
    glTexCoord2f(0,1);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

/**
 * 函数： Myplane::update
 * 函数描述：更新飞机的加速度与所处网格
 * 函数参数：
 *      vector<Blackhole>& blackholes: 当前地图上存在的黑洞list
 */
void Myplane::update(vector<Blackhole>& blackholes){
    add_speed=direction;//加速度方向就是玩家控制方向

    //将加速度大小调整为动力大小
    if(pow((add_speed.x*add_speed.x+add_speed.y*add_speed.y),0.5)>0.1){//避免除以0
        add_speed.x = (add_speed.x)/pow((add_speed.x*add_speed.x+add_speed.y*add_speed.y),0.5)*power;
        add_speed.y = (add_speed.y)/pow((add_speed.x*add_speed.x+add_speed.y*add_speed.y),0.5)*power;
    }
    Point invicible_power=add_speed*0.1;
    for(int i=0;i<blackholes.size();i++){
        Point collide_speed=blackholes[i].center-center;
        collide_speed=collide_speed/pow((0.001+distance(blackholes[i].center,center)),2)*gravity*pow(blackholes[i].radius,2);//吸引力
        add_speed=add_speed+collide_speed;//加上这个物体对本物体的吸引力
    }
    if(add_speed.x*direction.x+direction.y*add_speed.y<0){
        add_speed=invicible_power;
    }
    //算上摩擦力
    add_speed.x-= speed.x*fiction;
    add_speed.y-= speed.y*fiction;

 //   printf("myplane %lf,%lf\n",add_speed.x,add_speed.y);


}
