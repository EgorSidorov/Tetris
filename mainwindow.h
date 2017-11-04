#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "object.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPathItem>
#include <QTimer>
#include <QSwipeGesture>
#include <QMediaPlayer>
#include <QGridLayout>
#include <QPushButton>
#include <QSlider>
#include <QFile>
#include <QSize>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    int skorost=6;//скорость игры
    double nowscale=1;//текущее увеличение в graphicsview
    double lastscale=1;//прошлое увеличение в graphicsview
    bool menu=false;//текущее состояние меню, открыто или нет
    bool increaselevel=false;//состояние режима авто-увеличения уровня
    QTimer *timer=new QTimer();//игровой таймер для очередного падения фигуры
    QByteArray bestscore;//лучший счет
    QGridLayout *menulayout;
    QGridLayout *imagelayout;//иконка звука
    QPushButton *newgame;//кнопка "новая игра"
    QSlider *volume;//слайдер звука
    QPushButton* exit;//кнопка "выйти"
    QPushButton* autoincreaselevel;//кнопка "авто-увеличение уровня"
    QWidget* menuwindow;//виджет меню
    QMediaPlayer mediaplayer;//аудио плеер
    int mousepositionx,mousepositiony;
    qreal lastwidth, lastheight;
    QGraphicsScene mygraphicsscene;//сцена с полем для игры
    QGraphicsScene nextfiguregraphicsscene;//сцена для отображения следующей фигуры
    QGraphicsRectItem *rect[20][10];//массив квадратов поля игры
    QGraphicsRectItem *nextfigurerect[4][3];//массив квадратов следующей фигуры
    objectt* figure=new objectt;//объект с методами управления текущей фигурой

    ~MainWindow();
    explicit MainWindow(QWidget *parent = 0);

protected:

    bool eventFilter(QObject *obj, QEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void paintEvent(QPaintEvent *event);//перерисовка окна при изменении размеров

private slots:

    void Esc_clicked();

    void slotincreaselevel();//увеличение скорости игры

    void on_start_clicked(bool checked);

    void on_down_clicked();

    void slotnewfigure();//действия при создании новой фигуры

    void nextsong(QMediaPlayer::State state);//повтор песни

    void on_fast_clicked();

    void on_slow_clicked();

    void on_newgame_clicked();

private:

    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
