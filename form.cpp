#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(myfunction()));
    timer->start(600000);
}


Form::~Form()
{
    delete ui;
}

double Form::Readnum()
        {
            QString bruh = (ui->lineEdit->text());
            return bruh.toDouble(nullptr);
        }
double Form::Readdist()
{
    QString bruh = (ui->lineEdit_3->text());
    return bruh.toDouble(nullptr);
}
double Form::Readsize()
{
    QString bruh = (ui->lineEdit_2->text());
    return bruh.toDouble(nullptr);
}

void Form::myfunction()
{
   int i=0;
}
