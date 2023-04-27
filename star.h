#pragma once
#include "cmath"
#include <QPainter>
#include <fstream>
#include <ostream>
#include <QColor>
#include <stdlib.h>

#ifndef STAR_H
#define STAR_H
int numStars = 1200;
double systemRadius = 1e12, distConnect = 1e10;
const int dim = 2;
const int borderMassC = 3;
const double G = 6.67408e-11, dt = 10000;
const double massSun   = 1.98892e30,
             massJup   = 1898.6e24,
             massUran  = 86.832e24,
             massEarth = 5.9742e24,
             massVenus = 4.867e24,
             massMerc = 3.285e23;
const double borderMass[] = {massMerc,1.5*massMerc,2*massMerc, borderMassC*massMerc, borderMassC*massVenus, borderMassC*massEarth, borderMassC*massUran, borderMassC*massJup, borderMassC*massSun};
const QColor colStar[] = {Qt::darkRed, Qt::darkMagenta, Qt::green,Qt::darkCyan, Qt::red, Qt::cyan, Qt::darkGreen, Qt::magenta, Qt::yellow, Qt::white};
const int nColor = sizeof(colStar) / sizeof(colStar[0]);

/*void valueSetter(double* tempbruh)
{
    numStars = tempbruh[0];
    systemRadius = tempbruh[1];
    distConnect=tempbruh[2];
}*/

int colorSwap(double mass)
{
    for(int i = 0; i <= nColor; ++i)
    {
        if(mass <= borderMass[i])
        {
            return(i);
        }
    }
}

class star
{
public:
    static int starCounter;
    double x[dim];
    double v[dim];
    double m;
    double f[dim];
    QColor col;
    star(double *coord, double *speed, double mass);
    ~star(){starCounter--;}
};
int star::starCounter = 0;

star::star(double *coord, double *speed, double mass)
{
    for(int k = 0; k < dim; ++k)
    {
        x[k] = coord[k];
        v[k] = speed[k];
    }
    m = mass;
    col=colStar[colorSwap(m)]; // не годится, если будут объединения объектов, функция намного лучше
    starCounter++;
}

star operator + (star star1,star star2)
{
    double tmpM = star1.m + star2.m, tmpX[dim], tmpV[dim];
    for(int k = 0; k < dim; ++k)
    {
    tmpX[k] = (star1.x[k] * star1.m + star2.x[k] * star2.m)/tmpM;
    tmpV[k] = (star1.v[k] * star1.m + star2.v[k] * star2.m)/tmpM;
    }
    star1.m = tmpM;
    star1.col = colStar[colorSwap(star1.m)];
    for(int k = 0; k < dim; ++k)
    {
    star1.x[k] = tmpX[k];
    star1.v[k] = tmpV[k];
    }
    return(star1);
}

int comparator (const void* a, const void* b)
{
    if ( (*(star*)a).m <  (*(star*)b).m ) return -1;
    if ( (*(star*)a).m == (*(star*)b).m ) return 0;
    if ( (*(star*)a).m >  (*(star*)b).m ) return 1;
}

class galaxy
{
public:
    //int mass3[3]={1,1,1};
    int stepcounter=0;
    int num;
    //int numFilled=0;
    star **stars;
    star **sunless;
    galaxy(int n = numStars):num(n)
    {
        stars = new star*[num];
        sunless = new star*[num-1];
        double x1[dim] = {0}, v1[dim] = {0};
        stars[0] = new star(x1, v1, massSun); // самый массивный объект в начале координат
        double rad;
        double massDiff=massJup-massMerc;
        double randomMass=massMerc;
        for(int i = 1; i < num; ++i)
        {
            rad = 0;
            double R = rand() * systemRadius / RAND_MAX,
            fi  = (2 * M_PI * rand()) / RAND_MAX,
            theta = (M_PI * rand()) / RAND_MAX;
            x1[0] = R  * cos(fi);
            x1[1] = R  * sin(fi);
            if(dim == 3)
            {
                x1[0] *= sin(theta);
                x1[1] *= sin(theta);
                x1[3] = R * cos(theta);
            }
            for(int k = 0; k < dim; ++k)
            {
                rad += x1[k] * x1[k];
            }
// вторая космическая скорость
            double absV = sqrt(G * stars[0]->m / sqrt(rad)), alpha = (2 * M_PI * rand()) / RAND_MAX;
//если размерность 3, нужен еще один угол как для координат(два угла годятся и для плоскости, желающие могут сделать)
//            v1[0] = absV * cos(alpha);
//            v1[1] = absV * sin(alpha);
            v1[0] =  absV * sin(fi);
            v1[1] = -absV * cos(fi); // скорость направлена вдоль окружности с центром в начале координат
            randomMass+=rand()/massDiff + 1;
            stars[i] = new star(x1, v1, randomMass);
            /*switch (numFilled)
            {
            case 0:
                mass3[0]=i;
                numFilled++;
                break;
            case 1:
                mass3[1]=i;
                numFilled++;
                break;
            case 2:
                mass3[2]=i;
                numFilled++;
                break;
            default:
                if (stars[i]->m >= stars[mass3[0]]->m) {if (stars[i]->m >= stars[mass3[1]]->m) {if (stars[i]->m >= stars[mass3[2]]->m) {mass3[1]=mass3[2];mass3[2]=i;} else {mass3[0]=mass3[1]; mass3[1]=i;}} else{mass3[0]=i;}}
            }*/
        }
        for (int i=1;i<num;i++)
        {
           sunless[i-1]=stars[i];
        }
        qsort(sunless,sizeof(sunless),sizeof(star),comparator);
        for (int i=1; i<num;i++)
        {
           stars[i]=sunless[i-1];
        }
    };
    ~galaxy(){delete[] stars;};
    double border[dim];

