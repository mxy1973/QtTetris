#include "widget.h"
#include "ui_widget.h"
#include <stdlib.h>
#include <QPainter>
#include <time.h>
#include <QKeyEvent>
#include <QMessageBox>

//  L方块
int item1[4][4] =
{
    {0,1,0,0,},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};

int item2[4][4] =
{
    {0,0,1,0,},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}
};

// 田方块
int item3[4][4] =
{
    {0,1,1,0,},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
};

// I方块
int item4[4][4] =
{
    {0,1,0,0,},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0}
};

// S方块
int item5[4][4] =
{
    {0,1,0,0,},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}
};

int item6[4][4] =
{
    {0,0,1,0,},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
};

// 山方块
int item7[4][4] =
{
    {0,1,0,0,},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
};

// 块拷贝
inline void block_cpy(int block[4][4], int block_id[4][4])
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            block[i][j] = block_id[i][j];
        }
    }
}


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    ui->setupUi(this);
    resize(BLOCK_SIZE*AREA_COL + MARGIN * 4 + BLOCK_SIZE*4, BLOCK_SIZE * AREA_ROW + MARGIN * 2);//设置场景

    InitGame();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::paintEvent(QPaintEvent * event)
{
    //游戏背景
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(MARGIN, MARGIN,AREA_COL * BLOCK_SIZE , AREA_ROW * BLOCK_SIZE);

    //下一个设置
    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(next_block[i][j] == 1)
                painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE + j * BLOCK_SIZE, MARGIN + BLOCK_SIZE *i, BLOCK_SIZE,BLOCK_SIZE);
        }
    }

    //set score
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(QRect(MARGIN*3 + BLOCK_SIZE*AREA_COL,  BLOCK_SIZE * AREA_ROW/2-BLOCK_SIZE *2, BLOCK_SIZE*4,BLOCK_SIZE*4), Qt::AlignCenter,"score: "+QString::number(score));

    for(int i = 0; i < AREA_ROW; i++)
    {
        for(int j = 0; j < AREA_COL; j++)
        {
            if(game_area[i][j] == 1){    // 运动块
                painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
                painter.drawRect(MARGIN + j * BLOCK_SIZE, MARGIN + i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

            }

            else if(game_area[i][j]==2) //稳定块
            {
                painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
                painter.drawRect(j*BLOCK_SIZE+MARGIN,i*BLOCK_SIZE+MARGIN,BLOCK_SIZE,BLOCK_SIZE);
            }
        }
    }
}

void Widget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == game_timer)
    {
        BlockMove(DOWN);
    }
    if(event->timerId() == paint_timer)
        update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Up:
            BlockMove(UP);
            break;
    case Qt::Key_Left:
        BlockMove(LEFT);
        break;
    case Qt::Key_Right:
        BlockMove(RIGHT);
        break;
    case Qt::Key_Down:
        BlockMove(DOWN);
        break;
    default:
        break;
    }
}


void Widget::CreateBlock(int block[4][4], int block_id)
{
    switch (block_id)
    {
        case 0:
            block_cpy(block, item1);
            break;
        case 1:
            block_cpy(block, item2);
            break;
        case 2:
            block_cpy(block, item3);
            break;
        case 3:
            block_cpy(block, item4);
            break;
        case 4:
            block_cpy(block, item5);
            break;
        case 5:
            block_cpy(block, item6);
            break;
        case 6:
            block_cpy(block, item7);
            break;
    }

}

void Widget::GetBoder(int block[4][4], Border& boder)
{
   for(int i = 0; i < 4; i++)
   {
       for(int j = 0; j < 4; j++)
       {
           if(block[i][j] == 1)
           {
               boder.dbound = i;
               break;
           }
       }
   }

   for(int i = 3; i >= 0; i--)
   {
       for(int j = 0; j < 4; j++)
       {
           if(block[i][j] == 1)
           {
               boder.ubound = i;
               break;
           }
       }
   }

   for(int i = 0; i < 4; i++)
   {
       for(int j = 0; j < 4; j++)
       {
           if(block[j][i] == 1)
           {
               boder.rbound = i;
               break;
           }
       }
   }

   for(int i = 3; i >= 0; i--)
   {
       for(int j = 0; j < 4; j++)
       {
           if(block[j][i] == 1)
           {
               boder.lbound = i;
               break;
           }
       }
   }


}

void Widget::InitGame()
{
    // 初始化游戏区
    for(int i = 0; i < AREA_ROW; i++)
        for(int j = 0; j < AREA_COL; j++)
            game_area [i][j] = 0;


    speed_ms = 800;  //下落时间间隔
    refresh_ms = 30;  //刷新画面
    score = 0;    //分数

    srand(time(0));

    GameStart();
}

void Widget::ResetBlock()
{
    //拷贝块
    block_cpy(cur_block, next_block);
    GetBoder(cur_block, cur_border);

    // 产生下一块
    int block_id = rand() % 7;
    CreateBlock(next_block, block_id);

    //块初始坐标
    block_point start_point;
    start_point.pos_x = AREA_COL/2 - 2;
    start_point.pos_y = 0;
    block_pos = start_point;

}

void Widget::GameStart()
{
    startTimer(speed_ms);
    int block_id = rand()%7;
    CreateBlock(next_block, block_id);
    ResetBlock();

    game_timer=startTimer(speed_ms); //开启游戏timer
    paint_timer=startTimer(refresh_ms);

}

