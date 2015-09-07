#ifndef USER_H
#define USER_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class User{
  //size 236
private:
  char account[16];
  char password[16];
  unsigned int uid;
  unsigned int cid[50];
public:
  User();
  User(string account,string password,unsigned int id);
  User(string account,string password,unsigned int id,unsigned int *c);
  void setAccount(string account);
  void setPassword(string password);
  void printf();
  string getAccount();
  string getPassword();
  char* getCharAccount();
  unsigned int* getCid();
  unsigned int getUid();
  void setCid(unsigned int *c);
  void setCidItem(unsigned int id,int i);
  static int size;
};

#endif
