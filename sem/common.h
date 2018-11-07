#ifndef __common_h__
#define __common_h__

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#define PATHNAME "."
#define PROJ_ID 0X6666 //通过ftok获取键值时的参数
using namespace std;
typedef union semun{//semctl 的第四个参数的设置
  int val; /*for SETVAL 设置信号量级的计数值 */
  struct semid_ds *buf;//IPC_STAT AND IPC_SET
  unsigned short *array;//for GETALL and SETALL
}Semnum;
int creatSemset(int nums);
int getSemSet(int nums);
int initSem(int semid,int nums,int initval);
int P(int semid,int who);
int V(int semid,int who);
int destorySem(int semid);

#endif
