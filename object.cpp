#include "object.h"
int objectt::down()
{
    if(ignore==false){//если игра не присотановлена
        if(gameover){
            emit signalgameover();
            return 0;
        }
        draw(QColor(230,255,220),"mas");//залить место текущей фигуры цветом поля игры
        positionyfromup++;//переместить счетчик на позицию ниже
        for(int i=3;i>=0;i--){//обходим все элементы массива фигуры
            for(int j=0;j<4;j++){
                if(mas[i][j]==1){
                    if((i+positionyfromup)>=20 || rectt[i+positionyfromup][j+positionxfromcenter]->brush()!=QBrush(QColor(230,255,220)))//проверка, не дошли ли до границы снизу
                    {
                        positionyfromup--;autodraw();deleting();
                        positionxfromcenter=3; positionyfromup=0;
                        newfigure();
                        for(int i=3;i>=0;i--){//обходим все элементы массива фигуры для проверки на окончание игры
                            for(int j=0;j<4;j++){
                                if(mas[i][j]==1){
                                    if(rectt[i+positionyfromup][j+positionxfromcenter]->brush()!=QBrush(QColor(230,255,220))){
                                        emit signalgameover();
                                        gameover=true;
                                        fastdown=false;
                                        return 0;
                                    }
                                }
                            }
                        }
                        autodraw();fastdown=false;return 0;}
                }
            }
        }
        autodraw();
    }
    return 0;
}


objectt::objectt()
{
    timer->setInterval(interval);
    connect(timer,SIGNAL(timeout()),this,SLOT(down()));
    timer->start();
}

void objectt::draw(QColor color, QString massiv){
    if(massiv=="mas"){
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if(mas[i][j]==1) rectt[i+positionyfromup][j+positionxfromcenter]->setBrush(QBrush(color));
            }
        }
    } else if(massiv=="masnextfigure"){
        for(int i=0;i<4;i++){
            for(int j=0;j<3;j++){
                nextfigurerectt[i][j]->setBrush(QBrush(color));
                nextfigurerectt[i][j]->setPen(Qt::NoPen);
            }
        }
    }
}

void objectt::autodraw(){
    QColor color;
    switch(numberfigure){
    case 0: color=QColor(255,0,0);//red
        break;
    case 1: color=QColor(255,153,0);//orange
        break;
    case 2: color=QColor(150,120,140);//grey
        break;
    case 3: color=QColor(219,60,147);//lilovyi
        break;
    case 4: color=QColor(255,230,60);//yellow
        break;
    case 5: color=QColor(50,30,200);//blue
        break;
    case 6: color=QColor(0,255,0);//green
        break;
    }

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(mas[i][j]==1) rectt[i+positionyfromup][j+positionxfromcenter]->setBrush(QBrush(color));
        }
    }
}

void objectt::drawnextfigure(){
    QColor color;

    switch(nextnumberfigure){
    case 0: color=QColor(255,0,0);//red
        break;
    case 1: color=QColor(255,153,0);//orange
        break;
    case 2: color=QColor(150,120,140);//grey
        break;
    case 3: color=QColor(219,60,147);//lilovyi
        break;
    case 4: color=QColor(255,230,60);//yellow
        break;
    case 5: color=QColor(50,30,200);//blue
        break;
    case 6: color=QColor(0,255,0);//green
        break;
    }

    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            if(masnextfigure[i][j]==1) {
                nextfigurerectt[i][j]->setBrush(QBrush(color));
                nextfigurerectt[i][j]->setPen(Qt::SolidLine);
            }
        }
    }
}

int objectt::left(){
    if(ignore==false){
        positionxfromcenter++;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if(mas[i][j]==1){
                    if((j+positionxfromcenter)<=1 || rectt[i+positionyfromup][j+positionxfromcenter-2]->brush()!=QBrush(QColor(230,255,220))){positionxfromcenter--; return 0;}//проверка, не дошли ли до границы слева
                    j=4;//проверяем только 1 самый левый элемент, т.к. следующий элемент уже будет сравнивать с тем, что левее
                }
            }
        }
        positionxfromcenter--;
        //
        draw(QColor(230,255,220),"mas");
        positionxfromcenter--;
        autodraw();
    }
    return 0;
}

