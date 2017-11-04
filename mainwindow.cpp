#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*добавление иконки для dekstop*/
    QIcon icon;
    QPixmap iconphoto(":/photoicon.png");
    icon.addPixmap(iconphoto);
    this->setWindowIcon(icon);

    /*добавление музыки*/
    mediaplayer.setMedia(QUrl("qrc:/tetris_music.mp3"));
    connect(&mediaplayer,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(nextsong(QMediaPlayer::State)));
    mediaplayer.play();

    /*позиция мыши для отслеживания свайпа*/
    mousepositionx=0;mousepositiony=0;
    connect(figure,SIGNAL(signalnewfigure()),this,SLOT(slotnewfigure()));
    connect(figure,SIGNAL(signalgameover()),this,SLOT(Esc_clicked()));

    QFile file("score.txt");
    /*если файла со счетом нет(первый запуск приложения), то записать счет 0*/
    if(!file.exists()){
        QByteArray writee;
        writee.append("0");
        if(file.open(QIODevice::WriteOnly)){
            file.write(writee);
            file.close();
        }
    }

    /*переписать лучший счет из файла в окно игры*/
    if(file.open(QIODevice::ReadOnly)){
        bestscore.append(file.readAll());
    }
    ui->best_score->setText("Best score: "+QString(bestscore));

    /*настройки окна отображения фигур*/
    ui->mygraphicsview->setFrameStyle(0);//окно без рамок
    ui->nextfiguregraphicsView->setFrameStyle(0);
    this->setAutoFillBackground(true);//устанавливаем цвет фона
    this->setMinimumHeight(750);//минимальный размер окна
    this->setMinimumWidth(400);

    /*фильтр обработки свайпа в области всей игры*/
    this->installEventFilter(this);
    ui->mygraphicsview->viewport()->installEventFilter(this);
    ui->nextfiguregraphicsView->viewport()->installEventFilter(this);

    qreal size=39;//начальный размер квадратика фигуры
    qreal nextfiguresize=23;//начальный размер квадратика следующей фигуры
    ui->mygraphicsview->setScene(&mygraphicsscene);//установить графическую сцену в виджет
    ui->nextfiguregraphicsView->setScene(&nextfiguregraphicsscene);

    /*добавить квадратики на игровое поле*/
    for(int i=0;i<20;i++){
        for(int j=-4;j<=5;j++){
            rect[i][j+4]=mygraphicsscene.addRect(j*size,size*i,size,size);
            rect[i][j+4]->setBrush(QBrush(QColor(230,255,220)));
        }
    }
    figure->setrect(rect);

    /*добавить квадратики на поле следующей фигуры*/
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            nextfigurerect[i][j]=nextfiguregraphicsscene.addRect(j*nextfiguresize,nextfiguresize*i,nextfiguresize,nextfiguresize);
            nextfigurerect[i][j]->setPen(Qt::NoPen);
        }
    }
    figure->setnextfigurerect(nextfigurerect);

    /*начальные размеры графической сцены (для пересчета размеров в paintevent)*/
    lastwidth=400; lastheight=800;


}
void MainWindow::paintEvent(QPaintEvent *event){

    double nowheight= this->height();
    double nowwidth= this->width();
    double nowwidthgraphicsview=ui->mygraphicsview->width();
    double nowheightgraphicsview=ui->mygraphicsview->height();

    double matwidth=nowwidthgraphicsview/lastwidth;
    double matheight=nowheightgraphicsview/lastheight;
    ui->mygraphicsview->scale(matwidth,matheight);
    ui->nextfiguregraphicsView->scale(matwidth,matheight);
    lastheight= ui->mygraphicsview->height();
    lastwidth= ui->mygraphicsview->width();
    int nowresizex=(nowwidth*900/480-nowheight)/4;
    int nowresizey=(nowheight*480/900-nowwidth)/4;


    //установка размеров
    double scale=1;
if((nowwidth*900/480-nowheight)>0){
    scale=(nowheight/900);
} else {
    scale=(nowwidth/480);
}
if(this->heightMM()>150){
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            nextfigurerect[i][j]->setRect(j*23,23*i,23,23);
        }
    }