    void move()
    {
        double dist;
        double distSun;
        double dCoord[dim];
        for(int i = 1; i < num; ++i)
        { // force nullification
            for(int k = 0; k < dim; ++k)
            {
                if(stars[i])
                {
                    stars[i]->f[k] = 0;
                }
            }
        }
        for(int i = 1; i < num; i++)          //merger check
        {
            if(stars[i])
            {
                for(int j = i + 1; j < num; j++)
                {
                    if(i != j && stars[j])
                    {
                        dist = 0;
                        for(int k = 0; k < dim; ++k)
                        {
                            dCoord[k] = stars[i]->x[k] - stars[j]->x[k];
                            dist += dCoord[k] * dCoord[k];
                        }
                        if(sqrt(dist) < distConnect)
                        {
                            /*double tmpM = stars[i]->m + stars[j]->m, tmpX[dim], tmpV[dim];
                            for(int k = 0; k < dim; ++k)
                            {
                                tmpX[k] = (stars[i]->x[k] * stars[i]->m + stars[j]->x[k] * stars[j]->m)/tmpM;
                                tmpV[k] = (stars[i]->v[k] * stars[i]->m + stars[j]->v[k] * stars[j]->m)/tmpM;
                            }
                            delete stars[j];
                            stars[j] = nullptr;
                            stars[i]->m = tmpM;
                            stars[i]->col = colStar[colorSwap(stars[i]->m)];
                            for(int k = 0; k < dim; ++k)
                            {
                                stars[i]->x[k] = tmpX[k];
                                stars[i]->v[k] = tmpV[k];
                            }*/
                            *stars[i]=*stars[i]+*stars[j];
                            delete stars[j];
                            stars[j] = nullptr;
                            star::starCounter+=3;
                            /*for (int k=0; k<3;k++)
                            {
                                if (mass3[k]==j) {for (int i =1; i<num; i++) {if (stars[i]){mass3[k]=i; break;}}; break;}
                            }*/
                        }

                        //if (sqrt(stars[i]->x[0]*stars[i]->x[0] + stars[i]->x[1]*stars[i]->x[1]) < distConnect) {delete stars[i];}
                    }
                }
                distSun=0;
                for(int k = 0; k < dim; ++k)
                {
                    dCoord[k] = stars[i]->x[k];
                    distSun += dCoord[k] * dCoord[k];
                }
                if(sqrt(distSun) < distConnect)
                {
                    delete stars[i];
                    stars[i]=nullptr;
                    star::starCounter+=3;
                    /*for (int k=0; k<3;k++)
                    {
                        if (mass3[k]==i) {for (int j =1; j<num; j++) {if (stars[j]){mass3[k]=j; break;}}; break;}
                    }*/
                }
            }
        }
        for(int i = 0; i < num; i++)      //force recalc
        {
            if(stars[i])
            {
                for(int j = i + 1; j < num; j++)
                {
                    if(i != j && stars[j])
                    {
                        dist = 0;
                        for(int k = 0; k < dim; ++k)
                        {
                            dCoord[k] = stars[i]->x[k] - stars[j]->x[k];
                            dist += dCoord[k] * dCoord[k];
                        }
                        // dist = sqrt(dist); // для знаменателя пока квадрат, потом возьмем корень
                        for(int k = 0; k < dim; ++k)
                        {
                            double tmp = G * stars[i]->m * stars[j]->m / dist;
                            stars[i]->f[k] -= tmp * dCoord[k] / sqrt(dist);
                            stars[j]->f[k] += tmp * dCoord[k] / sqrt(dist);
                        }
                    }
                }
            }
        }
        /*for (int i = 0; i < num; ++i)
        {
            if(stars[i])
            {
             if (sqrt(stars[i]->x[0]*stars[i]->x[0] + stars[i]->x[1]*stars[i]->x[1]) < distConnect) {stars[i]->~star();stars[i]=0;}
            }
        }*/
        for(int i = 1; i < num; ++i)
        {
            if(stars[i])
            {
                for(int k = 0; k < dim; ++k)
                {
                    stars[i]->v[k] += dt * stars[i]->f[k] / stars[i]->m; //можно не делить на массу, а выше суммировать ускорение
                }
                for(int k = 0; k < dim; ++k)
                {
                    stars[i]->x[k] += dt * stars[i]->v[k];
                }
                //if (stars[i]->m >= stars[mass3[0]]->m) {if (stars[i]->m >= stars[mass3[1]]->m) {if (stars[i]->m >= stars[mass3[2]]->m) {mass3[1]=mass3[2];mass3[2]=i;} else {mass3[0]=mass3[1]; mass3[1]=i;}} else{mass3[0]=i;}}
               //if (stars[i]->m >= mass3[0]) {if (stars[i]->m >= mass3[1]) {if (stars[i]->m >= mass3[2]) {mass3[1]=mass3[2];mass3[2]=i;} else {mass3[0]=mass3[1]; mass3[1]=i;}} else{mass3[0]=i;}}
            }
           //if (stars[i]->m >= mass3[0]) {if (stars[i]->m >= mass3[1]) {if (stars[i]->m >= mass3[2]) {mass3[1]=mass3[2];mass3[2]=i;} else {mass3[0]=mass3[1]; mass3[1]=i;}} else{mass3[0]=i;}}

        }
        /*for (int i = 1; i<num; ++i)
        {
            if (stars[i])
            {
               if (stars[i]->m >= mass3[0]) {if (stars[i]->m >= mass3[1]) {if (stars[i]->m >= mass3[2]) {mass3[1]=mass3[2];mass3[2]=i;} else {mass3[0]=mass3[1]; mass3[1]=i;}} else{mass3[0]=i;}}
            }
        }*/
        for (int i=1;i<num;i++)
        {
           sunless[i-1]=stars[i];
        }
        qsort(sunless,sizeof(sunless),sizeof(star),comparator);
        for (int i=1; i<num;i++)
        {
           stars[i]=sunless[i-1];
        }
      stepcounter++;
    }
};