int objectt::right(){
    if(ignore==false){
        positionxfromcenter++;
        for(int i=0;i<4;i++){
            for(int j=3;j>=0;j--){
                if(mas[i][j]==1){
                    if((j+positionxfromcenter)>=10 || rectt[i+positionyfromup][j+positionxfromcenter]->brush()!=QBrush(QColor(230,255,220))){positionxfromcenter--;return 0;}//проверка, не дошли ли до границы справа
                    j=0;
                }
            }
        }
        positionxfromcenter--;
        //
        draw(QColor(230,255,220),"mas");
        positionxfromcenter++;
        autodraw();
    }
    return 0;
}

void objectt::setrect(QGraphicsRectItem *rect[20][10]){
    for(int i=0;i<20;i++){
        for(int j=0;j<=9;j++){
            rectt[i][j]=rect[i][j];
        }
    }
}

void objectt::setnextfigurerect(QGraphicsRectItem *nextfigurerect[4][3]){
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            nextfigurerectt[i][j]=nextfigurerect[i][j];
        }
    }
}

void objectt::newfigurepovorot(){
    for(int i=numberfigure*4;i<numberfigure*4+4;i++){
        for(int j=numberpovorot*4;j<numberpovorot*4+4;j++){
            mas[i-numberfigure*4][j-numberpovorot*4]=mapfigure[i][j];
        }
    }
}

void objectt::newfigure(){
    draw(QColor(100, 100, 0),"masnextfigure");
    numberfigure=nextnumberfigure;
    numberpovorot=0;
    for(int i=numberfigure*4;i<numberfigure*4+4;i++){
        for(int j=0;j<4;j++){
            mas[i-numberfigure*4][j]=mapfigure[i][j];
        }
    }
    qsrand(time(NULL)+qrand() % (999-0));
    nextnumberfigure=qrand() % (7-0);
    for(int i=nextnumberfigure*4;i<nextnumberfigure*4+4;i++){
        for(int j=0;j<4;j++){
            masnextfigure[i-nextnumberfigure*4][j]=mapfigure[i][j];
        }
    }
    drawnextfigure();

}

void objectt::startgame(){
    positionxfromcenter=3;
    positionyfromup=0;
    qsrand(time(NULL));
    nextnumberfigure=qrand() % (7-0);
    for(int i=nextnumberfigure*4;i<nextnumberfigure*4+4;i++){
        for(int j=0;j<4;j++){
            masnextfigure[i-nextnumberfigure*4][j]=mapfigure[i][j];
        }
    }
    draw(QColor(100, 100, 0),"masnextfigure");
    drawnextfigure();
    numberfigure=qrand() % (7-0);
    for(int i=numberfigure*4;i<numberfigure*4+4;i++){
        for(int j=0;j<4;j++){
            mas[i-numberfigure*4][j]=mapfigure[i][j];
        }
    }
}

int objectt::povorot(){
    if(ignore==false){
        if(numberpovorot==3) {numberpovorot=0;} else {numberpovorot++;}
        draw(QColor(230,255,220),"mas");
        newfigurepovorot();
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if(mas[i][j]==1){
                    if((j+positionxfromcenter)>=10 || (j+positionxfromcenter)<0 || rectt[i+positionyfromup][j+positionxfromcenter]->brush()!=QBrush(QColor(230,255,220))){if(numberpovorot==0) {numberpovorot=3;} else {numberpovorot--;}newfigurepovorot();autodraw();return 0;}//проверка, не дошли ли до границы справа
                }
            }
        }
        autodraw();
        newfigurepovorot();
    }
    return 0;
}

void objectt::deleting(){
metka:
    for(int i=0;i<20;i++){
        int shet=0;
        for(int j=0;j<10;j++){
            if(rectt[i][j]->brush()!=QBrush(QColor(230,255,220))) shet++;
        }
        if(shet==10){
            for(int zz=i;zz>0;zz--){
                for(int yy=0;yy<10;yy++){
                    rectt[zz][yy]->setBrush(rectt[zz-1][yy]->brush());
                }
            }
            for(int yy=0;yy<10;yy++){
                rectt[0][yy]->setBrush(QColor(230,255,220));
            }
            score=score+50;emit signalnewfigure();
            goto metka;
        }
    }
}

void objectt::functionfastdown(){
    if(ignore==false && gameover==false){
        fastdown=true;
        while(fastdown) down();
    }
}

bool objectt::getGameover() const
{
    return gameover;
}

void objectt::setGameover(bool value)
{
    gameover = value;
}

void objectt::setPositionyfromup(int value)
{
    positionyfromup = value;
}

void objectt::setPositionxfromcenter(int value)
{
    positionxfromcenter = value;
}



