#ifndef GAME_H
#define GAME_H

#include "entity_and_point.h"
#include "background_point.h"
#include "blackhole.h"
#include "bullet.h"
#include "fan.h"
#include "my_plane.h"
#include "spark.h"
#include "argument.h"
#include "level_data.h"
#include <vector>
using namespace std;

void fire(Myplane& my,vector<Bullet>& bullets);//玩家开火
void update_blackhole(vector<Blackhole>& blackholes,const argument& arg);//更新黑洞
void update_bullet(Myplane& my,vector<Bullet>& bullets,vector<vector<vector<int>>>& collide_check,
                   vector<Spark>& sparks,vector<Blackhole>& blackholes,const argument& arg);//子弹移动更新，检查是否出界或碰到黑洞
//敌人移动更新，与子弹、黑洞、玩家的碰撞检测
int update_fan(Myplane& my,vector<Bullet>& bullets,vector<Fan>& fans,vector<vector<vector<int>>>& collide_check,
                vector<vector<vector<int>>>& collide_check_fan,vector<Spark>& sparks,vector<Blackhole>& blackholes,const argument& arg);
//更新火花位置
void update_spark(vector<Spark>& sparks,vector<Blackhole>& blackholes,const argument& arg);

//令某个下标的敌人设置位置为非法位置（从而在后续处理的时候可以将其剔除
void fan_die(vector<Spark>& sparks,vector<Fan>& fans,const int idx);
void create_sparks(vector<Spark>& sparks,const int num,const Point& center,double r=1,double g=1,double b=1,double angle=0);//产生火花
void create_fan(Myplane& my,vector<Fan>& fans,vector<Blackhole>& blackholes,const argument& arg,level_data& lev);//生成敌人
//更新背景控制点
void background(vector<vector<Background_point>>& background_points,vector<Blackhole>& blackholes,
                vector<Bullet>& bullets,vector<vector<vector<int>>>& collide_check_bullet,const argument& arg);
//绘制四条边界
void draw_border(const argument& arg);
#endif // GAME_H
