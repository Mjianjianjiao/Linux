#include "date.h"

 int Date::GetMonthday (int year,int month)
{
    int days[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(month==2)
    {
      if((year%4==0&&year%100!=0)||year%100==0)
        return 29;
    }
    

    return days[month];

}

bool Date:: operator>(const Date& d)
{
    if(_year>d._year)
      return true;
    else if(_year==d._year)
    {
      if(_month>d._month)
        return true;
      else if(_month==d._month)
      {
        if(_day>d._day)
        return true;
        else false;
      }else return false;

    }else return false;


}
    bool Date::operator==(const Date& d)
{
      if(_year==d._year&&_month==d._month&&_day==d._day)
        return true ;
      return false;
}


    bool Date::operator!=(const Date& d)
    {
       if(!(*this==d))
         return true;
        return false;

    }
    

bool Date::operator<(const Date& d)
{
     if(_year<d._year)
       return true;
     else if(_year==d._year)
     {
        if(_month<d._month)
        {
          return true;
        }else if(_month==d._month)
        { 
          if(_day<d._day)
            return true;
          else return false;

        }else return false;
     }else return false;

}
    bool Date::operator>=(const Date& d)
{
      if(!(*this<d))
        return true;
      return false;

}
    bool Date::operator<=(const Date& d)
{
     if(!(*this>d))
       return true;
     return false;
}
 Date  Date::operator+(int day)
{
       Date ret(*this);
      ret+=day ;

       return ret;

}
    Date  Date::operator-(int day)
{
    Date ret(*this);
    ret-=day;

    return ret;
}
 Date&  Date::operator+=(int day)
{
  if(day<0)
  {
    *this-=(-day);
  }
    _day+=day;
    while(_day> GetMonthday (_year,_month))
    {
      _day-=GetMonthday(_year,_month);
      _month++;
       if(_month==13)
       {
          _year++;
          _month=1;   
       }
    }

    return *this;
}
Date& Date::operator-=(int day)
{
  if(day<0)
  {
    *this+=(-day);
  }

  _day-=day;

  while(_day<=0)
  {
    _month--;
    if(_month==0)
    {
      _year--;
      _month=12;
    }
    _day+=GetMonthday(_year,_month);
  }
  return *this;
}
Date& Date::operator++()
{

       *this+=1;
       return *this;
}
Date& Date::operator--()//编译器约定前置++不带参数，后置++带一个参数，什么都可以，只用于和前置区分
{
      *this-=1;
      return *this;
} 
  
  Date Date::operator++(int)
{
      Date ret(*this);
      *this+=1;

      return ret;
}
Date Date::operator--(int)
{
      Date ret(*this);
      *this-=1;

  
   return ret;
}

int Date::GetDays(const Date& d)
{
   Date max;
   Date min;
   int count=0;
   if(*this>d)
   {
      max=*this;
      min=d;
   }else if(*this<d)
   {
     min=*this;
     max=d;
   }else return count;
cout<<1<<endl;
   while(max!=min)
   {
     count++;
     min++;
   } 
  cout<<count<<endl;
   return count;
}

void Date::Print(){

  cout<<_year<<"-"<<_month<<"-"<<_day<<endl;

}








