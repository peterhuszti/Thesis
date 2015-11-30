#include <iostream>
#include <map>
#include "sparse_matrix.h"

sparse_matrix::sparse_matrix()
{
    n=-1;
    nodeszam=0;
}

sparse_matrix::sparse_matrix(int meret)
{
    setSize(meret);
}

sparse_matrix::~sparse_matrix()
{
    for(int i=0;i<n;i++)
    {
        node* p=oszlopok[i];
        node* r;
        while(p!=NULL)
        {
            r=p;
            p=p->nextS;
            delete r;
        }
    }
}
//getter
int sparse_matrix::getNodeszam()
{
    return nodeszam;
}

int sparse_matrix::getSize() const
{
    return n;
}
//setter
void sparse_matrix::setSize(int meret)
{
    n=meret;
    nodeszam=0;
    oszlopok.resize(n);
    sorok.resize(n);
    for(int i=0;i<n;i++)
    {
        oszlopok[i]=NULL;
        sorok[i]=NULL;
    }
}
//
void sparse_matrix::befuz(node *elem)
{
    int o=elem->indO;
    int s=elem->indS;

    node *p=oszlopok[o];//bef�zz�k a megfelel� oszlopba
    if(p==NULL || p->indS>s)//ha az elej�re kell bef�zni
    {
        oszlopok[o]=elem;
        elem->nextS=p;
    }
    else//ha nem az elej�re kell
    {
        node *r=p;
        p=p->nextS;
        while(p!=NULL && p->indS<s)//am�g megtal�lja a hely�t, p-vel n�z el�re,
        {                          //teh�t r �s p k�z� kell bef�zni
            r=p;
            p=p->nextS;
        }
        r->nextS=elem;
        elem->nextS=p;
    }

    p=sorok[s];//bef�zz�k a megfelel� sorba
    if(p==NULL || p->indO>o)//ha az elej�re kell bef�zni
    {
        sorok[s]=elem;
        elem->nextO=p;
    }
    else//ha nem az elej�re kell
    {
        node *r=p;
        p=p->nextO;
        while(p!=NULL && p->indO<o)
        {
            r=p;
            p=p->nextO;
        }
        r->nextO=elem;
        elem->nextO=p;
    }
    nodeszam++;
}

double sparse_matrix::at(int sor, int oszlop) const
{
    int o=oszlop-1;
    int s=sor-1;

    node *p=oszlopok[o];//az oszlopon megy v�gig
    if(p==NULL)//ha �res az oszlop, akkor csak 0 lehet
        return 0;
    else
    {
        while(p!=NULL)
        {
            if(p->indS==s)//ha van ebben a sorban elem
                return p->value;
            if(p->indS>s)//ha t�lment, vagy nincs abban a sorban elem, teh�t 0
                return 0;
            p=p->nextS;
        }
        return 0;//ha v�gig�rt, akkor szint�n nincs elt�rolva itt elem, teh�t 0
    }
}

void sparse_matrix::modosit(int sor, int oszlop, double x)
{
    int o=oszlop-1;
    int s=sor-1;

    if(x==0 && at(sor,oszlop)!=0)//0-ra m�dos�tok nem 0-t, akkor ki kell f�zni az elemet
    {
        node *p=oszlopok[o];//kif�zi az oszlopok k�z�l
        node *r=NULL;
        while(p!=NULL && p->indS<s)
        {
            r=p;
            p=p->nextS;
        }
        if(p!=NULL && p->indS==s)
        {
            if(oszlopok[o]==p) oszlopok[o]=p->nextS;
            else r->nextS=p->nextS;
            p->nextS=NULL;
        }

        p=sorok[s];//kif�zi a sorok k�z�l
        r=NULL;
        while(p!=NULL && p->indO<o)
        {
            r=p;
            p=p->nextO;
        }
        if(p!=NULL && p->indO==o)
        {
            if(sorok[s]==p) sorok[s]=p->nextO;
            else r->nextO=p->nextO;
            p->nextO=NULL;
        }
        delete p;//itt m�r lehet t�r�lni
        nodeszam--;
    }
    else if(at(sor,oszlop)==0 && x!=0)//0-t m�dost�tok nem 0-ra
    {
         befuz(new node(sor,oszlop,x));
    }
    else if(at(sor,oszlop)!=0 && x!=0)//nem 0-t m�dos�tok nem 0-ra
    {
        bool b=false;
        node *p=oszlopok[o];
        while(!b)
        {
            if(p->indS==s)
            {
                b=true;
                p->value=x;
            }
            p=p->nextS;
        }
    }
    //else//0-t 0-ra, nincs teend�
}

sparse_matrix::sparse_matrix(const sparse_matrix& a)
{
    n=-1;//jelezz�k, hogy �res a m�trix
    nodeszam=0;
    copy(a);
}

sparse_matrix& sparse_matrix::operator=(const sparse_matrix &a)
{
    if(this==&a) return *this;//ha saj�t mag�t adjuk �rt�k�l
    if(oszlopok.size()==0) n=-1;//ha �res a bal oldali m�trix
    copy(a);
    return *this;
}

void sparse_matrix::copy(const sparse_matrix &a)//ha n > a.n akkor t�r�lni kell
{
    if(n==-1)
    {//inicializ�lja a k�t vektort
        oszlopok.resize(a.n);
        sorok.resize(a.n);
        for(int i=0;i<a.n;i++)
        {
            oszlopok[i]=NULL;
            sorok[i]=NULL;
        }
    }
    if(n>a.n)
    {//t�rli a t�bbletet
        for(int i=a.n;i<n;i++)
        {
            node* p=oszlopok[i];
            node* r;
            while(p!=NULL)
            {
                r=p;
                p=p->nextS;
                delete r;
            }
        }
    }
    else
    {//megn�veli a vektorokat
        oszlopok.resize(a.n);
        sorok.resize(a.n);
    }
    n=a.n;
    nodeszam=a.nodeszam;
    for(int i=1;i<n+1;i++)
        for(int j=1;j<n+1;j++)
        {
            double uj=a.at(i,j);
            double regi=at(i,j);
            if(uj!=0 && regi==0) befuz(new node(i,j,uj));//ha nem volt ott elem, mert 0, akkor 
                                                         //bef�zni kell
            else if(regi!=0) modosit(i,j,uj);//k�l�nben el�g m�dos�tani
                                             //ha 0-ra m�dos�tunk, akkor a modosit() kif�zi 
                                             //az elemet
        }
}