/*std::ofstream & operator<<(std::ofstream & ofs, const star& star)
{
    ofs << "x[0]= " << star.x[0] << "; x[1]= " << star.x[1] << "; v[0]= " << star.v[0] << "; v[1]= " << star.v[1]
         << "\nm= " << star.m << "; f[0]= " << star.f[0] << "; f[1]= " << star.f[1] << "; col= " << star.col.name().toStdString() << std::endl;
    return ofs;
}

 std::ofstream & operator<<(std::ofstream & ofs,const galaxy& galactika)
 {
    ofs << "Число звёзд = " << galactika.num <<"; Размер области = " << systemRadius << "; Радиус соединения = " << distConnect
        << "\nТекущий шаг = " << galactika.stepcounter << "; Текущее время = " <<galactika.stepcounter*dt <<" секунд." << std::endl;
    for (int i=0;i<galactika.num;i++)
    {
        if (galactika.stars[i])
        {
         ofs<<galactika.stars[i];
        }
    }
    return ofs;
 }

 void operator>>(std::ifstream& ifs, galaxy& galactika)
 {
    std::string tmp;
    std::getline(ifs,tmp);
    double tmpNum = std::stold(tmp);
    galactika.num = tmpNum;
    tmpNum = std::stold(tmp);
    std::getline(ifs,tmp);
    systemRadius = tmpNum;
    std::getline(ifs,tmp);
    tmpNum = std::stold(tmp);
    distConnect = tmpNum;
 }*/
#endif // STAR_H
