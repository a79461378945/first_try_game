#ifndef BULLET_H
#define BULLET_H

#include "entity_and_point.h"
#include "blackhole.h"
#include <vector>
using namespace std;

/**
 * 类：Bullet
 * 类描述：继承自实体类Entity，对象表示当前射出的子弹
 */

struct Bullet:public Entity{
    Bullet();
    void draw();
    void update(vector<Blackhole>& blackholes);
};

#endif // BULLET_H
