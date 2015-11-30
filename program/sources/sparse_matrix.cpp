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

    node *p=oszlopok[o];//befûzzük a megfelelõ oszlopba
    if(p==NULL || p->indS>s)//ha az elejére kell befûzni
    {
        oszlopok[o]=elem;
        elem->nextS=p;
    }
    else//ha nem az elejére kell
    {
        node *r=p;
        p=p->nextS;
        while(p!=NULL && p->indS<s)//amíg megtalálja a helyét, p-vel néz elõre,
        {                          //tehát r és p közé kell befûzni
            r=p;
            p=p->nextS;
        }
        r->nextS=elem;
        elem->nextS=p;
    }

    p=sorok[s];//befûzzük a megfelelõ sorba
    if(p==NULL || p->indO>o)//ha az elejére kell befûzni
    {
        sorok[s]=elem;
        elem->nextO=p;
    }
    else//ha nem az elejére kell
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

    node *p=oszlopok[o];//az oszlopon megy végig
    if(p==NULL)//ha üres az oszlop, akkor csak 0 lehet
        return 0;
    else
    {
        while(p!=NULL)
        {
            if(p->indS==s)//ha van ebben a sorban elem
                return p->value;
            if(p->indS>s)//ha túlment, vagy nincs abban a sorban elem, tehát 0
                return 0;
            p=p->nextS;
        }
        return 0;//ha végigért, akkor szintén nincs eltárolva itt elem, tehát 0
    }
}

void sparse_matrix::modosit(int sor, int oszlop, double x)
{
    int o=oszlop-1;
    int s=sor-1;

    if(x==0 && at(sor,oszlop)!=0)//0-ra módosítok nem 0-t, akkor ki kell fûzni az elemet
    {
        node *p=oszlopok[o];//kifûzi az oszlopok közül
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

        p=sorok[s];//kifûzi a sorok közül
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
        delete p;//itt már lehet törölni
        nodeszam--;
    }
    else if(at(sor,oszlop)==0 && x!=0)//0-t módostítok nem 0-ra
    {
         befuz(new node(sor,oszlop,x));
    }
    else if(at(sor,oszlop)!=0 && x!=0)//nem 0-t módosítok nem 0-ra
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
    //else//0-t 0-ra, nincs teendõ
}

sparse_matrix::sparse_matrix(const sparse_matrix& a)
{
    n=-1;//jelezzük, hogy üres a mátrix
    nodeszam=0;
    copy(a);
}

sparse_matrix& sparse_matrix::operator=(const sparse_matrix &a)
{
    if(this==&a) return *this;//ha saját magát adjuk értékül
    if(oszlopok.size()==0) n=-1;//ha üres a bal oldali mátrix
    copy(a);
    return *this;
}

void sparse_matrix::copy(const sparse_matrix &a)//ha n > a.n akkor törölni kell
{
    if(n==-1)
    {//inicializálja a két vektort
        oszlopok.resize(a.n);
        sorok.resize(a.n);
        for(int i=0;i<a.n;i++)
        {
            oszlopok[i]=NULL;
            sorok[i]=NULL;
        }
    }
    if(n>a.n)
    {//törli a többletet
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
    {//megnöveli a vektorokat
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
                                                         //befûzni kell
            else if(regi!=0) modosit(i,j,uj);//különben elég módosítani
                                             //ha 0-ra módosítunk, akkor a modosit() kifûzi 
                                             //az elemet
        }
}
