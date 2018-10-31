#ifndef __DATE_H__
#define __DATE_H__

#include <iostream>

using namespace std;
class Date {

  public:
    Date(int year=1900,int month=1,int day=1)
      :_year(year)
      ,_month(month)
      ,_day(day)
  {}
    bool operator>(const Date& d);
    bool operator==(const Date& d);
    bool operator!=(const Date& d);
    bool operator<(const Date& d);
    bool operator>=(const Date& d);
    bool operator<=(const Date& d);
    Date  operator+(int day);
    Date  operator-(int day);
    Date& operator+=(int day);
    
    Date& operator-=(int day);
  Date& operator++();//前置
   Date& operator--();//前置
    Date operator++(int);
    Date operator--(int);
    int GetDays(const Date& d);
    void Print();
  inline int GetMonthday(int yera,int month);
 private:
    int _year;
    int _month;
    int _day;
} ;


#endif
