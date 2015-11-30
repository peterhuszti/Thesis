#ifndef SPARSE_MATRIX_H_INCLUDED
#define SPARSE_MATRIX_H_INCLUDED

#include <vector>

struct node
{
    node *nextO;//következõ oszlop, amiben van indS.-ik sorban elem
    node *nextS;//következõ sor, amiben van indO.-ik az oszlopban elem
    double value;
    int indS;//sor index
    int indO;//oszlop index

    node(int s,int o,double v)
    {
        value=v;
        nextO=nextS=NULL;
        indS=s-1;
        indO=o-1;
    }
};

class sparse_matrix
{

private:

    int n;//méret
    int nodeszam;//elemszám
    std::vector<node*> oszlopok;
    std::vector<node*> sorok;

public:

    sparse_matrix();
    sparse_matrix(int);//méret
    sparse_matrix(const sparse_matrix& a);//copy konstruktor
    ~sparse_matrix();

    sparse_matrix& operator=(const sparse_matrix&);
    void copy(const sparse_matrix &);

//getter
    int getSize() const;
    int getNodeszam();
//setter
    void setSize(int);

    void befuz(node*);
    void modosit(int,int,double);
    double at(int,int) const;//=mtx[int][int]

    friend sparse_matrix T(const sparse_matrix&);//transzponált
    //lehet szorozni ritka és sûrû mátrixszal és vektorral is akkor, ha mátrix a végeredmény
    friend sparse_matrix operator*(const sparse_matrix&,const sparse_matrix&);
    friend sparse_matrix operator*(const sparse_matrix&,const std::vector<std::vector<double> >&);
    friend sparse_matrix operator*(const std::vector<std::vector<double> >&,const sparse_matrix&);

    friend void write_mtx(const sparse_matrix &);
};

#endif // SPARE_MATRIX_H_INCLUDED
