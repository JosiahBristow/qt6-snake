#include "GameWidget.h"

#include <QMessageBox>
#include <QString>
#include <QPainter>
#include <ctime>
#include <QCheckBox>
#include <QPushButton>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent) {
    // 初始化场景界面尺寸
    this->setFixedSize(MARGIN * 25 + (AREA_COL + 3) * BLOCK_SIZE, MARGIN * 2 + AREA_ROW * BLOCK_SIZE);
    // 初始化游戏
    InitGame();
    QPushButton *startGame = new QPushButton("开始\\暂停", this);
    startGame->move(MARGIN * 3 + AREA_COL * BLOCK_SIZE, MARGIN + 2 * BLOCK_SIZE + 2 * BLOCK_SIZE);
    connect(startGame, &QCheckBox::clicked, this, [=]() {
        if (gameTimer->isActive())
            gameTimer->stop();
        else
            gameTimer->start();
    });
    QPushButton *back = new QPushButton("返回", this);
    back->move(MARGIN * 3 + AREA_COL * BLOCK_SIZE + startGame->size().width() + MARGIN, MARGIN + 2 * BLOCK_SIZE + 2 * BLOCK_SIZE);
    connect(back, &QCheckBox::clicked, this, [=]() {
        this->close();
    });
    QCheckBox *foodMove = new QCheckBox("食物可动", this);
    foodMove->move(MARGIN * 3 + AREA_COL * BLOCK_SIZE, MARGIN + 2 * BLOCK_SIZE + 4 * BLOCK_SIZE);
    connect(foodMove, &QCheckBox::stateChanged, this, [=]() {
        if (this->foodCanMove)
            this->foodCanMove = false;
        else
            this->foodCanMove = true;
    });
    QCheckBox *throughWall = new QCheckBox("不可穿墙", this);
    throughWall->setCheckState(Qt::Checked);
    throughWall->move(MARGIN * 3 + AREA_COL * BLOCK_SIZE, MARGIN + 2 * BLOCK_SIZE + 5 * BLOCK_SIZE);
    connect(throughWall, &QCheckBox::stateChanged, this, [=]() {
        if (this->canThroughWall)
            this->canThroughWall = false;
        else
            this->canThroughWall = true;
    });
}

void GameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::black));
    // 绘制游戏场景
    painter.drawImage(QRectF(QPoint(0, 0), QSize(this->size().width(), this->size().height())), QImage(":/image/res/background.jpg"));
    // 绘制辅助网格
    for (int i = 0; i <= AREA_COL; i++)
        painter.drawLine(QPoint(i * BLOCK_SIZE +  MARGIN, MARGIN), QPoint(i * BLOCK_SIZE +  MARGIN, AREA_ROW * BLOCK_SIZE + MARGIN));
    for (int i = 0; i <= AREA_ROW; i++)
        painter.drawLine(QPoint(MARGIN, i * BLOCK_SIZE +  MARGIN), QPoint(MARGIN + AREA_COL * BLOCK_SIZE, i * BLOCK_SIZE +  MARGIN));
    // 绘制贪吃蛇
    for (int i = 0; i < snake.size(); i++) {
        if (i == 0) { // 画蛇头
            switch(direction) {
                case UP:
                painter.drawImage(QRectF(QPointF(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE), QSizeF(BLOCK_SIZE, BLOCK_SIZE)), QImage(":/image/res/headup.png"));
                    break;
                case DOWN:
                    painter.drawImage(QRectF(QPointF(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE), QSizeF(BLOCK_SIZE, BLOCK_SIZE)), QImage(":/image/res/headdown.png"));
                    break;
                case LEFT:
                    painter.drawImage(QRectF(QPointF(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE), QSizeF(BLOCK_SIZE, BLOCK_SIZE)), QImage(":/image/res/headleft.png"));
                    break;
                case RIGHT:
                    painter.drawImage(QRectF(QPointF(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE), QSizeF(BLOCK_SIZE, BLOCK_SIZE)), QImage(":/image/res/headright.png"));
                    break;
            }
        } else {
            if (i == snake.size() - 1) { // 画蛇尾
                if (snake[i].x() == snake[i - 1].x() + 1 && snake[i].y() == snake[i - 1].y())
                    painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/tailleft.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
                else if(snake[i].x() == snake[i - 1].x() - 1 && snake[i].y() == snake[i - 1].y())
                    painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/tailright.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
                else if(snake[i].x() == snake[i - 1].x() && snake[i].y() == snake[i - 1].y() - 1)
                    painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/taildown.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
                else if(snake[i].x() == snake[i - 1].x() && snake[i].y() == snake[i - 1].y() + 1)
                    painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/tailup.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
            } else { // 画蛇身
                    if ((snake[i].x() == snake[i - 1].x() && snake[i].y() == snake[i - 1].y() - 1 &&
                         snake[i].x() == snake[i + 1].x() - 1 && snake[i].y() == snake[i + 1].y()) ||
                        (snake[i].x() == snake[i - 1].x() - 1 && snake[i].y() == snake[i - 1].y() &&
                         snake[i].x() == snake[i + 1].x() && snake[i].y() == snake[i + 1].y() - 1))
                        painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/tl_corner.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
                    else if ((snake[i].x() == snake[i - 1].x() - 1 && snake[i].y() == snake[i - 1].y() &&
                              snake[i].x() == snake[i + 1].x() && snake[i].y() == snake[i + 1].y() + 1) ||
                             (snake[i].x() == snake[i - 1].x() && snake[i].y() == snake[i - 1].y() + 1 &&
                              snake[i].x() == snake[i + 1].x() - 1 && snake[i].y() == snake[i + 1].y()))
                        painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/bl_corner.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
                    else if ((snake[i].x() == snake[i - 1].x() + 1 && snake[i].y() == snake[i - 1].y() &&
                              snake[i].x() == snake[i + 1].x() && snake[i].y() == snake[i + 1].y() - 1) ||
                             (snake[i].x() == snake[i - 1].x() && snake[i].y() == snake[i - 1].y() - 1 &&
                              snake[i].x() == snake[i + 1].x() + 1 && snake[i].y() == snake[i + 1].y()))
                        painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/tr_corner.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
                    else if ((snake[i].x() == snake[i - 1].x() && snake[i].y() == snake[i - 1].y() + 1 &&
                              snake[i].x() == snake[i + 1].x() + 1 && snake[i].y() == snake[i + 1].y()) ||
                             (snake[i].x() == snake[i - 1].x() + 1 && snake[i].y() == snake[i - 1].y() &&
                              snake[i].x() == snake[i + 1].x() && snake[i].y() == snake[i + 1].y() + 1))
                        painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/br_corner.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
                    else if (snake[i].y() == snake[i - 1].y() && snake[i].x() != snake[i - 1].x())
                        painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/h_body.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
                    else if (snake[i].x() == snake[i - 1].x() && snake[i].y() != snake[i - 1].y())
                        painter.drawImage(MARGIN + snake[i].x() * BLOCK_SIZE, MARGIN + snake[i].y() * BLOCK_SIZE, QImage(":/image/res/v_body.png").scaled(QSize(BLOCK_SIZE, BLOCK_SIZE)));
            }
        }
    }
    // 绘制食物
    if (foodStyle)
        painter.drawImage(QRectF(QPointF(MARGIN + foodPoint.x() * BLOCK_SIZE, MARGIN + foodPoint.y() * BLOCK_SIZE), QSizeF(BLOCK_SIZE, BLOCK_SIZE)), QImage(":/image/res/rat.png"));
    else
        painter.drawImage(QRectF(QPointF(MARGIN + foodPoint.x() * BLOCK_SIZE, MARGIN + foodPoint.y() * BLOCK_SIZE), QSizeF(BLOCK_SIZE, BLOCK_SIZE)), QImage(":/image/res/rats.png"));
    // 绘制游戏分数
    painter.setPen(Qt::black);
    painter.setFont(QFont("Microsoft YAHEI", 20));
    painter.drawText(MARGIN * 3 + AREA_COL * BLOCK_SIZE, MARGIN + 2 * BLOCK_SIZE, "score: " + QString::number(score));
    // 绘制等级分数
    painter.setPen(Qt::black);
    painter.setFont(QFont("Microsoft YAHEI", 20));
    painter.drawText(MARGIN * 3 + AREA_COL * BLOCK_SIZE, MARGIN + 2 * BLOCK_SIZE + BLOCK_SIZE, "level: " + QString::number(score / 5));
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    // 贪吃蛇的方向是个状态机,注意各个方向之间切换有限制
    switch (event->key()) {
        case Qt::Key_W:
            if(direction != DOWN)
                direction = UP;
            break;
        case Qt::Key_S:
            if(direction != UP)
                direction = DOWN;
            break;
        case Qt::Key_A:
            if(direction != RIGHT)
                direction = LEFT;
            break;
        case Qt::Key_D:
            if(direction != LEFT)
                direction = RIGHT;
            break;
        case Qt::Key_P:
            PauseResumeGame();
            break;
        default:
            break;
    }
}

