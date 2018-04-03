#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

const int AREA_ROW = 20;     //游戏区高度
const int AREA_COL = 10;    //游戏区宽度
const int BLOCK_SIZE = 25;    //方块大小
const int MARGIN = 5;        //边距

//运动方向
enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT

};

//块的边界
struct Border
{
    int dbound;
    int ubound;
    int lbound;
    int rbound;

};

//块坐标
struct block_point
{
    int pos_x;
    int pos_y;
};

namespace Ui {
class widget;
}

class Widget : public QWidget
{
    Q_OBJECT


public:
    void InitGame();  //游戏初始化
    void GameStart();  //开始游戏
    void GameOver();   //游戏结束

    void BlockMove(Direction dir);  //移动方向


    void ConvertStable(int x, int y); //转为稳定态
    void CreateBlock(int block[4][4], int block_id); // 创建块
    void ResetBlock(); //设置块
    void BlockRotate(int block[4][4]);   //旋转块


    void GetBoder(int block[4][4], Border &boder);  //获取块边界

    bool IsCollide(int x, int y,Direction dir);  //判断块是否碰撞

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::widget *ui;


public:
    virtual void paintEvent(QPaintEvent * event);
    virtual void timerEvent(QTimerEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    int next_block[4][4];
    int cur_block[4][4];
    int score;
    int game_area[AREA_ROW][AREA_COL];   //游戏区域
    int speed_ms;
    Border cur_border;
    block_point block_pos;

    int game_timer;

    int paint_timer;

    int refresh_ms;

 };

#endif // WIDGET_H
