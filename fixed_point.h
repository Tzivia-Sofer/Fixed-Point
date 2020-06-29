#include "my_exception.h"
#include <iostream>
#include <limits>
#include <cmath>
#include <cstring>
#include <sstream>

template <unsigned num,typename T =int, typename S = short> class Price;
template <unsigned num, typename T,typename S> std::ostream&  operator << (std::ostream& ,const Price<num,T,S>&);

template <unsigned num, typename T, typename S>
class Price{
public:
    Price();
    Price(T, S);
    Price(T);

    operator double();
    Price& operator++();
    Price operator++(int);
    Price& operator--();
    Price operator--( int);
    Price& operator+=(const Price&);
    Price& operator-=(const Price&);
    friend std::ostream&  operator << <>(std::ostream& ,const Price<num,T,S>&);
    Price& operator =(T);

    T getDollars()const;
    S getCents()const;
    void setSign(bool);
    bool getPositive()const;
    std::string asString();

    static const int cents_in_dollar = 100;
    static const T max_num = std::numeric_limits<T>::max();

private:
    T m_dollar ;
    S m_cent;
    bool m_positive;

};

template <unsigned num,typename T, typename S>
std::string Price<num,T,S>::asString()
{
    std::string s;
    std::stringstream out;
    double d = double(*this);
    out << d;
    s = out.str();
    std::cout<<s<<std::endl;
    return s;
}

template <unsigned num,typename T, typename S>
void Price<num,T,S>::setSign(bool state){this->m_positive=state;}

template <unsigned num,typename T, typename S>
S Price<num,T,S>::getCents() const{return this->m_cent;}

template <unsigned num,typename T, typename S>
T Price<num,T,S>::getDollars() const{ return this->m_dollar;}

template <unsigned num,typename T, typename S>
bool Price<num,T,S>::getPositive() const{return this->m_positive;}

template <unsigned num,typename T, typename S>
Price<num,T,S>::operator double()
{
    const double result = double(m_dollar) + double(m_cent)/pow(10,num);
    return m_positive? result:-result;
}

template <unsigned num,typename T, typename S>
Price<num,T,S>::Price():m_dollar(0),m_cent(0),m_positive(true){}

template <unsigned num,typename T, typename S>
Price<num,T,S>::Price(T num_to_insert):m_dollar(num_to_insert),m_cent(0),m_positive(true)
{
    if (num < 0)
        m_positive = false;
}

template <unsigned num,typename T, typename S>
Price<num,T,S>::Price(T dollar, S cent):m_dollar(dollar),m_cent(cent)
{
    if (m_cent<0)
    {
        m_cent = -m_cent;
        m_positive = false;
    }
    if (m_dollar<0)
    {
        m_dollar = -m_dollar;
        m_positive = false;
    } else
    {
        m_positive = true;
    }
}

template <unsigned num,typename T, typename S>
Price<num,T,S> operator -(const Price<num,T,S> & p)
{
    Price<num,T,S> result(p.getDollars(),p.getCents());
    result.setSign(!p.getPositive());
    return result;
}

template <unsigned num,typename T, typename S>
Price<num,T,S>& Price<num,T,S>::operator =(T num_to_insert)
{
    this->m_dollar = num_to_insert;
    this->m_cent = 0;
    return  *this;
}

template <unsigned num,typename T, typename S>
bool operator ==(const Price<num,T,S>& p1,const Price<num,T,S>& p2)
{
    return ((p1.getDollars()==p2.getDollars()) && (p1.getCents()==p2.getCents()));
}

template <unsigned num,typename T, typename S>
bool operator !=(const Price<num,T,S>& p1,const Price<num,T,S>& p2)
{
    return (!(p1==p2));
}

template <unsigned num,typename T, typename S>
bool operator <(const Price<num,T,S>& p1,const Price<num,T,S>& p2)
{
    if(!p1.getPositive())
    {
        if(!p2.getPositive())
        {
            if(p1.getDollars()==p2.getDollars())
                return (p1.getCents()>p2.getCents());
            return (p1.getDollars()>p2.getDollars());
        }
        else{
            return true;
        }
    }
    if(p1.getDollars()==p2.getDollars())
        return (p1.getCents()<p2.getCents());
    return (p1.getDollars()<p2.getDollars());
}