bool GameWidget::IsGameOver() {
    int x = snake.front().x();
    int y = snake.front().y();
    if (!canThroughWall) {
            // 出边界
            if (x < 0 || x > AREA_COL - 1 || y < 0 || y > AREA_ROW - 1)
                return true;
    }
    // 撞了自己的尾巴
    if (snake[0] == snake[snake.size() - 1])
        return true;
    return false;
}

void GameWidget::GameUpdate() {
    // 贪吃蛇移动的策略是每次删除尾部，然后添加新的头部，维护一个动态链表
    switch (direction) {
        case UP:
            if (canThroughWall && snake[0].y() == 0) {
                snake.push_front(QPoint(snake.front().x(), AREA_ROW - 1));
            } else
                snake.push_front(QPoint(snake.front().x(), snake.front().y() - 1));
            break;
        case DOWN:
            if (canThroughWall && snake[0].y() == AREA_ROW - 1) {
                snake.push_front(QPoint(snake.front().x(), 0));
            } else
                snake.push_front(QPoint(snake.front().x(), snake.front().y() + 1));
            break;
        case LEFT:
            if (canThroughWall && snake[0].x() == 0) {
                snake.push_front(QPoint(AREA_COL - 1, snake.front().y()));
            } else
                snake.push_front(QPoint(snake.front().x() - 1, snake.front().y()));
            break;
        case RIGHT:
            if (canThroughWall && snake[0].x() == AREA_COL - 1) {
                snake.push_front(QPoint(0, snake.front().y()));
            } else
                snake.push_front(QPoint(snake.front().x() + 1, snake.front().y()));
            break;
        default:
            break;
    }
    // 如果吃到了食物，则尾部不删除，否则删除尾部更新头部
    if (snake.contains(foodPoint)) {
        if (foodStyle) {
            score += 1; // 得分
            GenerateFood(); // 重新生成食物
        } else {
            score += 3; // 得分
            switch (direction) {
            case UP:
                if (canThroughWall && snake[0].y() == 0) {
                    snake.push_front(QPoint(snake.front().x(), AREA_ROW - 1));
                    snake.push_front(QPoint(snake.front().x(), AREA_ROW - 1));
                } else {
                    snake.push_front(QPoint(snake.front().x(), snake.front().y() - 1));
                    snake.push_front(QPoint(snake.front().x(), snake.front().y() - 1));
                }
                break;
            case DOWN:
                if (canThroughWall && snake[0].y() == AREA_ROW - 1) {
                    snake.push_front(QPoint(snake.front().x(), 0));
                    snake.push_front(QPoint(snake.front().x(), snake.front().y() + 1));
                } else {
                    snake.push_front(QPoint(snake.front().x(), snake.front().y() + 1));
                    snake.push_front(QPoint(snake.front().x(), snake.front().y() + 1));
                }
                break;
            case LEFT:
                if (canThroughWall && snake[0].x() == 0) {
                    snake.push_front(QPoint(AREA_COL - 1, snake.front().y()));
                    snake.push_front(QPoint(AREA_COL - 1, snake.front().y()));
                } else {
                    snake.push_front(QPoint(snake.front().x() - 1, snake.front().y()));
                    snake.push_front(QPoint(snake.front().x() - 1, snake.front().y()));
                }
                break;
            case RIGHT:
                if (canThroughWall && snake[0].x() == AREA_COL - 1) {
                    snake.push_front(QPoint(0, snake.front().y()));
                    snake.push_front(QPoint(0, snake.front().y()));
                } else {
                    snake.push_front(QPoint(snake.front().x() + 1, snake.front().y()));
                    snake.push_front(QPoint(snake.front().x() + 1, snake.front().y()));
                }
                break;
            default:
                break;
            }
            GenerateFood(); // 重新生成食物
        }
    } else
        snake.pop_back();
    // 游戏是否结束
    if (IsGameOver()) {
        GameOver();
        return; // 赶在重绘之前跳出函数，防止贪吃蛇真的出界
    }

    if (foodCanMove) {
        // 食物移动
        srand(time(NULL));
        int flag = 1 + rand() % 8;
        switch(flag) {
            case 1: // 上
            if (foodPoint.y() > 0)
                    foodPoint.setY(foodPoint.y() - 1);
                break;
            case 2: // 下
                if (foodPoint.y() < AREA_ROW - 1)
                    foodPoint.setY(foodPoint.y() + 1);
                break;
            case 3: // 左
                if (foodPoint.x() > 0)
                    foodPoint.setX(foodPoint.x() - 1);
                break;
            case 4: // 右
                if (foodPoint.x() < AREA_COL - 1)
                    foodPoint.setX(foodPoint.x() + 1);
                break;
            case 5: // 左上
                if (foodPoint.x() > 0 && foodPoint.y() > 0) {
                    foodPoint.setX(foodPoint.x() - 1);
                    foodPoint.setY(foodPoint.y() - 1);
                }
                break;
            case 6: // 左下
                if (foodPoint.x() > 0 && foodPoint.y() < AREA_ROW - 1) {
                    foodPoint.setX(foodPoint.x() - 1);
                    foodPoint.setY(foodPoint.y() + 1);
                }
                break;
            case 7: // 右上
                if (foodPoint.y() > 0 && foodPoint.x() < AREA_COL - 1) {
                    foodPoint.setX(foodPoint.x() + 1);
                    foodPoint.setY(foodPoint.y() - 1);
                }
                break;
            case 8: // 右下
                if (foodPoint.x() < AREA_COL - 1  && foodPoint.y() < AREA_ROW - 1) {
                    foodPoint.setX(foodPoint.x() + 1);
                    foodPoint.setY(foodPoint.y() + 1);
                }
                break;
        }
    }
    update(); // 重绘
}

