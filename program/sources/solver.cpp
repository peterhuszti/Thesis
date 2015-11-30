#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include "solver.h"
#include "linalg.h"
#include "sparse_matrix.h"

solver::solver()
{
    ex=false;
    deg_rad=0;
    k=10;
    kmax=-1;
    epsilon=0.001;
    kmax=-1;
    exact.resize(0);
    
    xi_norm.resize(0);
    xi_vect.resize(0);
    conv1.resize(0);
    conv2.resize(0);
}

void solver::writeVecToFile(const std::vector<double> &v, std::ostream &o)
{
    o << "( ";
    for(int i=0;i<v.size();i++)
    {
        o << v[i] << "; ";
    }
    o << ")\n";   
}
//getter
int solver::getKmax()
{
    return kmax;
}

std::vector<double> solver::getXiNorm()
{
    return xi_norm;
}

std::vector<double> solver::getXiVect()
{
    return xi_vect;
}

std::vector<double> solver::getConv1()
{
    return conv1;
}

std::vector<double> solver::getConv2()
{
    return conv2;
}
//setter
void solver::addF(const std::vector<std::queue<std::string> > &f)
{
    F=f;
}

void solver::setExact(const std::vector<double> &xx)
{
    ex=true;
    exact=xx;
}

void solver::setKmax(int c)
{
    kmax=c;
}

void solver::setEpsilon(double c)
{
    epsilon=c;
}

void solver::setK(int c)
{
    k=c;
}

//
void solver::newclick()
{
    ex=false;
    k=10;
    kmax=-1;
    epsilon=0.001;
    deg_rad=0;
    backclick();
}

void solver::backclick()
{
    F.resize(0);
    xi_norm.resize(0);
    xi_vect.resize(0);
    conv1.resize(0);
    conv2.resize(0);
    exact.resize(0);
}

void solver::changeDegRad(int c)
{
    deg_rad=c;
}

