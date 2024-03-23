#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QPainter>
#include <QPushButton>
#include <QLabel>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    // 设置窗口图标
    this->setWindowIcon(QIcon(":/image/res/icon.png"));
    // 设置窗口标题
    this->setWindowTitle("Snake VS Rats");
    // 设置窗口大小
    this->setFixedSize(MARGIN * 25 + (AREA_COL + 3) * BLOCK_SIZE, MARGIN * 2 + AREA_ROW * BLOCK_SIZE);
    // 开始按钮
    QPushButton *startGameBtn = new QPushButton(this);
    startGameBtn->resize(150, 150);
    startGameBtn->setStyleSheet("border-image:url(:/image/res/start.png);");
    connect(startGameBtn, &QPushButton::clicked, this, [=]() {
        gameWidget = new GameWidget(this);
        gameWidget->show();
    });
    QLabel *rules = new QLabel(this);
    rules->setFont(QFont("Microsoft YAHEI", 25));
    rules->move(0, 150);
    rules->setText("操作方法\nW: 向上\nS: 向下\nA: 向左\nD: 向右\nP: 暂停\n");
}

MainWidget::~MainWidget() { delete ui; }

void MainWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // 启用抗锯齿效果
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(Qt::yellow));
    painter.drawRect(0, 0, this->size().width(), this->size().height());
    painter.drawImage(QRectF(QPoint(0, 0), QSize(this->size().width(), this->size().height())), QImage(":/image/res/logo.png"));
}