void GameWidget::GameOver() {
    gameTimer->stop();
    QMessageBox *gameover = new QMessageBox();
    gameover->setIconPixmap(QPixmap(":/image/res/gameover.png"));
    gameover->show();
}

void GameWidget::InitGame() {
    // 初始化贪吃蛇，初始长度为INIT_SIZE，注意头部在前，尾部在后
    for (int i = INIT_SIZE; i >= 0; i--)
        snake.push_back(QPoint(i, 0));
    // 初始方向为右
    direction = RIGHT;
    // 初始化食物
    srand(time(nullptr));
    GenerateFood();
    // 初始化游戏得分
    score = 0;
    // 初始化暂停变量
    isPause = false;
    // 初始化蛇是否可穿墙
    canThroughWall = false;
    // 初始化食物是否可动
    foodCanMove = false;
    // 初始化食物样式
    foodStyle = true;
    // 初始化计时器
    gameTimer = new QTimer(this);
    gameTimer->setInterval(TIME_INTERVAL);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(GameUpdate()));
}

void GameWidget::GenerateFood() {
    // 随机样式
    foodStyle = rand() % 2 == 0;
    // 随机产生位置
    foodPoint.setX(rand() % AREA_COL);
    foodPoint.setY(rand() % AREA_ROW);
    // 如果与贪吃蛇位置冲突，重新生成
    if (snake.contains(foodPoint))
        GenerateFood();
}

void GameWidget::PauseResumeGame() {
    // 暂停和恢复定时器
    if(!isPause) {
        isPause = !isPause;
        gameTimer->stop();
    } else {
        isPause = !isPause;
        gameTimer->start(TIME_INTERVAL);
    }
}
