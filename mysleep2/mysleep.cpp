#include <iostream>
#include <signal.h> 
#include <unistd.h>
using namespace std;


void sa_alarm(int singo)
{
  //什么也不做
}

void mysleep(int sec)
{

  struct sigaction act,oact;
  sigset_t newsigset,oldsigset;
  act.sa_handler=sa_alarm;//设置自定义信号处理函数
  sigemptyset(&act.sa_mask);//初始化信号屏蔽字
  sigaction(SIGALRM,&act,&oact);//捕获信号
  sigemptyset(&newsigset);
  sigaddset(&newsigset,SIGALRM);//将信号添加进去
  sigprocmask(SIG_BLOCK,&newsigset,&oldsigset);//屏蔽信号
  //设置闹钟
  alarm(sec);
  //挂起
  sigdelset(&oldsigset,SIGALRM);//将信号集中的该信号清零
  sigsuspend(&oldsigset);//解除信号屏蔽，并挂起
  //恢复原处理信号的函数
  sigaction(SIGALRM,&oact,NULL);
  //取消闹铃
  alarm(0);
}


int main ()
{
  while(1)
  {
   mysleep(1);
  cout<<"sleeping..."<<endl;
  }
  return 0;
}