std::vector<double> solver::eval(const std::vector<double> &x)
{     
    double deg=PI/180.;//trigonometrikus fv-khez
    double deg2=180./PI;//inverz trig fv-hez
    
    std::vector<double> res;
    int n=F.size();//egyenletek száma
    for(int i=0;i<n;i++)
    {
        std::stack<std::string> verem;//kiértékelõ verem
        std::queue<std::string> lengyel=F[i];//az aktuális egyenlet lengyelformája
        while(!lengyel.empty())//amíg fel nem dolgozta az egészet
        {
            td szamok;
            std::string nextLengyel=lengyel.front();//szimbólumonként kiveszi a sorból az elemeket
            lengyel.pop();
            switch(nextLengyel[0])
            {
                case 's'://sin, sqrt
                    szamok.elso=StringToNumber(verem,x);
                    if(nextLengyel[1]=='i')/sin
                    {                   
                        //deg_rad==0 -> radián
                        //deg_rad==1 -> fok
                        verem.push(NumberToString(sin( (1-deg_rad)*szamok.elso+deg_rad*deg*szamok.elso )));
                    }
                    else//sqrt
                        verem.push(NumberToString(sqrt(szamok.elso)));
                    break;
                case 'c'://cos, ctg, cbrt
                    szamok.elso=StringToNumber(verem,x);
                    if(nextLengyel[1]=='o')//cos
                    {   
                        verem.push(NumberToString(cos( (1-deg_rad)*szamok.elso+deg_rad*deg*szamok.elso )));
                    }
                    else if(nextLengyel[1]=='t')//ctg
                    {  
                        verem.push(NumberToString(1/(tan( (1-deg_rad)*szamok.elso+deg_rad*deg*szamok.elso ))));
                    }
                    else//cbrt
                        verem.push(NumberToString(cbrt(szamok.elso)));
                    break;
                case 't'://tan
                    szamok.elso=StringToNumber(verem,x);
                    verem.push(NumberToString(tan( (1-deg_rad)*szamok.elso+deg_rad*deg*szamok.elso )));
                    break;
                case 'a'://asin, acos, atan, actg
                    szamok.elso=StringToNumber(verem,x);
                    switch(nextLengyel[2])
                    {
                        double seged;
                        case 'i'://asin
                            seged=asin(szamok.elso);
                            verem.push(NumberToString( (1-deg_rad)*seged*deg2+deg_rad*seged ));
                            break;
                        case 'o'://acos
                            seged=acos(szamok.elso);
                            verem.push(NumberToString((1-deg_rad)*seged+deg_rad*deg2*seged));
                            break;
                        case 'a'://atan
                            seged=atan(szamok.elso);
                            verem.push(NumberToString((1-deg_rad)*seged+deg_rad*deg2*seged));
                            break;
                        case 't'://acth
                            seged=1/atan(szamok.elso);
                            verem.push(NumberToString((1-deg_rad)*seged+deg_rad*deg2*seged));
                            break;
                    }
                    break;
                case 'l'://ln
                    szamok.elso=StringToNumber(verem,x);
                    verem.push(NumberToString(log(szamok.elso)));
                    break;
                case '+':
				{
                    szamok=StringToNumbers(verem,x);
                    verem.push(NumberToString(szamok.elso+szamok.masodik));
                    break;
                }
                case '-':
                    if(nextLengyel.size()==2)//negálás
                    {
                        szamok.elso=StringToNumber(verem,x);
                        verem.push(NumberToString((-1)*szamok.elso));
                    }
                    else//kivonás
                    {
                        szamok=StringToNumbers(verem,x);
                        verem.push(NumberToString(szamok.masodik-szamok.elso));
                    }
                    break;
                case '*':
                    szamok=StringToNumbers(verem,x);
                    verem.push(NumberToString(szamok.elso*szamok.masodik));
                    break;
                case '/':
                    szamok=StringToNumbers(verem,x);
                    if(szamok.elso==0) szamok.elso=thres;
                    verem.push(NumberToString(szamok.masodik/szamok.elso));
                    break;
                case '^'://hatványozás
                    szamok=StringToNumbers(verem,x);
                    verem.push(NumberToString(pow(szamok.masodik,szamok.elso)));
                    break;
                case 'x':
                    verem.push(NumberToString(x[chooseX(nextLengyel)]));
                    break;
                default: //szám
                    verem.push(nextLengyel);
                    break;
            }
        }
        res.push_back(myAtof(verem.top().c_str()));//ha kiürült a verem, akkor a legfelsõ elem a megoldás
    }    
    return res;
}

std::string solver::NumberToString(double t)
{
     std::ostringstream oo;
     oo << t;
     return oo.str();
}

td solver::StringToNumbers(std::stack<std::string> &verem,const std::vector<double> &x)
{
    std::string temp1=verem.top();
    verem.pop();
    std::string temp2=verem.top();
    verem.pop();
    td res;
    
    for(int i=0;i<temp1.size();i++)
        if(temp1[i]==',') temp1[i]='.';
    for(int i=0;i<temp2.size();i++)
        if(temp2[i]==',') temp2[i]='.';
    
    if(temp1[0]=='x') res.elso=x[chooseX(temp1)-1];
    else if(temp1[0]=='e') res.elso=e;
    else if(temp1[0]=='P') res.elso=PI;
    else res.elso=myAtof(temp1.c_str());
    if(temp2[0]=='x') res.masodik=x[chooseX(temp2)-1];
    else if(temp2[0]=='e') res.masodik=e;
    else if(temp1[0]=='P') res.elso=PI;
    else res.masodik=myAtof(temp2.c_str());
    return res;
}

double solver::StringToNumber(std::stack<std::string> &verem,const std::vector<double> &x)
{
    std::string temp1=verem.top();
    verem.pop();
    
    for(int i=0;i<temp1.size();i++)
        if(temp1[i]==',') temp1[i]='.';
    
    double temp;
    if(temp1[0]=='x') temp=x[chooseX(temp1)-1];
    else if(temp1[0]=='e') temp=e;
    else if(temp1[0]=='P') temp=PI;
    else temp=myAtof(temp1.c_str());
    return temp;
}

int solver::chooseX(const std::string &str)
{
    std::string temp=str;
    temp.erase(temp.begin());
    return atoi(temp.c_str())-1;
}

