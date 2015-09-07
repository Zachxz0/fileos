#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stack>
#include <stdio.h>
using namespace std;
struct Sys{
  char name[16];
  char dir;
  unsigned int user[50];//the max user can register
  unsigned int diskSize;
  unsigned int blockSize;
  unsigned int blockNum;
};

struct DirItem{
  unsigned int id;
  unsigned int addr;
};
struct disk{
  bool a[1024];
};
int main()
{
  disk *d = new disk;
  disk *a = d;
  a=NULL;
  if(d!=NULL){
    cout<<"nt"<<endl;
  }

}
