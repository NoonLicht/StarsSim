#ifndef FORM_H
#define FORM_H

#include "qtimer.h"
#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
    QTimer* timer;
    double Readnum();
    double Readdist();
    double Readsize();
    double* on_pushButton_clicked()
    {
        static double bruh[3];
        bruh[0]=Readnum();
        bruh[1]=Readsize();
        bruh[2]=Readdist();
        return bruh;
    }
public slots:
    void myfunction();
private:
    Ui::Form *ui;
};

#endif // FORM_H
