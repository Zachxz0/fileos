#include "User.h"
#include <iostream>
#include "Node.h"
using namespace std;
User::User(){
}
User::User(string account,string password,unsigned int uid)
{
  setAccount(account);
  setPassword(password);
  this->uid = uid;
  for(int i =0;i<50;i++){
    cid[i] = 0;
  }
}
User::User(string account,string password,unsigned int uid,unsigned int *c)
{
  setAccount(account);
  setPassword(password);
  this->uid = uid;
  setCid(c);
}
void User::printf(){
  cout<<"Your account is "<<account<<" password: "<<password<<" uid: "<<uid<<endl;
  for(int i=0;i<50;i++){
    cout<<"cid "<<i<<"="<<cid[i]<<endl;
  }
}
void User::setAccount(string account){
  int size = account.size();
  for(int i=0;i<15;i++){
    if(i<size){
      this->account[i] = account.at(i);
    }else{
      this->account[i] = '\0';
    }
  }

}
void User::setPassword(string password){
  int size = password.size();
  for(int i=0;i<15;i++){
    if(i<size){
      this->password[i] = password.at(i);
    }else{
      this->password[i] = '\0';
    }
  }
}
string User::getAccount(){
  string a(account);
  return a;
}
string User::getPassword(){
  string a(password);
  return a;
}
char* User::getCharAccount(){
  return account;
}
unsigned int* User::getCid(){
  return cid;
}
unsigned int User::getUid(){
  return uid;
}
void User::setCid(unsigned int *c){
  for(int i=0;i<50;i++){
    cid[i] = c[i];
  }
}
void User::setCidItem(unsigned int id,int i){
  this->cid[i] = id;
}
int User::size = 36;
