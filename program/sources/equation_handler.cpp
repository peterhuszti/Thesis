#include "equation_handler.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stack>
#include "linalg.h"

equation_handler::equation_handler()
{
    make_prec();
	setActeq(0);
	n=0;
	n_old=0;
}

void equation_handler::newclick()
{
    n=0;
    n_old=0;
    x0.resize(0);
    lengyelformak.resize(0);
    equations.resize(0);
    exact.resize(0);
	setActeq(0);
}

//getter
std::vector<double> equation_handler::getX0_vect()
{
    return x0;
}

std::vector<double> equation_handler::getEx_vect()
{
    return exact;
}

std::vector<std::queue<std::string> > equation_handler::getLengyel()
{
    return lengyelformak;
}

int equation_handler::getActeq()
{
    return act_eq;
}

std::string equation_handler::getEq(int i)
{
    return equations[i];
}

int equation_handler::getN()
{
    return n;
}

int equation_handler::getN_old()
{
    return n_old;
}

std::string equation_handler::getX0()
{
    std::string temp="(";
    for(int i=0;i<n;i++)
    {
        if(i>=x0.size())
        {
            temp+="x";
            temp+=NumberToString(i+1);
        }
        else temp+=NumberToString(x0[i]);
        if(i+1!=n) temp+="; ";
    }
    temp+=")";
    return temp;
}

std::string equation_handler::getEx()
{
    if(exact.size()==0) return "";
    std::string temp="(";
    for(int i=0;i<n;i++)
    {
        if(i>=exact.size())
        {
            temp+="x";
            temp+=NumberToString(i+1);
        }
        else temp+=NumberToString(exact[i]);
        if(i+1!=n) temp+="; ";
    }
    temp+=")";
    return temp;
}


std::string equation_handler::getLengyel(int i)
{
    std::vector<std::queue<std::string> > seged=lengyelformak;
    std::string str="";
    while(!seged[i].empty())
    {
       std::string temp=seged[i].front();
       seged[i].pop();
       str+=temp;
       str+=" ";
    }
    return str;
}

//setter
void equation_handler::setN(int p)
{
    n_old=n;
    n=p;
    lengyelformak.resize(n);
}

void equation_handler::setActeq(int j)
{
    act_eq=j;
}

void equation_handler::setEx(const std::string &str_of)
{
    std::string str=str_of;
    if(str=="")
    {
        return;
    }
    exact.resize(n,0);
    int i=1;
    int j=0;
    bool b=true;
    char c=str[0];
    std::string temp="";
    while(b)
    {
        while(c!=';' && c!='\0')//; az elválasztó, \0 a vége
        {
            temp+=c;
            c=str[i++];
        }
        if(temp=="Pi" || temp=="pi" || temp=="PI" || temp=="pI") exact[j++]=PI;
        else if(temp=="e" || temp=="E") exact[j++]=e;
        else
        {
            double temp2=atof(temp.c_str());
            if(absolute(temp2)<thres) exact[j++]=0;//ha 0-hoz túl közel van, akkor nem lesz hozzáadva
            else exact[j++]=temp2;
        }
        if(c=='\0') b=false;//ha vége
        else
        {
            c=str[i++];
            temp="";
        }
    } 
}

void equation_handler::setX0(const std::string &str_of)
{
    std::string str=str_of;
    x0.resize(n,0);
    if(str=="x01;x02;...;x0n") return;
    int i=1;
    int j=0;
    bool b=true;
    char c=str[0];
    std::string temp="";
    while(b)
    {
        while(c!=';' && c!='\0')//; az elválasztó, \0 a vége
        {
            temp+=c;
            c=str[i++];
        }
        if(temp=="Pi" || temp=="pi" || temp=="PI" || temp=="pI") x0[j++]=PI;
        else if(temp=="e" || temp=="E") x0[j++]=e;
        else
        {
            double temp2=atof(temp.c_str());
            if(absolute(temp2)<thres) x0[j++]=0;//ha 0-hoz túl közel van, akkor nem lesz hozzáadva
            else x0[j++]=temp2;
        }
        if(c=='\0') b=false;//ha vége
        else
        {
            c=str[i++];
            temp="";
        }
    }
}

void equation_handler::addEQ(std::string str)
{
    equations.push_back(str+'\n');
    elozo=1;
}

void equation_handler::removeEQ()
{
    equations.pop_back();
}

