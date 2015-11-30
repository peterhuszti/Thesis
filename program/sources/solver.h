#ifndef SOLVER_H_INCLUDED
#define SOLVER_H_INCLUDED

#include <vector>
#include <queue>
#include <stack>
#include <fstream>
#include <string>

#define PI 3.141592653589793238462643383279
#define e 2.718281828459045235360287471352

struct td//kiértékeléshez
{
    double elso,masodik;
};

class solver
{

private:
    
    std::ofstream file;//napló
    
    void writeVecToFile(const std::vector<double> &,std::ostream&);//egy vektor kiírása egy fájlba
    
    bool ex;//van-e pontos megoldás
    
	//paraméterek
    double epsilon;
    int k;
    int kmax;

    std::vector<double> xi_norm//g
    std::vector<double> xi_vect;//h
    std::vector<double> conv1;//g
    std::vector<double> conv2;//h

    std::vector<double> exact;
    int deg_rad;//fok vagy radién
    std::vector<std::queue<std::string> > F;//függvény

	//konverziós függvények
    std::string NumberToString(double);
    td StringToNumbers(std::stack<std::string> &,const std::vector<double> &);
    double StringToNumber(std::stack<std::string> &,const std::vector<double> &);
    
    std::vector<double> broyden(const std::vector<double> &,const std::vector<double>&);//Broyden-módszer
    int chooseX(const std::string &);//megfelelő x kiválasztása

public:

    solver();
//getter
    int getKmax();
	std::vector<double> getXiNorm();
    std::vector<double> getXiVect();
    std::vector<double> getConv1();
    std::vector<double> getConv2();
//setter
    void setEpsilon(double);
    void setK(int);
    void setKmax(int);
    void setExact(const std::vector<double> &);
    void addF(const std::vector<std::queue<std::string> > &);
	
    void newclick();
    void backclick();

    void changeDegRad(int);
   
    std::vector<double> eval(const std::vector<double> &);//kiértékel
    std::vector<double> cont(const std::vector<double> &);//folytatás módszere

};

#endif // SOLVER_H_INCLUDED
