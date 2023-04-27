/*#pragma once
#include "mainwindow.h"
#include "math.h"
#include <star.h>
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTime>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    connect(ui->pushButtonStart,  SIGNAL(clicked()), this, SLOT(buttonText()));
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
  // connect(timer, &QTimer::timeout, this, SLOT(myTimer()));
    timer->start(1);
}
MainWindow::~MainWindow(){
    delete ui;
}
void MainWindow::buttonText()
{
    if(ui->pushButtonStart->text()==textB[0]){
        ui->pushButtonStart->setText(textB[1]);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    }else
    {
        ui->pushButtonStart->setText(textB[0]);
        disconnect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    }
    return;
}

const int topX0 = 100, topY0 = 100, h = 800, length = 800;
galaxy *galactika = new galaxy;
std::ofstream fileout("C:\\MINE\\starsOutput.txt",std::ofstream::app | std::ofstream::trunc);
std::ifstream filein("C:\\MINE\\starsInput.txt");
void MainWindow::paintEvent(QPaintEvent *e)
{
   // filein >> *galactika;
  Q_UNUSED(e);
  QPainter painter(this);
  QPen pen(Qt::black, 1, Qt::SolidLine);
  painter.setPen(pen);
//  QTime time = QTime::currentTime();
//  int mSec = time.msec();
//  int Sec = time.second();

  QBrush brush;//(Qt::yellow);
  brush.setStyle(Qt::SolidPattern);

  double coefX = length / 2 / 1e12; // system radius
  int centerX = length / 2;
  for(int i = 0; i < galactika->num; ++i)
  {
      if(galactika->stars[i])
      {
          brush.setColor(galactika->stars[i]->col);
          if(!i) brush.setColor(Qt::yellow);
          painter.setBrush(brush);
          for(int k = 0; k < 2; ++k)
          {
             // условие не рисовать вне квадрата
             /* if(galactika->stars[i]->x[0] * coefX + centerX + topX0 > 0 &&
                 galactika->stars[i]->x[0] * coefX + centerX  < length &&
                 galactika->stars[i]->x[1] * coefX + centerX + topY0 > 0 &&
                 galactika->stars[i]->x[1] * coefX + centerX  < h)
                    painter.drawEllipse(galactika->stars[i]->x[0] * coefX + centerX + topX0,
                                        galactika->stars[i]->x[1] * coefX + centerX + topY0,
                                        6 + 4 * !i, 6 + 4 * !i);
          }
      }
  }
  if (galactika->stepcounter / 10 == 0) {fileout << galactika;}
  galactika->move();

  ui->lineEdit->setText(QString::number(star::starCounter));
  ui->lineEdit_2->setText(QString::number(galactika->stars[0]->m));
  ui->lineEdit_3->setText(QString::number(galactika->stars[0]->x[0]));
  ui->lineEdit_4->setText(QString::number(galactika->stepcounter));
  ui->lineEdit_5->setText(QString::number(galactika->stepcounter*dt));
 /* ui->lineEdit_7->setText(QString::number(galactika->stars[galactika->mass3[2]]->m));
  ui->lineEdit_6->setText(QString::number(galactika->stars[galactika->mass3[2]]->x[0]));
  ui->lineEdit_8->setText(QString::number(galactika->stars[galactika->mass3[2]]->x[1]));
  ui->lineEdit_9->setText(QString::number(galactika->stars[galactika->mass3[2]]->v[0]));
  ui->lineEdit_10->setText(QString::number(galactika->stars[galactika->mass3[2]]->v[1]));
  ui->lineEdit_16->setText(QString::number(galactika->stars[galactika->mass3[1]]->m));
  ui->lineEdit_19->setText(QString::number(galactika->stars[galactika->mass3[1]]->x[0]));
  ui->lineEdit_17->setText(QString::number(galactika->stars[galactika->mass3[1]]->x[1]));
  ui->lineEdit_18->setText(QString::number(galactika->stars[galactika->mass3[1]]->v[0]));
  ui->lineEdit_20->setText(QString::number(galactika->stars[galactika->mass3[1]]->v[1]));
  ui->lineEdit_11->setText(QString::number(galactika->stars[galactika->mass3[0]]->m));
  ui->lineEdit_12->setText(QString::number(galactika->stars[galactika->mass3[0]]->x[0]));
  ui->lineEdit_13->setText(QString::number(galactika->stars[galactika->mass3[0]]->x[1]));
  ui->lineEdit_14->setText(QString::number(galactika->stars[galactika->mass3[0]]->v[0]));
  ui->lineEdit_15->setText(QString::number(galactika->stars[galactika->mass3[0]]->v[1]));
  ui->lineEdit_21->setText(QString::number(galactika->mass3[2]));
  ui->lineEdit_22->setText(QString::number(galactika->mass3[1]));
  ui->lineEdit_23->setText(QString::number(galactika->mass3[0]));
}*/
