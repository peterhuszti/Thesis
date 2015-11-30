#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <stack>
#include "linalg.h"
#include "solver.h"
#include "equation_handler.h"
#include "sparse_matrix.h"
#include <time.h>

using namespace std;

void go()
{
    cout << "\n\nPress enter key to continue...";
    cin.get();
    system("CLS");
}

void test_sparse(const sparse_matrix &mtx)
{
    cout << "Testing\n";
    write_mtx(mtx);
    cout << "matrix...";

    cout << "\n\n    Sparse matrix functions\n\n";
    cout << "  Testing...\n";

    cout << "Transpose\n";
    if(!equals(mtx,T(T(mtx))))
    {
        cout << "  Error in transpose operation!\n";
        return;
    }

    cout << "Multiplication\n";
    if(!equals(T(mtx*mtx),T(mtx)*T(mtx)))
    {
        cout << "  Error in matrix multiplication!\n";
        return;
    }

    cout << "QR decomposition\n";
    qr temp=givens(mtx);
    if(!isUpperTriangular(temp.R))
    {
        cout << "  R is not upper triangulat matrix!\n";
        return;
    }
    if(!equals(temp.Q*temp.R,mtx))
    {
        cout << "  Error in qr decomposition!\n";
        return;
    }

    cout << "Inverse of an upper triangular matrix\n";
    if(isUpperTriangular(mtx))
    {
        sparse_matrix I(mtx.getSize());
        for(int i=1;i<I.getSize()+1;i++)
            I.befuz(new node(i,i,1));
        if(!equals(I,mtx*inverseUpperTriangular(mtx)))
        {
            cout << "  Error in inverse computing!\n";
            return;
        }
    }
    else
    {
        cout << "  Can not test, input matrix is not upper triangular!\n";
    }

    cout << "\n\nThere is no errors in the operations!!\n";

    go();
}

void test_dense(const vector<vector<double> > &mtx)
{
    cout << "Testing\n";
    write_mtx(mtx);
    cout << "matrix...";

    cout << "\n\n    Dense matrix functions\n\n";
    cout << "  Testing...\n";

    cout << "Transpose\n";
    if(!equals(mtx,T(T(mtx))))
    {
        cout << "  Error in transpose operation!\n";
        return;
    }

    cout << "Multiplication\n";
    if(!equals(T(mtx*mtx),(T(mtx)*T(mtx))))
    {
        cout << "  Error in matrix multiplication!\n";
        return;
    }



    cout << "Inverse of an upper triangular matrix\n";
    if(isUpperTriangular(mtx))
    {
        vector<vector<double> > I(mtx.size());
        for(int i=0;i<I.size();i++)
        {
            I[i].resize(I.size());
            I[i][i]=1;
        }
        if(!equals(I,mtx*inverseUpperTriangular(mtx)))
        {
            cout << "  Error in inverse computing!\n";
            return;
        }
    }
    else
    {
        cout << "  Can not test, input matrix is not upper triangular!\n";
    }

    cout << "\n\nThere is no errors in the operations!!\n";

    go();
}

void test_eh(string str)
{
    cout << "Testing Reverse Polish Notation with\n   ";
    cout << str;
    cout << "\nequation...";

    equation_handler eh;
    eh.setN(10);
    eh.addEQ(str);

    cout << "\n\n The symbols are: \n";
    eh.lengyel_forma(0);

    cout << "\n\n The result is: \n";
    cout << eh.getLengyel(0);

    go();
}

void write_text(const sparse_matrix &S, const vector<vector<double> > &D)
{
    cout << "Testing matrix functions with\n";
    write_mtx(S);
    cout << "matrix S, and with\n";
    write_mtx(D);
    cout << "matrix D...\n\n";
}

void test_matrix(const sparse_matrix &S, const vector<vector<double> > &D)
{
    write_text(S,D);
    cout << "S*D=\n";
    write_mtx(S*D);
    go();

    write_text(S,D);
    cout << "D*S=\n";
    write_mtx(D*S);
    go();
}

void test_vector(const vector<double> &v, const vector<double> u, const vector<vector<double> > &D)
{
    cout << "< ";
    write_vec(v);
    cout << "   ,   ";
    write_vec(u);
    cout << " > =";
    cout << vectorMultScalar(v,u);
    go();

    write_mtx(D);
    cout << "* \n\n";
    write_vec(u);
    cout << " = \n";
    write_vec(D*u);
    go();
}

int main()
{
    cout << "------TEST linalg package------\n\n";

    sparse_matrix mtx(5);
    mtx.befuz(new node(1,1,1));
    mtx.befuz(new node(4,2,2));
    mtx.befuz(new node(2,5,-1));
    mtx.befuz(new node(1,3,-2));
    mtx.befuz(new node(2,4,1));
    test_sparse(mtx);

    sparse_matrix mtx2(5);
    mtx2.befuz(new node(1,1,1));
    mtx2.befuz(new node(4,4,2));
    mtx2.befuz(new node(3,5,-1));
    mtx2.befuz(new node(1,3,-2));
    mtx2.befuz(new node(2,4,1));
    mtx2.befuz(new node(2,2,1));
    mtx2.befuz(new node(3,3,-4));
    mtx2.befuz(new node(5,5,3));
    test_sparse(mtx2);

    vector<vector<double> > mtx3=toDense(mtx2);
    test_dense(mtx3);

    test_matrix(mtx,mtx3);

    vector<double> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(-1);
    v.push_back(0.5);
    vector<double> u;
    u.push_back(0);
    u.push_back(0);
    u.push_back(0);
    u.push_back(-2.5);
    u.push_back(3.14);

    test_vector(v,u,mtx3);

    cout << "TEST equation_handler class...\n\n";

    string str1="x1+x2+sqrt(x1*x2)-28";
    test_eh(str1);

    string str2="5*ln(x1)+3*ln(x2)-2";
    test_eh(str2);

    string str3="x5*x1*x3^2+x6*x2*x4^2-3";
    test_eh(str3);

    return 0;
}
