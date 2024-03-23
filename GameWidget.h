#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QList>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class GameWidget; }
QT_END_NAMESPACE

const int BLOCK_SIZE        = 40;  // 单个方块单元的边长
const int MARGIN            = 5;   // 场景边距
const int AREA_ROW          = 20;  // 场景行数
const int AREA_COL          = 20;  // 场景列数
const int TIME_INTERVAL     = 200; // 定时器间隔时间
const int INIT_SIZE         = 5;   // 初始长度

enum Direction {                   // 方向
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class GameWidget : public QWidget {
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event);  // 界面刷新
    virtual void keyPressEvent(QKeyEvent *event); // 键盘监听
    void InitGame();                              // 初始化游戏
    void PauseResumeGame();                       // 暂停和恢复游戏
    void GameOver();                              // 游戏结束
    void GenerateFood();                          // 随机产生果实
    bool IsGameOver();                            // 判断是否游戏结束

private:
    Ui::GameWidget *ui;
    QTimer *gameTimer;                            // 游戏计时器
    bool isPause;                                 // 是否暂停
    QPoint foodPoint;                             // 食物出现坐标
    bool foodStyle;                               // 食物样式
    QList<QPoint> snake;                          // 贪吃蛇结构
    Direction direction;                          // 方向
    int score;                                    // 分数
    bool foodCanMove;                             // 食物是否可可动
    bool canThroughWall;                          // 蛇是否可可穿墙

private slots:
    void GameUpdate();                            // 贪吃蛇增长,食物移动
};
#endif // GAME_WIDGET_H
