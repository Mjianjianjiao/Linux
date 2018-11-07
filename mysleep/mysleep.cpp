#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

void sig_alarm(int singno)//捕捉信号后的自定义函数
{};
void mysleep(int sec)

{
    struct sigaction act,oact;
    act.sa_handler=sig_alarm;
    sigemptyset(&act.sa_mask);//将该进程的信号屏蔽字清空
    sigaction(SIGALRM,&act,&oact);//捕捉信号
    alarm(sec);
    pause();//收到信号后就将其挂起
    alarm(0);//取消闹钟
    sigaction(SIGALRM,&oact,NULL);//恢复原先的默认动作
}


int main()
{
  while(1){  
    mysleep(1);
    cout<<"sleeping...."<<endl;
 }
  return 0;
  
}
