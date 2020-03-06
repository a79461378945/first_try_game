#include "level_data.h"

level_data::level_data()
{
    last_turn=0;
}
//读个关卡信息
void level_data::load(string name){
    FILE* ifile=NULL;
    ifile=fopen(name.c_str(),"r");
    if(ifile==NULL){
        printf("can not open %s,using default level settings\n",name.c_str());
        int turn=0,type=0,x=10,y=10,number=1;
        turns.push_back(turn);
        types.push_back(type);
        Point a(x,y);
        positions.push_back(a);
        numbers.push_back(number);
        return;
    }
    int num=0;
    fscanf(ifile,"%d",&num);
    for(int i=0;i<num;i++){
        int turn,type,x,y,number;
        fscanf(ifile," turn=%d type=%d x=%d y=%d number=%d ",&turn,&type,&x,&y,&number);//前后空格去了就读不对了
        printf("reading...turn=%d type=%d x=%d y=%d number=%d\n",turn,type,x,y,number);
        turns.push_back(turn);
        types.push_back(type);
        Point a(x,y);
        positions.push_back(a);
        numbers.push_back(number);
    }
    return;
}
void level_data::add_last_turn(){
    last_turn++;
    if(last_turn>=turns.size()){
        last_turn=0;
    }
}
