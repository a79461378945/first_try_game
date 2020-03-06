#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H
#include <vector>
#include <string>
#include "entity_and_point.h"
using namespace std;
/*
存储关卡信息的类
*/
class level_data
{

public:
    vector<int> turns;//出现的回合
    vector<int> types;//种类
    vector<Point> positions;//位置
    vector<int> numbers;//数量
    int last_turn;//上一次出怪的回合（即turns[idx]的idx）
    level_data();
    void load(string name);//从文件中加载关卡信息
    void add_last_turn();//给last_turn加一，出界则变0
};

#endif // LEVEL_DATA_H
