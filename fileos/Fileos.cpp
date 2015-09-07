#include "Fileos.h"
#include "Command.h"
#include <iostream>
#include "Fileoperator.h"
#include "User.h"
#include <fstream>
#include "Node.h"
#include "DiskFormat.h"
using namespace std;
Fileos::Fileos()
{
  cout<<"-start the File System OK"<<endl;
  this->fop = new Fileoperator(this);
  this->cp = new Command(fop);
  this->fop->diskFormat();
  this->addRootToDir();
  cout<<"-start getting the Users list."<<endl;
  this->fop->getUsers(&ls_user,*ls_inodes->begin());
  user = NULL;
}
void Fileos::addRootToDir(){
  Sys r = *root;
  Dir* d = (convertToDir(r));
  this->pushDir(d);
}
void Fileos::startGetCmd()
{
  string cmd;
  while (true)
 {
   cout<<"zoson@"<<getStrDir()<<":$";
   getline(cin,cmd);
   cp->parser(cmd);
 }
}
void Fileos::addUser(User *user){
  ls_user.push_back(user);
}
unsigned int Fileos::getUserQuantity(){
  return ls_user.size();
}
void Fileos::setRoot(Sys *root){
  this->root = root;
}
struct Sys* Fileos::getRoot(){
  return root;
}
list<User*> Fileos::getUsers(){
  return ls_user;
}
void Fileos::pushDir(Dir *dir){
  stack_dir.push(dir);
}
Dir* Fileos::getTopStackOfDir(){
  Dir *dir = (stack_dir.top());
  return dir;
}
void Fileos::popDir(){
  stack_dir.pop();
}
Dir* Fileos::convertToDir(Sys sys){
  Dir *dir = new Dir;
  dir->uid = -1;
  dir->pid = -1;
  for(int i=0;i<50;i++){
    if (i==0) {
      dir->name[i] = sys.dir;
    }else if(i<16){
      dir->name[i] = '\0';
    }
  }
  return dir;
}
Dir* Fileos::convertToDir(User user){
  Dir *dir = new Dir;
  dir->uid = user.getUid();
  dir->id = user.getUid();
  dir->pid = 0;
  char* account = user.getCharAccount();
  for(int i = 0;i<16;i++){
    dir->name[i] = account[i];
  }
  for(int i=0;i<50;i++){
    if(user.getCid()[i]>0){
      dir->num+=1;
    }
  }
  return dir;
}
stack<Dir*> Fileos::getStackOfDir(){
  return stack_dir;
}
string Fileos::getStrDir(){
  stack<Dir*> s = stack_dir;
  string str;
  while(!s.empty()){
    Dir *dir = s.top();
    if(dir->name[0]=='/'){
      str = "/root" +str;
    }else{
      str =  '/'+string(dir->name)+str;
    }
    s.pop();
  }
  return str;
}
void Fileos::setUser(User *user){
  //User *u = new User(user->getAccount(),user->getPassword(),user->getUid(),user->getCid());
  //u.printf();
  this->user = user;
}
void Fileos::removeUser(){
  this->user = NULL;
}
User* Fileos::getUser(){
  return user;
}
bool Fileos::isLogin(){
  if(user == NULL){
    return false;
  }else{
    return true;
  }
}
void Fileos::addFile(File *file){
  ls_files.push_back(file);
}
void Fileos::addDir(Dir *dir){
  ls_dirs.push_back(dir);
}
list<File*> Fileos::getFiles(){
  return ls_files;
}
list<Dir*> Fileos::getDirs(){
  return ls_dirs;
}
Dir* Fileos::isExitedDirInList(unsigned int id,unsigned int uid,unsigned int pid){
  list<Dir*>::iterator it = ls_dirs.begin();
  for(;it!=ls_dirs.end();it++){
    Dir *dir = *it;
    if(dir->id == id && dir->pid == pid && dir->uid == uid){
      return dir;
    }
  }
  return NULL;
}
Dir* Fileos::isExitedDirInList(string name,unsigned int uid,unsigned int pid){
  list<Dir*>::iterator it = ls_dirs.begin();
  for(;it!=ls_dirs.end();it++){
    Dir *dir = *it;
    string n(dir->name);
    if(n == name && dir->pid == pid &&dir->uid == uid){
      return dir;
    }
  }
  return NULL;
}
File* Fileos::isExitedFileInList(unsigned int id,unsigned int uid,unsigned int pid){
  list<File*>::iterator it = ls_files.begin();
  for(;it!=ls_files.end();it++){
    File *file = *it;
    if(file->id == id && file->pid == pid && file->uid == uid){
      return file;
    }
  }
  return NULL;
}

void Fileos::setDiskSpace(DiskSpace *disk){
  this->diskspace = disk;
}

void Fileos::setInodes(list<Inode*> *ls_inode){
  this->ls_inodes = ls_inode;
}

list<Inode*>* Fileos::getInodes(){
  return ls_inodes;
}

DiskSpace* Fileos::getDiskSpace(){
  return this->diskspace;
}

bool Fileos::removeDirFromList(unsigned int id){
  bool remove =false;
  list<Dir*>::iterator i = ls_dirs.begin();
  for(;i!=ls_dirs.end();i++){
    if((*i)->id == id){
      // cout<<"dir name remove "<<(*i)->name<<endl;
      // ls_dirs.erase(i);
      remove = true;
      (*i)->id = 0;
      (*i)->pid = 0;
      (*i)->uid = 0;
      for(int j=0;j<15;j++){
        (*i)->name[j] = '\0';
      }
    }
  }
  return remove;
}

bool Fileos::removeFileFromList(unsigned int id){
  bool remove =false;
  list<File*>::iterator i = ls_files.begin();
  for(;i!=ls_files.end();i++){
    if((*i)->id == id){
      // cout<<"removeFileFromList file name remove "<<(*i)->name<<endl;
      // ls_files.erase(i);
      remove = true;
      (*i)->id = 0;
      (*i)->pid = 0;
      (*i)->uid = 0;
      for(int j=0;j<15;j++){
        (*i)->name[j] = '\0';
      }
    }
  }
  return remove;
}

void Fileos::setFile(File *file){
  this->file = file;
}

File* Fileos::getFile(){
  return file;
}
