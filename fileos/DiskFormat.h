#ifndef DISKFORMAT_H
#define DISKFORMAT_H
struct Sys;
struct DiskSpace;
struct Inode;
#include <list>
using namespace std;
class DiskFormat{
public:
  static int DiskSize;
  static int BloclSize;
  static int DiskNum;
  static int Block[2048];
  static char root_name;
  static int root_addr;
  DiskFormat();
  bool startDiskFormat(struct Sys *root,struct DiskSpace *diskspace,list<Inode*> *inodes);
  bool initDisk(struct Sys*root,struct DiskSpace *diskspace,list<Inode*> *inodes);
};

#endif
