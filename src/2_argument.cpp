#include "argument.h"
#include <fstream>
using namespace std;
argument::argument()
{
    FILE* ifile=NULL;
    fopen_s(&ifile,"arg.txt","r");
    if(ifile==NULL){
        printf("can not open arg.txt,using default argument\n");
        WIDTH=1920;
        HEIGHT=1080;
        CSIZE=40;
        BSIZE=20;
        fire_turn=4;
        fan_turn=20;
        return;
    }
    fscanf_s(ifile,"WIDTH=%d HEIGHT=%d CSIZE=%d BSIZE=%d fire_turn=%d fan_turn=%d",
             &WIDTH,&HEIGHT,&CSIZE,&BSIZE,&fire_turn,&fan_turn);
    printf("WIDTH=%d HEIGHT=%d CSIZE=%d BSIZE=%d fire_turn=%d fan_turn=%d\n",WIDTH,HEIGHT,CSIZE,BSIZE,fire_turn,fan_turn);
    fclose(ifile);
}
