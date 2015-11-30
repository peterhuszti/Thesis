#ifndef LINALG_H_INCLUDED
#define LINALG_H_INCLUDED

#include <vector>
#include "sparse_matrix.h"
#include <sstream>

#define thres 1e-12

enum errors{NOT_CONVERGENT};
//structs
struct qr//givens visszat�r�si �rt�ke, QR p�r
{
    sparse_matrix Q,R;

    qr(const sparse_matrix &q,const sparse_matrix &r): Q(q), R(r) {}
};

struct dd//choose visszat�r�si �rt�ke, indexp�r
{
    int ii,jj;

    void setIJ(int a, int b) { ii=a; jj=b; }
    dd(int i,int j): ii(i), jj(j) {}
};
//
template<typename T>
T absolute(T x);

double myAtof(const std::string &);
double norm(const std::vector<double> &v);//2-es norma

//sparse matrix
bool isUpperTriangular(const sparse_matrix &A);//fels�h�romsz�g m�trix-e

sparse_matrix T(const sparse_matrix &A);//transzpon�lt
sparse_matrix operator*(const sparse_matrix &a,const sparse_matrix &b);

qr givens(const sparse_matrix &mtx);
dd choose(const sparse_matrix &a,int);//0-zand� indexp�r
void setRCS(double &r,double &c, double &s,double a, double b);//givens elj�r�skor r,c,s
                                                               //kisz�m�t�sa
void javit(sparse_matrix &a);//0 k�r�l jav�t
sparse_matrix inverseUpperTriangular(const sparse_matrix &U);//inverze egy fels�h�romsz�g
                                                             //m�trixnak

//convert
sparse_matrix toSpare(const std::vector<std::vector<double> > &A);
std::vector<std::vector<double> > toDense(const sparse_matrix &A);

sparse_matrix operator*(const sparse_matrix&,const std::vector<std::vector<double> >&);
sparse_matrix operator*(const std::vector<std::vector<double> >&,const sparse_matrix&);

//dense matrix
double vectorMultScalar(const std::vector<double> &v, const std::vector<double> &u);//
                                                                            //skal�ris szorzat
void javit(std::vector<std::vector<double> > &a);//0 k�r�l jav�t
bool isUpperTriangular(const std::vector<std::vector<double> > &A);//fels�h�romsz�g-e

//=vektor
std::vector<double> operator*(const std::vector<double> &v, double c);
std::vector<double> operator+(const std::vector<double> &v, const std::vector<double> &u);
std::vector<double> operator*(const std::vector<std::vector<double> > &A, const std::vector<double> &x);

//=m�trix
std::vector<std::vector<double> > operator*(const std::vector<std::vector<double> > &A, double c);
std::vector<std::vector<double> > operator*(const std::vector<double> &v, const std::vector<double> &u);
std::vector<std::vector<double> > operator+(const std::vector<std::vector<double> > &A, const std::vector<std::vector<double> > &B);
std::vector<std::vector<double> > operator*(const std::vector<std::vector<double> > &A, const std::vector<std::vector<double> > &B);

std::vector<std::vector<double> > T(const std::vector<std::vector<double> > &A);//transzpon�lt
std::vector<std::vector<double> > inverseUpperTriangular(const std::vector<std::vector<double> > &U);

//write
void write_mtx(const std::vector<std::vector<double> > &mtx);
void write_vec(const std::vector<double> &v);
void write_mtx(const sparse_matrix &a);
bool equals(const sparse_matrix &a, const sparse_matrix &b);
bool equals(const std::vector<std::vector<double> > &a, const std::vector<std::vector<double> > &b);
#endif // LINALG_H_INCLUDED
