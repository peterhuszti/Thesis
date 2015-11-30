#ifndef EQUATION_HANDLER_H_INCLUDED
#define EQUATION_HANDLER_H_INCLUDED

#include <fstream>
#include <map>
#include <vector>
#include <queue>

#define PI 3.141592653589793238462643383279
#define e 2.718281828459045235360287471352

class equation_handler
{

private:

	int act_eq;//aktuálisan beolvasott egyenlet
	int act;//hanyadik karakternél jár a lengyelformára hozás
	int n;
	int n_old;//editnél kell
	int elozo;//negálás eldöntése
	int open;
	std::map<std::string,int> prec;
    std::vector<std::string> bj;
	std::vector<double> x0;
	std::vector<double> exact;
    std::vector<std::string> equations;
    std::vector<std::queue<std::string> > lengyelformak;

    std::string next(int l);
    bool if_bj(const std::string &str);
    void make_prec();

public:

    enum errors{UNKNOWN, OPERATOR_END, XI_ERROR, INVALID_BRACKET};

    equation_handler();
    void newclick();

//getter
    int getN();
    int getN_old();
    std::string getEx();
	std::vector<double> getEx_vect();
	std::string getX0();
	std::vector<double> getX0_vect();
	int getActeq();
	std::string getEq(int);
	std::string getLengyel(int i);
	std::vector<std::queue<std::string> > getLengyel();
//setter
	void setN(int p);
	void setN_old(int p);
	void setEx(const std::string &);
	void setX0(const std::string &);
	void setActeq(int j);
	
	void addEQ(std::string str);
	void removeEQ();
	void editEQ(std::string str,int l);
	
	std::string NumberToString(double t);
	
	void lengyel_forma(int l);
};

#endif // EQUATION_HANDLER_H_INCLUDED