ui->mygraphicsview->setFixedSize(QSize(392*scale,784*scale));
ui->nextfiguregraphicsView->setFixedSize(QSize(392*3/16*scale,784*scale*4/32));
ui->best_score->setFixedSize(390*scale,45*scale);
ui->score->setFixedSize(390*scale,45*scale);
ui->down->setFixedSize(70*scale,25*scale);
ui->start->setFixedSize(70*scale,25*scale);
ui->fast->setFixedSize(70*scale,25*scale);
ui->slow->setFixedSize(70*scale,25*scale);
} else if(this->heightMM()>100){
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            nextfigurerect[i][j]->setRect(j*23,23*i,23,23);
        }
    }
    ui->mygraphicsview->setFixedSize(QSize(392*scale,784*scale));
    ui->nextfiguregraphicsView->setFixedSize(QSize(392*3/16*scale*10/7,784*scale*4/32*10/7));
    ui->best_score->setFixedSize(390*scale,45*scale);
    ui->score->setFixedSize(390*scale,45*scale);
    ui->down->setFixedSize(100*scale,55*scale);
    ui->start->setFixedSize(100*scale,55*scale);
    ui->fast->setFixedSize(100*scale,55*scale);
    ui->slow->setFixedSize(100*scale,55*scale);
} else {
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            nextfigurerect[i][j]->setRect(j*23*100/70,23*i*100/70,23*100/70,23*100/70);
        }
    }
    ui->mygraphicsview->setFixedSize(QSize(392*scale,784*scale));
    ui->nextfiguregraphicsView->setFixedSize(QSize(392*3/16*scale*16/7,784*scale*4/32*16/7));;
    ui->best_score->setFixedSize(390*scale,45*scale);
    ui->score->setFixedSize(390*scale,45*scale);
    ui->down->setFixedSize(160*scale,80*scale);
    ui->start->setFixedSize(160*scale,80*scale);
    ui->fast->setFixedSize(160*scale,80*scale);
    ui->slow->setFixedSize(160*scale,80*scale);
}
    this->ui->gridLayoutmain->setContentsMargins(nowresizex+10,nowresizey+5,nowresizex+10,nowresizey+5);
ui->verticalLayout_12->setAlignment(Qt::AlignLeft);
ui->verticalLayout_13->setAlignment(Qt::AlignLeft);
ui->verticalLayoutbutton->setAlignment(Qt::AlignLeft);
    this->setStyleSheet("background-color:rgb(100, 100, 0);color:rgb(255, 255, 255);");
}


MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress){
        mousepositionx=static_cast<QMouseEvent*>(event)->globalX();
        mousepositiony=static_cast<QMouseEvent*>(event)->globalY();
    } else if (event->type() == QEvent::MouseButtonRelease) {

        if(abs(mousepositionx-static_cast<QMouseEvent*>(event)->globalX())>5|| abs(mousepositiony-static_cast<QMouseEvent*>(event)->globalY())>5){
            if(abs(mousepositionx-static_cast<QMouseEvent*>(event)->globalX())>abs(mousepositiony-static_cast<QMouseEvent*>(event)->globalY())){//горизонталь
                if((mousepositionx-static_cast<QMouseEvent*>(event)->globalX())>0) {
                    figure->left();
                } else{
                    figure->right();
                }
            }
            else if((mousepositiony-static_cast<QMouseEvent*>(event)->globalY())<0){
                figure->down();
            } else figure->povorot();
        }


    } else if (event->type() == QEvent::KeyPress) {
        int keypressed=static_cast<QKeyEvent*>(event)->key();
        if(keypressed==Qt::Key_Up){//вверх
            figure->povorot();
        } else if(keypressed==Qt::Key_Down){
            figure->down();
        } else if(keypressed==Qt::Key_Right){
            figure->right();
        } else if(keypressed==Qt::Key_Left){
            figure->left();
        }
    }
    return QMainWindow::eventFilter(obj, event);



}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Back || event->key() == Qt::Key_Escape) {
        ui->start->setEnabled(true);
        if(menu){//если меню открыто,то закрыть
            disconnect(exit,SIGNAL(clicked(bool)),this,SLOT(close()));
            delete menulayout;
            delete menuwindow;
            menu=false;
            ui->start->click();
            ui->start->setFocus();
        } else {//если меню закрыто, то открыть
            if(figure->ignore==false) ui->start->click();//остановить игру
            ui->start->setEnabled(false);
            menuwindow=new QWidget();
            if((this->width()*900/480-this->height())<0){
                menuwindow->setFixedWidth(ui->centralWidget->width()*0.74);
                menuwindow->setFixedHeight(ui->centralWidget->width()*0.74);
            } else {
                menuwindow->setFixedWidth(ui->centralWidget->height()*480/900*0.74);
                menuwindow->setFixedHeight(ui->centralWidget->height()*480/900*0.74);
            }
            menuwindow->setAutoFillBackground(true);
            menuwindow->setStyleSheet("color: rgb(255, 255, 255);background-color: rgba(100, 100, 0, 90); ");
            newgame=new QPushButton("New Game",menuwindow);
            connect(newgame,SIGNAL(clicked(bool)),this,SLOT(on_newgame_clicked()));
            newgame->setGeometry(menuwindow->width()*0.1,menuwindow->height()*0.0769*1,menuwindow->width()*0.8,menuwindow->height()*0.0769*2);
            newgame->show();
            volume=new QSlider(Qt::Horizontal,menuwindow);
            volume->setSliderPosition(mediaplayer.volume());
            connect(volume,SIGNAL(valueChanged(int)),&mediaplayer,SLOT(setVolume(int)));
            volume->setGeometry(menuwindow->width()*0.25,menuwindow->height()*0.0769*4,menuwindow->width()*0.65,menuwindow->height()*0.0769*2);
            volume->show();
            QPixmap myPixmap( ":/volume_icon.png" );
            QLabel *scrin_volume=new QLabel(menuwindow);
            myPixmap=myPixmap.scaled(menuwindow->height()*0.15,menuwindow->height()*0.15,Qt::KeepAspectRatio,Qt::SmoothTransformation);
            scrin_volume->setPixmap(myPixmap);
            scrin_volume->setGeometry(menuwindow->width()*0.10,menuwindow->height()*0.0769*4,menuwindow->width()*0.15,menuwindow->height()*0.0769*2);
            autoincreaselevel=new QPushButton(menuwindow);
            if(increaselevel) autoincreaselevel->setText("Auto-level on"); else autoincreaselevel->setText("Auto-level off");
            connect(autoincreaselevel,SIGNAL(clicked(bool)),this,SLOT(slotincreaselevel()));
            autoincreaselevel->setGeometry(menuwindow->width()*0.1,menuwindow->height()*0.0769*7,menuwindow->width()*0.8,menuwindow->height()*0.0769*2);
            autoincreaselevel->show();
            exit=new QPushButton("Exit",menuwindow);
            connect(exit,SIGNAL(clicked(bool)),this,SLOT(close()));
            exit->setGeometry(menuwindow->width()*0.1,menuwindow->height()*0.0769*10,menuwindow->width()*0.8,menuwindow->height()*0.0769*2);
            exit->show();

            menulayout=new QGridLayout();
            QLabel pause("pause");
            pause.setGeometry(100,100,0,0);
            menulayout->addWidget(menuwindow);
            menulayout->addWidget(&pause);
            menuwindow->show();
            ui->centralWidget->setLayout(menulayout);
            menu=true;
        }
    }
    QPaintEvent* menuevent;
    paintEvent(menuevent);
}