std::vector<double> solver::broyden(const std::vector<double> &x0, const std::vector<double> &d)
{
    int n=x0.size();
    std::vector<double> xm=x0;
    std::vector<double> xm_1;

    std::vector<std::vector<double> > I;//egységmátrix inicializálása
    I.resize(n);
    for(int i=0;i<n;i++)
    {
        I[i].resize(n);
        for(int j=0;j<n;j++)
        {
            if(i==j) I[i][j]=1;
            else I[i][j]=0;
        }
    }

    std::vector<std::vector<double> > Am_1;
    std::vector<std::vector<double> > Qm_1;
    sparse_matrix Rm_1;
    std::vector<std::vector<double> > Am;
    std::vector<std::vector<double> > Qm;
    sparse_matrix Rm;

    std::vector<double> x0Eval=eval(x0)+d;//(t-1)f(x0)
    std::vector<double> xmEval(n,0);
    std::vector<double> xm_1Eval(n,0);

    Am_1=Qm_1=I;//kezdõ Jacobi közelítés az I
    Rm_1=toSpare(I);//tanácsosabb ritka mátrixként tárolni a felsõháromszög alakú R-t

    int k=0;//lépések száma
    do
    {
        if(k!=0)//elsõ lépésnél nem kell
        {
            Am_1=Am;
            Qm_1=Qm;
            Rm_1=Rm;
        }
        
        xm_1=xm;

        xm_1Eval=eval(xm_1)+d;

        xm=xm_1+toDense(inverseUpperTriangular(Rm_1)*T(Qm_1))*xm_1Eval*(-1);
		//következõ közelítõ vektor
        xmEval=eval(xm)+d;

        xi_norm.push_back(norm(eval(xm)));//ábrázoláshoz
        conv1.push_back(-1);//ha közbülsõ egyenlet vektora, akkor -1
        if(ex)
        { 
            xi_vect.push_back(norm(xm+exact*(-1)));
            conv2.push_back(-1);
        }
       
        file << "   ";//napló
            file << "( ";
            for(int i=0;i<xm.size();i++)
            {
                file << std::setprecision(10) << xm[i] << "; ";
            }
            file << ")"; 
            file << "        " << norm(xmEval) << "\n";

		//Jacobi-mtx következõ közelítése
        std::vector<double> v=xm+xm_1*(-1);
        Am=Am_1+((xmEval+xm_1Eval*(-1)+Am_1*v*(-1))*v)*(1/vectorMultScalar(v,v));
        std::vector<double> u=(Am+Am_1*(-1))*v*(1/pow(norm(v),2));

		//QR-felbontás
        std::vector<std::vector<double> > QR_temp=toDense(Rm_1)+T(Qm_1)*u*v;
        sparse_matrix QR=toSpare(QR_temp);
        qr QR_decomped=givens(QR);

        std::vector<std::vector<double> > Q=toDense(QR_decomped.Q);
        Qm=Qm_1*Q;
        if(QR.getNodeszam()<n)//ha QR ritka volt
        {
            Rm=T(Q)*QR;
        }
        else//ha sûrû
        {
            Rm=toSpare(T(Q)*QR_temp);
        }

        javit(Rm);//hibák kiküszöbölése
        javit(Qm);

        k++;
        
    }while(norm(xm+(xm_1*(-1)))>epsilon*(0.5+norm(xm)) && k<kmax);
    
    return xm;
}


