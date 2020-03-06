

# 玩法说明
在game下面找到game1.exe，双击即可开始游戏

游戏玩法：开始后点击start game

进入游戏后中间的飞机为玩家控制机体，wasd控制移动，方向键控制射击子弹

红色圈是黑洞，会吸引物体

其他几何体为敌人，用子弹消灭他们并避免被他们碰到，否则游戏结束

空格暂停/开始 ，暂停时可以在开始的窗口看到目前分数，点击stop game结束游戏



## 进阶
arg.txt里面有游戏环境的参数配置， CSIZE  是格子法进行碰撞检测的格子大小，BSZIE为背景点的间隔
fire_turn为每多少帧射击一次，越大则射击间隔越长
fan_turn为每多少帧算作敌人一回合

custom_level.txt里面是关卡配置，第一个数为生成敌人的条目数（即下面有多少行）
turn为在第几回合生成， type为生成的敌人类型编号（对应fan_data.txt顺序） 
x,y为生成位置，若x为负数表示横坐标实际为WIDTH+x ；y为负数表示纵坐标实际为HEIGHT+y
number为生成的敌人数目

fan_data.txt为各类敌人的配置
开头number of type:3表示总共有3种敌人（编号从0开始，0，1，2）
type name:名字
picture name:要用的贴图文件名
radius= 半径大小，碰撞检测时用
power= 动力大小
fiction= 摩擦系数
repulsive= 相互挤压作用力系数
gravity= 引力系数
follow_rate= 追随玩家的意愿系数
dodge_others_rate= 躲避周围其他敌人的意愿系数
dodge_bullets_rate= 躲避子弹的意愿系数

plane.txt为玩家本身配置
radius=碰撞半径
power=动力大小
fiction=摩擦系数
gravity=引力系数

# 代码部分
编译需要用到glew，可百度搜索安装glew，里面就有安装方法，几个文件很快就弄好了

.cpp大致的按引用层次给了个数字在名字前面

程序有两窗体，myglwidget为游戏本体，myui为用来提供开始、结束按钮和显示计分的窗口

myglwidget里面就存储了各种游戏元素，在argument,level_data,read_bmp这三个分别完成读取配置属性，关卡信息，贴图纹理。game提供了游戏过程中的函数，如更新黑洞、敌人等的函数。

my_plane,fan,bullet,blackhole分别为玩家飞机，敌人，子弹，黑洞的实现，这四者之间能相互碰撞，都是游戏里的实体

entity_and_point,background_point,sparks分别为游戏实体+点，背景控制点，火花的实现