template <unsigned num,typename T, typename S>
bool operator >(const Price<num,T,S>& p1,const Price<num,T,S>& p2)
{
   return(!(p1 < p2));
}

template <unsigned num,typename T, typename S>
bool operator >=(const Price<num,T,S>& p1,const Price<num,T,S>& p2)
{
    return (p1 > p2 || p1 == p2);
}

template <unsigned num,typename T, typename S>
bool operator <=(const Price<num,T,S>& p1,const Price<num,T,S>& p2)
{
    return (p1 < p2 || p1 == p2);
}

template <typename S>
int countDigit(S n)
{
    if (n == 0)
        return 0;
    return 1 + countDigit<S>(n / 10);
}

template <unsigned num,typename T, typename S>
std::ostream&  operator <<(std::ostream& os ,const Price<num,T,S> &p)
{
    if(!p.m_positive)
        os << "-";
    os << p.m_dollar << '.';
    if(p.getCents()==0)
    {
        for ( int i = 0; i< num-countDigit(p.getCents())-1; i++)
        {
            std::cout<<"0";
        }
    }
    else
    {
        for ( int i = 0; i< num-countDigit(p.getCents()); i++)
        {
            std::cout<<"0";
        }
    }
    os<< std::to_string(p.m_cent)<< " dollars " <<std::endl;
    return os;
}

template <unsigned num,typename T, typename S>
const Price<num,T,S> operator +(const Price<num,T,S>& p1, const Price<num,T,S>& p2)
{
    if(p1.getPositive()&& !p2.getPositive())
    {
        return (p1 - -p2);
    }
    if(!p1.getPositive()&& p2.getPositive())
    {
        return (p2 - -p1);
    }
    if (p1.getDollars() + p2.getDollars() + 2 > Price<num,T,S>::max_num) //overflow
               throw ErrorOverFlow("Addition Failed");
    T dollar = p1.getDollars()+p2.getDollars();
    T cent = p1.getCents()+p2.getCents();
    if(p1.getCents()+p2.getCents() >= Price<num,T,S>::cents_in_dollar)
    {
        cent-=100;
        dollar++;
    }
    Price<num,T,S> result(dollar,cent);
    result.setSign(p1.getPositive());
    return result;
}

template <unsigned num,typename T, typename S>
const Price<num,T,S> operator -(const Price<num,T,S>& p1, const Price<num,T,S>& p2)
{
    if (!p2.getPositive()){
        return (p1 + -p2);
    }
    if (!p1.getPositive()){
        return -(p1 + p2);
    }
    if (p2 > p1){
        return -(p2 - p1);
    }
    T dollar = p1.getDollars() - p2.getDollars();
    T cent = p1.getCents() - p2.getCents();
    if(cent<0)
    {
        cent+=100;
        dollar-=1;
    }
    Price<num,T,S> result(dollar, cent);
    return result;
}

template <unsigned num,typename T, typename S>
Price<num,T,S>& Price<num,T,S>::operator +=(const Price<num,T,S>& p2)
{
    *this = *this + p2;
    return *this;
}

template <unsigned num,typename T, typename S>
Price<num,T,S>& Price<num,T,S>::operator -=(const Price<num,T,S>& p2)
{
    *this = *this - p2;
    return *this;
}

template <unsigned num,typename T, typename S>
Price<num,T,S>& Price<num,T,S>::operator ++()
{
    *this+=1;
    return *this;
}

template <unsigned num,typename T, typename S>
Price<num,T,S> Price<num,T,S>::operator ++( int)
{
    Price tmp(*this);
    *this+=1;
    return tmp;
}

template <unsigned num,typename T, typename S>
Price<num,T,S>& Price<num,T,S>::operator --()
{
    *this-=1;
    return *this;
}

template <unsigned num,typename T, typename S>
Price<num,T,S> Price<num,T,S>::operator --(int)
{
    Price tmp(*this);
    *this-=1;
    return tmp;
}

template <typename T,unsigned num,typename S>
Price<num,T,S> operator *(const Price<num,T,S>& p1,const Price<num,T,S>& p2)//todo
{
    double d1 = double(p1);
    double d2 = double(p2);
    T dollar = int(d1*d2);
    S cent = (d2*d1-dollar)*pow(10,num);
    Price<num,T,S> result(dollar,cent);
    if(p1.getPositive()!=p2.getPositive())
    {
        result.setSign(false);
    }
    return result;
}