std::vector<double> solver::cont(const std::vector<double> & x0)
{
    file.open("output/log.txt");//napló inicializálása
    file << "Solving the\n";
    file << "F(x_i, t_i) = f(x_i) + (t_i - 1) * f(x_0)\n";
    file << "function, where i=1..k, therefore t_i - 1 = 0..1.\n\n";
    file << "The starting vector (x_0) is: ";
    writeVecToFile(x0,file);
    
    if(ex)//ha van pontos megoldás
    {
        file << "The exact solution (x*) is: ";
        writeVecToFile(exact,file);
    }
    else//ha nincs
    {
        file << "The exact solution is unknown.\n\n";
    }

    xi_norm.push_back(norm(eval(x0)));
    conv1.push_back(xi_norm[xi_norm.size()-1]); 
    if(ex)
    {
        xi_vect.push_back(norm(x0+exact*(-1)));
        conv2.push_back(xi_vect[xi_vect.size()-1]); 
        file << "|| x* - x_0 || = " << xi_vect[0] << "\n\n";
    } 
    
    std::vector<double> x0Eval=eval(x0);
    
    if(norm(x0Eval)<=thres)//ha gyöktõl indulunk
    {
        file << "The starting vector is a root of the function.\n";
        file.close();
        return x0;
    }    
    
    if(kmax==-1) kmax=(-1)*log(epsilon/norm(x0Eval))+1;//ha felül van írva a max lépésszám

    double delta=1/(double)k;
    double t=0;
	double t_old=0;//visszalépésnél kell az eredeti ti
    std::vector<double> xi=x0;
    std::vector<double> d;//(t-1)f(x0)
    double oszto=2;
    
    file << "Parameters:\n";
    file << "   Epsilon = " << epsilon << "\n";
    file << "   Delta t = " << delta << "\n";
    file << "   Maximum steps = " << kmax << "\n";
    
    conv1.resize(1);
    if(ex) conv2.resize(1);
    
    while(t<=1)
    {
        file << "\nt = " << t << "\n";
        file << "   Starting Broyden-method from ";
        writeVecToFile(xi,file);

        oszto=2;

        std::vector<double> xi_1=xi;
        d=x0Eval*(t-1);
        int oldSize=xi_norm.size();//ha vissza kell lépni, akkor az ábrázoló vektorokból a rossz értékeke ki kell majd törölni
        
        xi=broyden(xi_1,d);//Broyden, x következõ közelítõ vektora
        
        conv1[conv1.size()-1]=xi_norm[xi_norm.size()-1];  
        if(ex) conv2[conv2.size()-1]=xi_vect[xi_vect.size()-1]; 
           
        file << "\n   || F(x_i+1) || = " << norm(eval(xi)+d);

        double xd=norm(eval(xi_1)+d);

        while(norm(eval(xi)+d)>xd)//addig, amíg nem konvergál a Broyden-módszer
        {
            file << " > || F(x_i) || = " << xd << "\n";
            file << "   The method is not convergent, stepping back.\n\n";
            
            xi_norm.resize(oldSize);
            conv1.resize(oldSize);
            if(ex)
            {
               xi_vect.resize(oldSize);
               conv2.resize(oldSize);
            }
            
            t=t_old;//visszalépés
            t+=delta/oszto;//lépésköz felezése

            file << "Corrected t = " << t << "\n";

            d=x0Eval*(t-1);
            xi=broyden(xi_1,d);
    
            file << "\n   || F(x_i+1) || = " << norm(eval(xi)+d);
            
            oszto=oszto*2;
        
            if(oszto>1048576)//2^20
            {
                file.close();
                throw NOT_CONVERGENT;
            }
        }
        
        file << " <= || F(x_i) || = " << xd << "\n";
        file << "   The method is convergent.\n";
        if(ex) file << "   || x* - x_i || = " << xi_vect[xi_vect.size()-1] << "\n";
        file << "   || f(x_i) || = " << xi_norm[xi_norm.size()-1] << "\n";
        
        conv1[conv1.size()-1]=xi_norm[xi_norm.size()-1];  
        if(ex) conv2[conv2.size()-1]=xi_vect[xi_vect.size()-1];
        
        t_old=t;
        if(t>=0.9999) break;
        else
            if(t<1-delta) t+=delta;
            else t=1;
    }

	//eredmény naplózása
    file << "\n\nThe method was successful!\n\n";
    file << "The result is:\n";
    file << "   ";
    writeVecToFile(xi,file);
    file << "\n";
    file << "|| f(x_i) || = " << norm(eval(xi)) << "\n";
    if(ex) file << "|| x* - x_i || = " << xi_vect[xi_vect.size()-1];

    file.close();
    return xi;
}
