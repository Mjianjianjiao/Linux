#include "date.h"

int main()
{
  Date d1(2019,2,4);
  Date d2(2019,2,3);
 // (d1+=500).Print();
 // (d2-=500).Print();
// d3.Print();
//  Date d4=d1-100;
// d4.Print();
(++d2).Print();
(d2++).Print();
(--d1).Print();
(++d1).Print();
d1.GetDays(d2);
 return 0;
}
