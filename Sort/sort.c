#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
void Print(int* num,int n)
{
 assert(num);
  int i=0;
  for(i=0;i<n;i++)
  {
    printf("%d ",num[i] );
  }
}


void  InsertSort(int* num,int n)
{
  //插入排序 
  assert(num&&n>0);
  int i=0,j=0;
  for(i=0;i<n;i++)
  {
    int tmp=num[i];
    for(j=i;j>0;j--)
    {
      if(tmp<num[j-1])
        num[j]=num[j-1];
      else 
      {
        break;
      }
    }
    num[j]=tmp;
  }
}

void Shellsort(int* num ,int n)
{
  assert(num&&n>0);
  int k=5,i=0,j=0;
  while(k>0)
  {
      
    for(i=0;i<n;i+=k)
    {
      int tmp=num[i];
      for(j=i;j>0;j-=k)
      {
          if(tmp<num[j-k])
            num[j]=num[j-k];
          else 
            break;
      }
      num[j]=tmp;
    }
     k-=2;
  }
}

void SelectSort(int* num ,int n)
{
    assert(num&&n>0);
    int min=0;
    int i=0;
    int j=0;
    for(i=0;i<n-1;i++)
    {
      for(j=i;j<n;j++)
      {
        if(num[min]>num[j])
          min=j;
      }         
        int tmp=num[i];
        num[i]=num[min];
        num[min]=tmp;
    }
}

void BubbleSort(int * num,int n)
{
  assert(num&&n>0);
  int i=0,j=0;
  int flag=0;
  for(i=0;i<n-1;i++)
  { 
    
  
    flag=0;
    for(j=0;j<n-i-1;j++)
    {
      if(num[j+1]<num[j])
      {
          flag=1;
        int tmp=num[j];
        num[j]=num[j+1];
        num[j+1]=tmp;
      }
    }
    if(flag==0)
      break;
  }
}

void CountSort(int* num,int n)
{
  assert(num&&n>0);
  int max=0,min=0,i=0,j=0;
  for(i=0;i<n;i++)
  {
    if(max<num[i])
      max=num[i];
    if(min>num[i])
      min=num[i];
  }
  int len=max-min+1;
  int* arr=(int*)calloc(sizeof(int),len);
  for(i=0;i<n;i++)
    arr[num[i]-min]++;
  int index=0;
  for(i=0;i<len;i++)
  {
       while(arr[i])
       {
         num[index++]=i+min;
         arr[i]--;
      }
  }
  free(arr);
}

void Onesort(int* num,int left ,int right)
{
  assert(num);
  int start=left;
  int end=right;
  int key=num[left];
    if(left>=right)
    return ;
  while(left<right)
  {
    while(left<right&&num[right]>=key)
       right--;
    num[left]=num[right];
    while(left<right&&num[left]<=key)
      left++;
    num[right]=num[left];
  }
  num[right]=key; 
  Onesort(num,start,left);
  Onesort(num,right+1,end);
}

void QuickSort(int* num,int n)
{
    Onesort(num,0,n-1);
}



void swap(int* a,int* b)
{
  int tmp=0;
  tmp=*a;
  *a=*b;
  *b=tmp;
}

void Heapjust(int* num,int n,int i)
{
 int  parent=i;
 int  child=parent*2+1;
 int tmp=0;
 while(parent<n)
  {
     child=parent*2+1;
     if(child+1<n&&num[child]<num[child+1])
     {
       child++;
     }
     if(num[parent]<num[child])
     swap(&num[parent],&num[child]);
     else break;
    parent=child;
}

}

void Heapsort(int *num,int n)
{
  //初始化为大堆
  int i=0;
  for(i=n/2-1;i>=0;i--)
    Heapjust(num,n,i);


  i=n-1;
  while(i>=0)
  {  
    swap(&num[0],&num[i]);
      i--;
     Heapjust(num,i,0);
  }

  printf("%d\n",n);
}

int main()
{
  int num[]={2,54,0,6,1,3,9,0};
  int n=sizeof(num)/sizeof(num[0]);

//  InsertSort(num,n);
//  Shellsort(num,n);
//  SelectSort(num,n);
//  BubbleSort(num,n);
//  CountSort(num,n);
//  QuickSort(num,n);
    Heapsort(num,n);
    printf("%d",n);
    Print(num,n);
};


