#include "DiskFormat.h"
#include <fstream>
#include <iostream>
#include "Node.h"
#include <string.h>
#include <list>
using namespace std;
DiskFormat::DiskFormat(){
  cout<<"-start check disk"<<endl;
}
bool DiskFormat::startDiskFormat(struct Sys* root,struct DiskSpace *diskspace,list<Inode*> *inodes){
  char name[16] = {'f','o','s','.','f','s','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
  strcpy(root->name,name);
  root->dir = '/';
  root->diskSize = 1024*1024;
  root->blockNum = 2048;
  root->blockSize = 512;
  fstream fs;
  fs.open(root->name,ios::out|ios::binary);
  if(fs){
    fs.write((char*)root,sizeof(*root));
    cout<<"Format finished"<<endl;
  }else{
    cout<<"Format fail"<<endl;
    return false;
  }
  fs.close();

  for(int i=0;i<1024;i++){
    diskspace->disk[i] = false;
  }
  fs.open("disk.fs",ios::out|ios::binary);
  if(fs){
    fs.write((char*)diskspace,sizeof(DiskSpace));
  }else{
    cout<<"Format fail"<<endl;
    return false;
  }
  fs.close();
  fs.open("inode.fs",ios::out|ios::binary);
  if(fs){
    Inode *root = new Inode;
    root->id = 0;
    root->name[0] = '/';
    root->type = false;
    root->rw = false;
    root->flag = false;
    root->use = true;
    for(int i = 0;i<10;i++){
      root->block[i] = -1;
    }
    for(int i =1;i<15;i++){
      root->name[i] = '\0';
    }
    fs.write((char*)root,sizeof(Inode));
    inodes->push_back(root);
    for(int i=1;i<1024;i++){
      Inode *inode = new Inode;
      inode->id = i;
      inode->use = false;
      for(int i=0;i<15;i++){
        inode->name[i] = '\0';
      }
      for(int i=0;i<10;i++){
        root->block[i] = -1;
      }
      fs.write((char*)inode,sizeof(Inode));
      inodes->push_back(inode);
    }

  }else{
    cout<<"Format fail"<<endl;
    return false;
  }
  fs.close();
  return true;
}

bool DiskFormat::initDisk(struct Sys* root,struct DiskSpace *diskspace,list<Inode*> *inodes){
  fstream fs;
  fs.open("fos.fs",ios::in|ios::binary);
  if (fs) {
    fs.read((char*)root,sizeof(*root));
    cout<<"-init disk ok"<<endl;
  }else{
    fs.close();
    cout<<"You dont has any account."<<endl;
    startDiskFormat(root,diskspace,inodes);
    return false;
  }
  fs.close();

  fs.open("disk.fs",ios::in|ios::binary);
  if(fs){
    fs.read((char*)diskspace,sizeof(*diskspace));
  }else{
    return false;
  }
  fs.close();

  fs.open("inode.fs",ios::in|ios::binary);
  if(fs){
    for(int i=0;i<1024;i++){
      Inode *inode = new Inode;
      fs.read((char*)(inode),sizeof(Inode));
      inodes->push_back(inode);
    }
  }else{
    return false;
  }
  fs.close();
  return true;
}

// int DiskFormat::DiskSize = 1024*1024;
// int DiskFormat::BloclSize = 512;
// int DiskFormat::DiskNum = 2028;
// char DiskFormat::root_name = '/';
// int DiskFormat::root_addr = 1;
