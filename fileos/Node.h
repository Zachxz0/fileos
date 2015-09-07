#ifndef NODE_H
#define NODE_H
// //56 byte
// struct Node{
//   unsigned int uid;
//   unsigned int nid;
//   unsigned int addr;
//   unsigned int size;// the file size
//   unsigned int index[10];//i node has 10 disk block
// };
struct File{
  unsigned int uid;// user id
  unsigned int pid;//dir id
  unsigned int id;
  bool rw;//read or write
  bool flag;//true is protected false is not
  char name[15];
};


//8b
struct DirItem{
  unsigned int id;
  unsigned int addr;
};
struct Dir{
  unsigned int uid;//user id
  unsigned int pid;//parent dir id
  unsigned int id;//itself id
  char name[15];//dir name
  int num;
  DirItem diritem[1280];
};
//system info
struct Sys{
  char name[16];
  char dir;
  unsigned int diskSize;
  unsigned int blockSize;
  unsigned int blockNum;
};
struct DiskSpace{
  bool disk[1024];
};
//64b
struct Inode{
  unsigned int id;
  char name[15];
  bool use;
  bool type;//false is dir ,true is file
  bool rw;//read or write
  bool flag;//true is protected false is not
  int block[10];
};
/*
inode num 1024
disksize = 1024*(1024+64+1+1)
blockSize = 1024;
blocknum = 1024;
inode num =1024
inode size =64;
inode has 64 block
sys has 1 block
blocktable has
*/

#endif
