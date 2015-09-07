#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H
#include <iostream>
#include <list>
class Fileos;
class User;
#include "Node.h"
using namespace std;
class Fileoperator{
private:
  Fileos *fos;
public:
  Fileoperator(Fileos* fos);
  void f_create_file(string name,bool rw,bool flag);
  void f_create_dir(string name);
  void f_delete(string type,string name);
  void f_close();
  void f_open(string);
  void f_read();
  void f_write();
  void openDir(string name);
  void login(string account,string password);
  void reg(string account,string password);
  void help();
  void disk();
  void inode();
  void quit();
  void getUsers(list<User*>*list,Inode *inode);
  void diskFormat();
  int checkUsers();
  bool checkAccountIsExited(string account);
  void changeInodeBlock();
  void changeUserInfo(User *user);
  void changeDirInfo(Dir *dir);
  void dir(bool);
  void back();
  int checkBlockUnuse();
  Inode* checkInodeUnuse();
  void setBlockUse(int i);
  Inode* getInodeById(unsigned int id);
  struct Dir* convertInodeToDir(Inode *inode);
  struct File* convertInodeToFile(Inode *inode);
  File* getFileById(unsigned int id);
  Dir* getDirById(unsigned int id);
  Dir* getDirByName(string);
  User* getUserByName(string name);
  File* getFileByName(string);
  void setBlockUnuse(int i);
  void removeDirItem(unsigned int id);
};
#endif
