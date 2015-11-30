#include <iostream>
#include <map>
#include <cmath>
#include <stdlib.h>
#include "sparse_matrix.h"
#include "linalg.h"

template<typename T>
T absolute(T x)
{
    if(x<0) return (-1)*x;
    else return x;
}

double myAtof(const std::string &str)
{
    double res;
    std::stringstream stream(str);
    stream >> res;
    return res;
}

double norm(const std::vector<double> &v)
{
    double res=0;
    for(int i=0;i<v.size();i++)
    {
        res+=v[i]*v[i];
    }
    return sqrt(res);
}
//sparse matrix

bool isUpperTriangular(const sparse_matrix &A)
{
    int n=A.getSize();
    for(int i=1;i<n+1;i++)
    {
        for(int j=1;j<i;j++)
        {
            if(A.at(i,j)!=0) return false;
        }
    }
    return true;
}

sparse_matrix T(const sparse_matrix &A)
{
    sparse_matrix res(A.getSize());
    for(int i=1;i<res.n+1;i++)
    {
        for(int j=1;j<res.n+1;j++)
        {
            double temp=A.at(i,j);
            if(res.at(j,i)!=0 && temp!=0) res.modosit(j,i,temp);
            else if(temp!=0)res.befuz(new node(j,i,temp));
        }
    }
    return res;
}

sparse_matrix operator*(const sparse_matrix &a,const sparse_matrix &b)
{
    int n=a.getSize();
    sparse_matrix res(n);

    std::map<int,bool> ind;
    for(int i=0;i<n;i++) ind[i]=true;//az �res oszlopokat t�rolja, hogy ne 
                                     //kelljen �jra megvizsg�lni

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            node* p=a.sorok[i];
            node* q;
            if(ind[j])
            {
                q=b.oszlopok[j];

                if(p==NULL)break;//ha �res a sor, akkor mehet�nk a k�vetkez� sorra
                if(q==NULL)//ha �res a sor
                {
                    do
                    {
                        ind[j]=false;//�res oszlop mgejel�l�se
                        q=b.oszlopok[++j];
                    }while(q==NULL && j<n);//megkeress�k a k�vetkez� nem�res oszlopot
                }
                if(j==n) break;//ha az oszlopok v�g�re �rt�nk
                double sum=0;
                while(p!=NULL && q!=NULL)
                {
                    if(p->indO<q->indS) p=p->nextO;
                    else if(p->indO>q->indS) q=q->nextS;
                    //megkeress�k az els� olyan p�rt, ahol egyezik az oszlop �s sorindex, 
                    //teh�t �ssze kell szorozni �ket
                    else
                    {
                        sum+=p->value*q->value;
                        p=p->nextO;
                        q=q->nextS;
                    }
                }//am�g a sor vagy az oszlop v�g�re nem �r�nk
                if(sum!=0)
                {
                    res.befuz(new node(i+1,j+1,sum));//ha nem 0, akkor be kell f�zni
                }
            }
        }
    }
    return res;
}

sparse_matrix operator*(const sparse_matrix& S,const std::vector<std::vector<double> >& D)
{
    int n=S.getSize();
    sparse_matrix res(n);

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            double sum=0;
            node* p=S.sorok[j];
            while(p!=NULL)
            {
                sum+=p->value*D[p->indO][i];
                p=p->nextO;
            }
            if(sum!=0)
            {
                res.befuz(new node(j+1,i+1,sum));
            }
        }
    }
    return res;
}
sparse_matrix operator*(const std::vector<std::vector<double> >& D,const sparse_matrix& S)
{
    int n=S.getSize();
    sparse_matrix res(n);

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            double sum=0;
            node* p=S.oszlopok[j];
            while(p!=NULL)
            {
                sum+=p->value*D[i][p->indS];
                p=p->nextS;
            }
            if(sum!=0)
            {
                res.befuz(new node(i+1,j+1,sum));
            }
        }
    }
    return res;
}