void Widget::GameOver()
{
    killTimer(game_timer);
    killTimer(paint_timer);
    QMessageBox::information(this, "failed","game over");
}


void Widget::BlockRotate(int block[4][4])
{
    int temp[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[3 - j][i] = block[i][j];
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            block[i][j] = temp[i][j];
        }
    }


}

void Widget::BlockMove(Direction dir)
{
    switch (dir)
    {
        case DOWN:

            // 判断是否到达边界或者发生碰撞
            if(IsCollide(block_pos.pos_x, block_pos.pos_y, DOWN) || block_pos.pos_y + cur_border.dbound == AREA_ROW -1)
            {
                ConvertStable(block_pos.pos_x, block_pos.pos_y);
                ResetBlock();
                break;
            }

            //清空上方
            for(int j = cur_border.lbound; j <= cur_border.rbound; j++)
            {

                    game_area[block_pos.pos_y][block_pos.pos_x + j] = 0;
            }
            block_pos.pos_y += 1;
            //块拷贝给游戏区
            for(int i = 0; i < 4; i++)
            {
                for(int j = cur_border.lbound; j <= cur_border.rbound; j++ )
                {
                    if(block_pos.pos_y + i<= AREA_ROW - 1 && game_area[block_pos.pos_y + i][block_pos.pos_x + j] != 2)
                        game_area[block_pos.pos_y + i][block_pos.pos_x + j] = cur_block[i][j];
                }
            }
            break;  //down


        case UP:
            if(IsCollide(block_pos.pos_x, block_pos.pos_y, UP))
                break;

            // 旋转
            BlockRotate(cur_block);
            for (int i = 0;  i< 4; i++) {
                for (int j = 0; j < 4; j++) {

                        game_area[block_pos.pos_y + i][block_pos.pos_x + j] = cur_block[i][j];

                }
            }

            GetBoder(cur_block, cur_border);
        break;

    case LEFT:
        //判断是否到达边界以及碰撞
        if(block_pos.pos_x + cur_border.lbound == 0 || IsCollide(block_pos.pos_x, block_pos.pos_y, LEFT))
            break;

         // 清空右方
        for (int i = cur_border.ubound; i <= cur_border.dbound; i++) {
            game_area[block_pos.pos_y + i][block_pos.pos_x + 3] = 0;
        }


        block_pos.pos_x -=1;

        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
                   for(int j=0;j<4;j++)
                       if(block_pos.pos_x+j>=0&&game_area[block_pos.pos_y+i][block_pos.pos_x+cur_border.lbound+j]!=2) //注意场景数组不越界
                           game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];

        break;

    case RIGHT:
        if(block_pos.pos_x + cur_border.rbound == AREA_COL - 1 || IsCollide(block_pos.pos_x, block_pos.pos_y, RIGHT))
            break;


        //清空左方
        for (int i = cur_border.ubound; i <= cur_border.dbound; i++) {
            game_area[block_pos.pos_y + i][block_pos.pos_x] = 0;
        }

        block_pos.pos_x += 1;

        for (int i = cur_border.ubound; i <= cur_border.dbound; i++) {
            for(int j = 0; j < 4; j++)
            {
                game_area[block_pos.pos_y + i][block_pos.pos_x + j] = cur_block[i][j];
            }
        }  //right
        break;

    default:
        break;




    } //switch

    //更新画面及分数
    int count = 0;
    int dis_count = AREA_ROW - 1; //从最下方开始
    while(dis_count >= 0)
    {

        bool dis_line = true;

        for (int j = 0; j < AREA_COL; j++) {
            //判定是否有空格
            if(game_area[dis_count][j] == 0)
            {
                dis_line = false;
                dis_count--;
                break;
            }
        }
        //如无空格
        if(dis_line)
        {
            count += 1;
            //上方区拷贝给下方区
            for (int k = dis_count; k >= 0; k--) {
                for (int j = 0; j < AREA_COL; ++j) {
                    game_area[k][j] = game_area[k-1][j];
                }
            }

        }
    } //while

    score += count*10; //分数
    for (int j = 0; j < AREA_COL; j++) {
        if(game_area[0][j] == 2)
            GameOver();
    }

}

//转为稳定态
void Widget::ConvertStable(int x, int y){
    for(int i = cur_border.ubound; i <= cur_border.dbound; i++)
    {
        for(int j = cur_border.lbound; j <= cur_border.rbound; j++)
        {
            if(cur_block[i][j] == 1)
                game_area[y + i][x + j] = 2;
        }
    }
}

//是否发生碰撞
bool Widget::IsCollide(int x, int y, Direction dir)
{
    //用一个临时块去判断
    int temp_block[4][4];
    Border temp_border;
    block_cpy(temp_block, cur_block);
    GetBoder(temp_block, temp_border);

    switch (dir) {
    case UP:
        BlockRotate(temp_block);
        GetBoder(temp_block,temp_border);
        break;

    case DOWN:
        y += 1;
        break;

    case LEFT:
        x -= 1;
        break;

    case RIGHT:
        x += 1;
        break;

    default:
        break;
    }

   for(int i = temp_border.ubound; i <= temp_border.dbound; i++)
   {
       for( int j = temp_border.lbound; j <= temp_border.rbound; j++)
       {
           if(game_area[y + i][x + j] == 2 && temp_block[i][j] ==1)
               return true;
       }
   }

   return false;

}