void equation_handler::editEQ(std::string str,int l)
{
    std::string temp=str+'\n';
    equations[l]=temp;
}
//
std::string equation_handler::next(int l)//kiolvassa a következõ szimbólumot az 
{                                        //l.-ik egyenletbõl
    //elozo:
    //( -> 1
    //egyebkent -> -1
    
    std::string next_simb="";
    next_simb+=equations[l][act++]; 
            
    switch(next_simb[0])
    {
        case ' '://skip whitespace
            next_simb=next(l);
            break;
        case 'P': case 'p'://Pi
        {
            char temp=equations[l][act];
            if(temp=='I' || temp=='i')
            {
                next_simb+=equations[l][act++];
                elozo=-1;
            }
            else throw UNKNOWN;
            break;
        }
        case 'e': case'E':
            elozo=-1;
            break;
        case 'a'://asin acos atan actg
        {
            std::string temp="";
            temp+=equations[l][act];
            temp+=equations[l][act+1];
            temp+=equations[l][act+2];
            if(temp=="tan" || temp=="cos" || temp=="sin")
            {
                next_simb+=equations[l][act++];
                next_simb+=equations[l][act++];
                next_simb+=equations[l][act++];
                elozo=-1;
            }
            else throw UNKNOWN;
            break;
        }
        case 's'://sin sqrt
        {
            std::string temp="";
            temp+=equations[l][act];
            temp+=equations[l][act+1];
            if(temp=="in")
            {
                next_simb+=equations[l][act++];
                next_simb+=equations[l][act++];
            }
            else
            {
                temp+=equations[l][act+2];
                if(temp=="qrt")
                {
                    next_simb+=equations[l][act++];
                    next_simb+=equations[l][act++];
                    next_simb+=equations[l][act++];
                    elozo=-1;
                }
                else throw UNKNOWN;
            }
            break;
        }
        case 'c'://cos ctg cbrt
        {
            std::string temp="";
            temp+=equations[l][act];
            temp+=equations[l][act+1];
            if(temp=="os" || temp=="tg")
            {
                next_simb+=equations[l][act++];
                next_simb+=equations[l][act++];
            }
            else
            {
                temp+=equations[l][act+2];
                if(temp=="brt")
                {
                    next_simb+=equations[l][act++];
                    next_simb+=equations[l][act++];
                    next_simb+=equations[l][act++];
                    elozo=-1;
                }
                else throw UNKNOWN;
            }
            break;
        }
        case 't'://tan
        {
            std::string temp="";
            temp+=equations[l][act];
            temp+=equations[l][act+1];
            if(temp=="an")
            {
                next_simb+=equations[l][act++];
                next_simb+=equations[l][act++];
                elozo=-1;
            }
            else throw UNKNOWN;
            break;
        }
        case 'l'://ln
        {
            char temp=equations[l][act];
            if(temp=='n')
            {
                next_simb+=equations[l][act++];
                elozo=-1;
            }
            else throw UNKNOWN;
            break;
        }
        case 'x': case 'X':
        {
            std::string temp="";
            while(isdigit(equations[l][act]))
            {
                temp;
                temp+=equations[l][act++];
            }
            double s=myAtof(temp);
            if(s==0 || s>n) throw XI_ERROR;
            else next_simb+=temp; 
            elozo=-1;
            break;
        }
        case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': case '0':
        {
            std::string temp="";
            while(isdigit(equations[l][act]) || equations[l][act]==',')
            {
                temp;
                temp+=equations[l][act++];
            }
            next_simb+=temp;
            elozo=-1;
            break;  
        }
        case '(':
            open++;
            elozo=1;
            break;
        case '-':
            if(elozo==1) next_simb+="-";
            elozo=-1;
            break;
        case ')':
            if(open==0) throw INVALID_BRACKET;
            open--;
            break;
        case '\n'://vége
            if(!isdigit(equations[l][act-2]) && equations[l][act-2]!=')')
                throw OPERATOR_END;
            break;
        case '+': case '*': case '/': case '^':
            elozo=-1;
            break;
        default:// egyéb ismeretlen karakter 
            throw UNKNOWN;
            break;
    }
   return next_simb;
}

void equation_handler::lengyel_forma(int l)
{      
    open=0;
    act=0;
    std::stack<std::string> verem;
    std::string simb=next(l);//szimbólumonként olvas

    while(simb[0]!='\n')
    {
        switch(simb[0])
        {
            case '(':
                verem.push(simb);
                break;
            case ')':
                while(verem.top()!="(")
                {
                    lengyelformak[l].push(verem.top());
                    verem.pop();
                }
                verem.pop();
                break;
            case 'x': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9': case '0':
            case 'P': case 'e':
                lengyelformak[l].push(simb);
                break;
            default:
                int pr=prec[simb];
                if(if_bj(simb))
                {
                    while(!verem.empty() && verem.top()!="(" && prec[verem.top()]>=pr)
                    {
                        lengyelformak[l].push(verem.top());
                        verem.pop();
                    }
                }
                else
                {
                    while(!verem.empty() && verem.top()!="(" && prec[verem.top()]>pr)
                    {
                        lengyelformak[l].push(verem.top());
                        verem.pop();
                    }
                }
                verem.push(simb);
                break;
        }
        simb=next(l);
    }
    if(open!=0) throw INVALID_BRACKET;
    while(!verem.empty())
    {
        lengyelformak[l].push(verem.top());
        verem.pop();
    }
}

void equation_handler::make_prec()
{
    prec["--"]=0;

    prec["+"]=1;
    prec["-"]=1;

    prec["*"]=2;
    prec["/"]=2;

    prec["^"]=3;

    prec["sin"]=5;
    prec["cos"]=5;
    prec["tan"]=5;
    prec["ln"]=5;
    prec["ctg"]=5;
    prec["atan"]=5;
    prec["asin"]=5;
    prec["acos"]=5;
    prec["actg"]=5;
    prec["sqrt"]=5;
    prec["cbrt"]=5;

    bj.push_back("+");
    bj.push_back("*");
    bj.push_back("/");
    bj.push_back("-");
}

bool equation_handler::if_bj(const std::string &str)
{
    for(std::vector<std::string>::iterator it=bj.begin();it!=bj.end();it++)
    {
        if(*it==str) return true;
    }
    return false;
}

std::string equation_handler::NumberToString(double t)
{
     std::ostringstream oo;
     oo << t;
     return oo.str();
}