void MainWindow::Esc_clicked()
{
    QKeyEvent *event=new QKeyEvent(QEvent::KeyPress,Qt::Key_Escape, Qt::NoModifier);
    this->keyPressEvent(event);
}

void MainWindow::slotincreaselevel()
{
    if(increaselevel){
        increaselevel=false;
        autoincreaselevel->setText("Auto-level off");
        ui->slow->setEnabled(true);
        ui->fast->setEnabled(true);
        disconnect(timer,SIGNAL(timeout()),this,SLOT(on_fast_clicked()));
    } else {
        increaselevel=true;
        disconnect(timer,SIGNAL(timeout()),this,SLOT(on_fast_clicked()));
        autoincreaselevel->setText("Auto-level on");
        ui->slow->setEnabled(false);
        ui->fast->setEnabled(false);
        qsrand(time(NULL));
        skorost=1;
        figure->timer->setInterval(950);
        ui->label->setText(QString::number(skorost));
        connect(timer,SIGNAL(timeout()),this,SLOT(on_fast_clicked()));
    }
}



void MainWindow::on_start_clicked(bool checked)
{
    if(checked==true) {figure->ignore=false;if(!figure->getGameover())figure->autodraw();if(increaselevel)timer->start(15000);}
    else {figure->ignore=true;if(increaselevel)timer->stop();}
}

void MainWindow::on_down_clicked()
{
        figure->functionfastdown();
}

void MainWindow::slotnewfigure()
{
    ui->score->setText("Score: "+QString::number(figure->score));
    if(figure->score>bestscore.toInt()){
        QFile file("score.txt");
        QByteArray writee;
        writee.append(QString::number(figure->score));
        if(file.open(QIODevice::WriteOnly)){
        file.write(writee);
        file.close();
        }
        ui->best_score->setText("Best score: "+QString::number(figure->score));
    }
}

void MainWindow::nextsong(QMediaPlayer::State state)
{
    mediaplayer.play();
}

void MainWindow::on_fast_clicked()
{
    if(skorost<=8){
        skorost++;
        switch(skorost){
        case 1: figure->timer->setInterval(950);break;
        case 2: figure->timer->setInterval(850);break;
        case 3: figure->timer->setInterval(750);break;
        case 4: figure->timer->setInterval(650);break;
        case 5: figure->timer->setInterval(550);break;
        case 6: figure->timer->setInterval(450);break;
        case 7: figure->timer->setInterval(350);break;
        case 8: figure->timer->setInterval(250);break;
        case 9: figure->timer->setInterval(150);break;
        }
        ui->label->setText(QString::number(skorost));
    }
}

void MainWindow::on_slow_clicked()
{
    if(skorost>=2){
        skorost--;
        switch(skorost){
        case 1: figure->timer->setInterval(950);break;
        case 2: figure->timer->setInterval(850);break;
        case 3: figure->timer->setInterval(750);break;
        case 4: figure->timer->setInterval(650);break;
        case 5: figure->timer->setInterval(550);break;
        case 6: figure->timer->setInterval(450);break;
        case 7: figure->timer->setInterval(350);break;
        case 8: figure->timer->setInterval(250);break;
        case 9: figure->timer->setInterval(150);break;
        }
        ui->label->setText(QString::number(skorost));
    }
}

void MainWindow::on_newgame_clicked()
{
    figure->setGameover(false);
    if(increaselevel){
        skorost=1;
        figure->timer->setInterval(950);
        ui->label->setText(QString::number(skorost));
    }
    QKeyEvent *event=new QKeyEvent(QEvent::KeyPress,Qt::Key_Escape, Qt::NoModifier);
    this->keyPressEvent(event);
    if(figure->ignore==false) {
        ui->start->click();
    }
    for(int i=0;i<20;i++){
        for(int j=-4;j<=5;j++){
            rect[i][j+4]->setBrush(QBrush(QColor(230,255,220)));
        }
    }
    figure->startgame();
    figure->score=0;
    ui->score->setText("Score: 0");
    ui->start->click();
}