qr givens(const sparse_matrix &mtx)
{
    int n=mtx.getSize();
    sparse_matrix R(mtx);//inicializ�l�s
    std::vector<sparse_matrix> G;//forgat�m�trixok

    int f=1;//forgat�sok sz�ma
    bool kesz=false;
    while(!kesz)
    {
        double c,s;
        dd chosen(-1,-1);
        chosen=choose(R,f);//0-zand� m�trixelem kiv�laszt�sa (indexp�r)
        if(chosen.ii == -1)//ha nincs mit null�zni, akkor k�szen vagyunk
        {
            kesz=true;
        }
        else
        {
            setRCS(c,s,R.at(chosen.jj,chosen.jj),R.at(chosen.ii,chosen.jj));
                        //r,c,s kisz�m�t�sa
            G.push_back(sparse_matrix(n));//forgat�si m�trix iniciliz�l�sa
            if(c!=0)
            {//bef�zz�k a megfelel� helyekre a kisz�m�tott �rt�keket
                G[f-1].befuz(new node(chosen.jj,chosen.jj,c));
                G[f-1].befuz(new node(chosen.ii,chosen.ii,c));
            }
            if(s!=0)
            {
                G[f-1].befuz(new node(chosen.ii,chosen.jj,s));
                G[f-1].befuz(new node(chosen.jj,chosen.ii,(-1)*s));
            }

            for(int j=1;j<n+1;j++)//felt�lti a f��tl�t 1-esekkel
            {
                if(j!=chosen.ii && j!=chosen.jj) G[f-1].befuz(new node(j,j,1));
            }
            R=G[f-1]*R;//forgat�s
            javit(R);
            f++;
        }
    }
    if(f==1)//ha nem volt forgat�s, akkor m�r alapb�l fels�h�romsz�g alak� votl a m�trix
    {
        sparse_matrix a(n);//Q
        for(int i=1;i<n+1;i++)
            a.befuz(new node(i,i,1));
        sparse_matrix b(mtx);//R
        qr res(a,b);
        return res;
    }
    sparse_matrix Q=T(G[0]);//Q kisz�m�t�sa
    for(int i=1;i<G.size();i++)
    {
        Q=Q*T(G[i]);
    }
    javit(Q);
    
    qr res(Q,R);
    return res;
}

void setRCS(double &c, double &s,double a, double b)
{
    if(b==0)
    {
        c=copysign(1,a);
        s=0;
     //   r=absolute(a);
    }
    else if(a==0)
    {
        c=0;
        s=(-1)*copysign(1,b);
    //    r=absolute(b);
    }
    else if(absolute(b)>absolute(a))
    {
        double t=a/b;
        double u=copysign(sqrt(1+t*t),b);
        s=(-1)/u;
        c=(-1)*s*t;
    //    r=b*u;
    }
    else
    {
        double t=b/a;
        double u=copysign(sqrt(1+t*t),a);
        c=1/u;
        s=(-1)*c*t;
     //   r=a*u;
    }
}

dd choose(const sparse_matrix &a,int f)
{
    dd res(-1,-1);
    int n=a.getSize();
    for(int i=1;i<n+1;i++)
    {
        for(int j=n;j>i;j--)
        {
            if(a.at(j,i)!=0 && absolute(a.at(j,i))>thres)
            {
                res.setIJ(j,i);
                return res;
            }
        }
    }
    /*if(f<n-1)
    {
        int i=n-f+1;
        if(a.at(i,i-1)!=0)
        {
            res.setIJ(i,i-1);
        }
    }
    else
    {
        int i=f-n+1;
        if(a.at(i+2,i+1)!=0)
        {
            res.setIJ(i+2,i+1);
        }
    }*/
    return res;
}

void javit(sparse_matrix &a)
{
    for(int i=1;i<a.getSize()+1;i++)
    {
        for(int j=1;j<a.getSize()+1;j++)
        {
            if(absolute(a.at(i,j))<thres && a.at(i,j)!=0) a.modosit(i,j,0);
        }
    }
}

