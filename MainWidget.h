#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <GameWidget.h>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

    public:
        explicit MainWidget(QWidget *parent = nullptr);
        ~MainWidget();
        virtual void paintEvent(QPaintEvent *event);

    private:
        Ui::MainWidget *ui;
        GameWidget *gameWidget; // 游戏主页面
};

#endif // MAINWIDGET_H