sparse_matrix inverseUpperTriangular(const sparse_matrix &U)
{
    int n=U.getSize();
    sparse_matrix res(n);
    for(int i=1;i<n+1;i++)//egys�gm�trix
    {
        res.befuz(new node(i,i,1));
    }
    std::vector<double> D(n,0);

    for(int i=1;i<n+1;i++)
    {
        double temp=U.at(i,i);
        if(temp!=0) D[i-1]=1/temp;
    }

    sparse_matrix C(U);

    for(int i=1;i<n+1;i++)//norm�lja a m�trixot �gy, hogy a f��tl�ban 1-esek legyenek
    {
        for(int j=1;j<n+1;j++)
        {
            C.modosit(i,j,C.at(i,j)*D[i-1]);
        }
    }
    
    C=T(C);//als�h�romsz�g m�trix lesz

    int i=1;
    int j=0;
    do
    {
        int k=i;
        while(k!=n)
        {
            double temp=C.at(k+1,j+1);
            if(i==1)//f��tl� alatti �tl�
            {
                res.modosit(k+1,j+1,(-1)*temp);
            }
            else if(i==2)
            {
                res.modosit(k+1,j+1,C.at(k,j+1)*C.at(k+1,j+2)-temp);
            }
            else
            {
                res.modosit(k+1,j+1,temp);
                for(int l=1;l<i;l++)
                {
                    res.modosit(k+1,j+1,res.at(k+1,j+1)+C.at(k-l+1,j+1)*res.at(k+1,j+i-l+1));
                }
                res.modosit(k+1,j+1,res.at(k+1,j+1)*(-1));
            }
            k++;
            j++;
        }
        i++;
        j=0;
    }while(i!=n);

    res=T(res);//fels�h�romsz�g m�trix lesz
    
    for(int i=1;i<n+1;i++)//a fenti nrm�l�s inverz m�velete
    {
        for(int j=1;j<n+1;j++)
        {
            res.modosit(j,i,res.at(j,i)*D[i-1]);
        }
    }
    return res;
}

//dense matrix
std::vector<double> operator*(const std::vector<double> &v, double c)
{
    std::vector<double> res;
    res.resize(v.size());
    for(int i=0;i<res.size();i++)
    {
        res[i]=v[i]*c;
    }
    return res;
}

std::vector<std::vector<double> > operator*(const std::vector<std::vector<double> > &A, double c)
{
    std::vector<std::vector<double> > res;
    res.resize(A.size());
    for(int i=0;i<A.size();i++)
    {
        res[i].resize(A.size());
        for(int j=0;j<A.size();j++)
        {
            res[i][j]=A[i][j]*c;
        }
    }
    return res;
}

std::vector<double> operator+(const std::vector<double> &v, const std::vector<double> &u)
{
    std::vector<double> res;
    res.resize(v.size());
    for(int i=0;i<res.size();i++)
    {
        res[i]=v[i]+u[i];
    }
    return res;
}

std::vector<std::vector<double> > operator*(const std::vector<double> &v, const std::vector<double> &u)
{
    std::vector<std::vector<double> > res;
    res.resize(v.size());
    for(int i=0;i<res.size();i++) res[i].resize(v.size());

    for(int i=0;i<res.size();i++)
    {
        for(int j=0;j<res.size();j++)
        {
            res[i][j]=v[i]*u[j];
        }
    }
    return res;
}

std::vector<std::vector<double> > operator+(const std::vector<std::vector<double> > &A, const std::vector<std::vector<double> > &B)
{
    std::vector<std::vector<double> > res;
    res.resize(A.size());
    for(int i=0;i<res.size();i++)
    {
        res[i].resize(A.size());
        for(int j=0;j<res.size();j++)
        {
            res[i][j]=A[i][j]+B[i][j];
        }
    }
    return res;
}

std::vector<std::vector<double> > operator*(const std::vector<std::vector<double> > &A, const std::vector<std::vector<double> > &B)
{
    std::vector<std::vector<double> > res;
    res.resize(A.size());
    for(int i=0;i<A.size();i++) res[i].resize(A.size());

    for(int i=0;i<A.size();i++)
    {
        for(int j=0;j<B.size();j++)
        {
            res[i][j]=0;
            for(int k=0;k<A.size();k++)
            {
                res[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
    return res;
}

std::vector<std::vector<double> > T(const std::vector<std::vector<double> > &A)//transzpon�lt
{
    std::vector<std::vector<double> > res=A;
    for(int i=0;i<A.size();i++)
    {
        for(int j=0;j<A.size();j++)
        {
            double temp=A[i][j];
            res[i][j]=A[j][i];
            res[j][i]=temp;
        }
    }
    return res;
}

void javit(std::vector<std::vector<double> > &a)
{
    for(int i=0;i<a.size();i++)
        for(int j=0;j<a.size();j++)
            if(absolute(a[i][j])<thres) a[i][j]=0;
}

std::vector<std::vector<double> > inverseUpperTriangular(const std::vector<std::vector<double> > &U)
{
    int n=U.size();
    std::vector<std::vector<double> > res;
    res.resize(n);
    for(int i=0;i<n;i++)//egys�gm�trix
    {
        res[i].resize(n);
        for(int j=0;j<n;j++)
            if(i==j) res[i][j]=1;
            else res[i][j]=0;
    }
    std::vector<double> D(n,0);

    for(int i=0;i<n;i++)
    {
        double temp=U[i][i];
        if(temp!=0) D[i]=1/temp;
    }

    std::vector<std::vector<double> > C=U;

    for(int i=0;i<n;i++)//norm�lja a m�trixot �gy, hogy a f��tl�ban 1-esek legyenek
    {
        for(int j=0;j<n;j++)
        {
            C[i][j]*=D[i];
        }
    }
    
    C=T(C);//als�h�romsz�g m�trix lesz

    int i=1;
    int j=0;
    do
    {
        int k=i;
        while(k!=n)
        {
            if(i==1)//f��tl� alatti �tl�
            {
                res[k][j]=(-1)*C[k][j];
            }
            else if(i==2)
            {
                res[k][j]=C[k-1][j]*C[k][j+1]-C[k][j];
            }
            else
            {
                res[k][j]=C[k][j];
                for(int l=1;l<i;l++)
                {
                    res[k][j]+=C[k-l][j]*res[k][j+i-l];
                }
                res[k][j]*=-1;
            }
            k++;
            j++;
        }
        i++;
        j=0;
    }while(i!=n);

    res=T(res);//fels�h�romsz�g m�trix lesz
    
    for(int i=0;i<n;i++)//a fenti norm�l�s inverz m�velete
    {
        for(int j=0;j<n;j++)
        {
            res[j][i]*=D[i];
        }
    }

    return res;
}

std::vector<double> operator*(const std::vector<std::vector<double> > &A, const std::vector<double> &x)
{
    int n=x.size();
    std::vector<double> res(n,0);

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            res[i]+=A[i][j]*x[j];
        }
    }

    return res;
}

double vectorMultScalar(const std::vector<double> &v, const std::vector<double> &u)
{//skal�ris szorz�s
    double res=0;

    for(int i=0;i<v.size();i++)
    {
        res+=v[i]*u[i];
    }

    return res;
}

bool isUpperTriangular(const std::vector<std::vector<double> > &A)
{
    int n=A.size();
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<i;j++)
        {
            if(A[i][j]!=0) return false;
        }
    }
    return true;
}
//konverzi�k
sparse_matrix toSpare(const std::vector<std::vector<double> > &A)
{
    int n=A.size();
    sparse_matrix res(n);
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            double temp=A[i][j];
            if(temp!=0) res.befuz(new node(i+1,j+1,temp));
        }
    }
    return res;
}

std::vector<std::vector<double> > toDense(const sparse_matrix &A)
{
    std::vector<std::vector<double> > res;
    int n=A.getSize();
    res.resize(n);
    for(int i=0;i<n;i++)
    {
        res[i].resize(n);
        for(int j=0;j<n;j++)
        {
            res[i][j]=A.at(i+1,j+1);
        }
    }
    return res;
}
