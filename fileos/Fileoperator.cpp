#include "Fileoperator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Fileos.h"
#include "User.h"
#include "Node.h"
#include "DiskFormat.h"
#include <list>
#include <stack>
#include <stdio.h>

using namespace std;
Fileoperator::Fileoperator(Fileos *fos):fos(fos){
  cout<<"-start Fileoperator OK"<<endl;
}
void Fileoperator::f_create_file(string name ,bool rw ,bool flag){
  if(!fos->isLogin()){
    cout<<"-You don't login ,can execute this command."<<endl;
    return;
  }
  File* f = getFileByName(name);
  if(f!=NULL){
    cout<<"-the file name "<<name<< "is already existed,create file failed"<<endl;
    return;
  }
  Dir *dir = fos->getTopStackOfDir();
  User *user = fos->getUser();
  File *file = new File;
  Inode* inode = checkInodeUnuse();
  if(dir->id == user->getUid()){
    for(int i=0;i<50;i++){
      if(user->getCid()[i] == 0){
        user->setCidItem(inode->id,i);
        break;
      }
    }
    changeUserInfo(user);
    dir->num++;
  }else{
    Inode *dirnode = getInodeById(dir->id);
    int num = dir->num;
    int block = num/128;
    if(dirnode->block[block]==-1){
      dirnode->block[block] = checkBlockUnuse();
      changeInodeBlock();
      setBlockUse(dirnode->block[block]);
    }
    fstream fs;
    stringstream ss;
    ss<<dirnode->block[block];
    string str = ss.str()+".fs";
    fs.open(str.data(),ios::out|ios::binary|ios::app);
    if(fs){
      DirItem *diritem = new DirItem;
      diritem->id = dirnode->id;
      diritem->addr = inode->id;
      fs.write((char*)diritem,sizeof(*diritem));
      dir->diritem[num] = *diritem;
      dir->num++;
    }else{
      cout<<"file access error"<<endl;
    }
  }
  file->id = inode->id;
  file->uid = user->getUid();
  file->pid = dir->id;
  file->rw = rw;
  inode->rw = rw;
  inode->flag = flag;
  file->flag = flag;
  inode->type = true;
  inode->use = true;
  inode->block[0] = checkBlockUnuse();
  setBlockUse(inode->block[0]);

  stringstream ss;
  ss<<inode->block[0];
  string str = ss.str() +".fs";
  fstream fs;
  fs.open(str.data(),ios::out|ios::binary);
  fs.close();

  const char *n = name.data();
  int i=name.size();
  for(int j =0;j<15;j++){
    if(j>i){
      file->name[j] ='\0';
      inode->name[j] = '\0';
    }else{
      file->name[j] = n[j];
      inode->name[j] = n[j];
    }
  }
  changeInodeBlock();
  fos->addFile(file);
  cout<<"   -File  Name:"<<file->name<<" uid "<<file->uid<<"  pid "<<file->uid<<endl;
}
void Fileoperator::f_create_dir(string name){
  if(!fos->isLogin()){
    cout<<"-You don't login ,can execute this command."<<endl;
    return;
  }
  Dir* d = getDirByName(name);
  if (d!=NULL) {
    cout<<"-the dir name "<<name<< "is already existed,create dir failed"<<endl;
    return;
  }
  Dir *dir = fos->getTopStackOfDir();
  User *user = fos->getUser();
  Dir *c_dir = new Dir;
  Inode* inode = checkInodeUnuse();
  if(dir->id == user->getUid()){
    for(int i=0;i<50;i++){
      if(user->getCid()[i] == 0){
        user->setCidItem(inode->id,i);
        break;
      }
    }
    changeUserInfo(user);
    dir->num++;
  }else{
    Inode *dirnode = getInodeById(dir->id);
    int num = dir->num;
    int block = num/128;
    if(dirnode->block[block]==-1){
      dirnode->block[block] = checkBlockUnuse();
      changeInodeBlock();
      setBlockUse(dirnode->block[block]);
    }
    fstream fs;
    stringstream ss;
    ss<<dirnode->block[block];
    string str = ss.str()+".fs";
    fs.open(str.data(),ios::out|ios::binary|ios::app);
    if(fs){
      DirItem *diritem = new DirItem;
      diritem->id = dirnode->id;
      diritem->addr = inode->id;
      fs.write((char*)diritem,sizeof(*diritem));
      dir->diritem[num] = *diritem;
      dir->num++;
    }else{
      cout<<"file access error"<<endl;
    }
  }
  c_dir->id = inode->id;
  c_dir->uid = user->getUid();
  c_dir->pid = dir->id;
  inode->type = false;
  inode->use = true;
  inode->block[0] = checkBlockUnuse();
  setBlockUse(inode->block[0]);

  stringstream ss;
  ss<<inode->block[0];
  string str = ss.str() +".fs";
  fstream fs;
  fs.open(str.data(),ios::out|ios::binary);
  fs.close();

  const char *n = name.data();
  int i=name.size();
  for(int j =0;j<15;j++){
    if(j>i){
      c_dir->name[j] ='\0';
      inode->name[j] = '\0';
    }else{
      c_dir->name[j] = n[j];
      inode->name[j] = n[j];
    }
  }
  changeInodeBlock();
  fos->addDir(c_dir);
  cout<<"   -Dir  Name:"<<c_dir->name<<" uid "<<c_dir->uid<<"  pid "<<c_dir->pid<<" Did "<<c_dir->id<<endl;
}
void Fileoperator::f_delete(string type,string name){
  if(!fos->isLogin()){
    cout<<"-You don't login ,can execute this command."<<endl;
    return;
  }
  if(type == "file"){
    File *file = getFileByName(name);
    if(file != NULL){
      if(file->flag){
        cout<<"-The file you want to delete is protected ,you can't delete"<<endl;
        return;
      }
      Inode* inode = getInodeById(file->id);
      for(int i=0;i<15;i++){
        if(i<10){
          if(inode->block[i] >0){
            setBlockUnuse(inode->block[i]);
          }
          inode->block[i] =0;
        }
        inode->name[i] = '\0';
      }
      inode->use = false;
      inode->type = false;
      inode->flag = false;
      inode->rw = false;
      changeInodeBlock();
      fos->removeFileFromList(inode->id);
      Dir* current = fos->getTopStackOfDir();
      User* user = fos->getUser();
      if(current->id == user->getUid()){
        for(int i=0;i<50;i++){
          if(user->getCid()[i] == inode->id){
            user->getCid()[i] = 0;
            changeUserInfo(user);
            break;
          }

        }
      }else{
        removeDirItem(inode->id);
        changeDirInfo(current);
      }
      current->num--;
    }else{
      cout<<"-The current dir has no this file"<<endl;
    }
  }else if(type == "dir"){
    Dir *dir = getDirByName(name);
    if(dir !=NULL){
      dir->num = 0;
      for(int i=0;i<128;i++){
        if(dir->diritem[i].addr>0){
          dir->num++;
        }
      }
      if(dir->num != 0){
        cout<<"-The dir you want to delete fail,because it isnot empty dir"<<endl;
        return;
      }
      Inode* inode = getInodeById(dir->id);
      for(int i=0;i<15;i++){
        if(i<10){
          if(inode->block[i] >0){
            setBlockUnuse(inode->block[i]);
          }
          inode->block[i] =0;
        }
        inode->name[i] = '\0';
      }
      inode->use = false;
      inode->type = false;
      inode->flag = false;
      inode->rw = false;
      changeInodeBlock();
      fos->removeDirFromList(inode->id);
      Dir* current = fos->getTopStackOfDir();
      User* user = fos->getUser();
      if(current->id == user->getUid()){
        for(int i=0;i<50;i++){
          if(user->getCid()[i] == inode->id){
            user->getCid()[i] = 0;
            changeUserInfo(user);
            break;
          }
        }
      }else{
        removeDirItem(inode->id);
        changeDirInfo(current);
      }
      current->num--;
    }else{
      cout<<"-The current dir has no this dir"<<endl;
    }
  }else{
    cout<<"Delete type unknow"<<endl;
  }
}
void Fileoperator::f_close(){
  if(!fos->isLogin()){
    cout<<"-You don't login ,can execute this command."<<endl;
    return;
  }
  if(fos->getFile()!=NULL){
    fos->setFile(NULL);
    cout<<"-close successfully"<<endl;
  }else{
    cout<<"-you don't open any file"<<endl;
  }
}
void Fileoperator::f_open(string name){
  if(!fos->isLogin()){
    cout<<"-You don't login ,can execute this command."<<endl;
    return;
  }
  if(fos->getFile()!=NULL){
    cout<<"-you have already open other file ,please close"<<endl;
    return;
  }
  File* file = getFileByName(name);
  if(file!=NULL){
    cout<<" -File Attribute-"<<endl;
    cout<<"    -Name- "<<file->name<<endl;
    if(file->rw){
      cout<<"    -Privilege- readWrite"<<endl;
    }else{
      cout<<"    -Privilege- readOnly"<<endl;
    }
    if (file->flag) {
      cout<<"    -Protected- true"<<endl;
    }else{
      cout<<"    -Protected- false"<<endl;
    }
    int size = -1;
    int last = -1;
    int len;
    Inode *inode = getInodeById(file->id);
    for(int i=0;i<10;i++){
      if(inode->block[i]>0){
        size++;
        last = i;
      }
    }
    if(last != -1){
      stringstream ss;
      ss<<inode->block[last];
      string str = ss.str()+".fs";
      fstream fs;
      fs.open(str.data(),ios::in|ios::binary);
      if(fs){
        fs.seekg(0,ios::end);
        len = fs.tellg();
      }
      fs.close();
    }
    int realsize = size*1024+len;
    double real = (double)realsize/1024;
    if(realsize>1024){
      cout<<"    -Size- "<<realsize<<" Bytes"<<" or "<<real<<" KB"<<endl;
    }else{
      cout<<"    -Size- "<<realsize<<" Bytes"<<endl;
    }
    cout<<"    -status- opening"<<endl;
    fos->setFile(file);
  }else{
    cout<<"-the file is not exist in this direction"<<endl;
  }
}
void Fileoperator::f_read(){
  if(!fos->isLogin()){
    cout<<"-You don't login ,can execute this command."<<endl;
    return;
  }
  File *file = fos->getFile();
  if(file!=NULL){
    Inode *inode = getInodeById(file->id);
    bool isEmpty = true;
    for(int i=0;i<10;i++){
      if(inode->block[i]>0){
        stringstream ss;
        ss<<inode->block[i];
        string str = ss.str()+".fs";
        char content[1024] = {'\0'};
        fstream fs;
        fs.open(str.data(),ios::in|ios::binary);
        fs.seekg(0,ios::end);
        int len = fs.tellg();
        fs.seekg(0,ios::beg);
        if(fs){
          fs.read((char*)content,len);
          if(string(content) !="")isEmpty = false;
          cout<<content;
        }
        fs.close();
      }
    }
    cout<<endl;
    if(isEmpty){
      cout<<"-The file is empty"<<endl;
    }
  }else{
    cout<<"-The file is not exist"<<endl;
  }
}
void Fileoperator::f_write(){
  if(!fos->isLogin()){
    cout<<"-You don't login ,can execute this command."<<endl;
    return;
  }
  File* file = fos->getFile();
  if(file!=NULL){
    if(file->rw == false){
      cout<<"-The file is readOnly"<<endl;
    }else{
      Inode *inode = getInodeById(file->id);
      cout<<"write: ";
      char a[10240];
      cin.get(a,10240);
      string content(a);

      int size = content.size();
      int block = inode->block[0];
      int index =-1;
      for(int i=1;i<10;i++){
        if(inode->block[i]==0){
          block = inode->block[i-1];
          index = i;
          break;
        }
      }
      stringstream ss;
      ss<<block;
      string str = ss.str() + ".fs";
      fstream fs;
      fs.open(str.data(),ios::in|ios::binary);
      fs.seekp(0,ios::end);
      int len = fs.tellp();
      fs.close();
      int sub;
      if(len+size >1024){
        sub = 1024-len;
        fs.open(str.data(),ios::out|ios::binary|ios::app);
        fs<<content.substr(0,sub);
        fs.close();
        if(index!=-1){
          inode->block[index] = checkBlockUnuse();
          setBlockUse(inode->block[index]);
          stringstream ss;
          ss<<inode->block[index];
          changeInodeBlock();
          string str = ss.str()+".fs";
          fs.open(str.data(),ios::out|ios::binary);
          fs<<content.substr(sub+1,size-sub);
          fs.close();
        }else{
          cout<<"-The content is more than 1 mb"<<endl;
        }
      }else{
        fs.open(str.data(),ios::out|ios::binary|ios::app);
        fs>>noskipws;
        fs<<content;
        fs.close();
      }
    }
  }else{
    cout<<"-The file is not exist"<<endl;
  }
}
void Fileoperator::openDir(string name){
  if(!fos->isLogin()){
    cout<<"-You don't login ,can execute this command."<<endl;
    return;
  }
  Dir* p_dir = fos->getTopStackOfDir();
  Dir *c_dir = fos->isExitedDirInList(name,p_dir->uid,p_dir->id);
  if(c_dir != NULL){
    fos->pushDir(c_dir);
  }else{
    cout<<"Not this Dir"<<endl;
  }
  dir(false);
}
void Fileoperator::login(string account,string password){
  if(fos->isLogin()){
    cout<<"-You hava been logining,Or you want to logout,Please input back command"<<endl;
    return ;
  }

  list<User*> ls_user = fos->getUsers();
  list<User*>::iterator it = ls_user.begin();
  for(;it!=ls_user.end();it++){
    User *user = (User*)*it;
    if(account == user->getAccount()&&password == user->getPassword()){
      cout<<"-login successfully"<<endl;
      Dir* d = fos->convertToDir(*user);
      fos->pushDir(d);
      fos->setUser(user);
      User *u = fos->getUser();
      dir(false);
      return;
    }
  }
  cout<<"-login unsuccessfully , maybe the account or the password is wrong"<<endl;
}
void Fileoperator::back(){
  Dir *current = fos->getTopStackOfDir();
  if(current->name[0] == '/'){
    cout<<"-The current dir is root."<<endl;
  }else{
    if(current->pid == 0){
      fos->removeUser();
    }
    fos->popDir();
  }
}
void Fileoperator::dir(bool vis){
  //cout<<"dir"<<endl;
  //stack<Dir*> stack_dir = fos->getStackOfDir();
  Dir *current = fos->getTopStackOfDir();
  if(current->name[0] == '/'){
    cout<<"-User List"<<endl;
    list<User*> ls_user = fos->getUsers();
    if(ls_user.size()==0){
      cout<<"  -account is empty "<<endl;
    }
    list<User*>::iterator it = ls_user.begin();
    for(;it!=ls_user.end();it++){
      cout<<"  -Account: "<<((User*)(*it))->getAccount()<<"  Uid: "<<((User*)(*it))->getUid()<<endl;
    }
  }else{
    if(current->id == fos->getUser()->getUid()){
      User *user = fos->getUser();
      for(int i=0;i<50;i++){
        if(user->getCid()[i] > 0){
          Inode *inode = getInodeById(user->getCid()[i]);
          if(!inode->type){
            Dir *dir = getDirById(inode->id);
            if(dir ==NULL){
              dir = convertInodeToDir(inode);
            }
            fos->addDir(dir);
            if(vis){
              cout<<"  -Dir-   "<<"Name: "<<dir->name<<"  Did: "<<dir->id<<"  Uid: "<<dir->num<<"  Dir: "<<fos->getStrDir()<<endl;
            }
          }else{
            File *file = getFileById(inode->id);
            if(file == NULL){
              file = convertInodeToFile(inode);
            }
            fos->addFile(file);
            if(vis){
              cout<<"  -File- "<<" Name: "<<file->name<<"  Addr: "<<file->id<<"  Privilege: "<<file->rw<<"  flag: "<<file->flag<<"  Dir: "<<fos->getStrDir()<<endl;
            }
          }
        }
      }
    }else{
      Inode *inode = getInodeById(current->id);
      current->num = 0;
      for(int i=0;i<10;i++){
        if(inode->block[i]==0)continue;
        fstream fs;
        stringstream ss;
        ss<<inode->block[i];
        string str = ss.str()+".fs";
        fs.open(str.data(),ios::in|ios::binary);
        fs.seekg(0,ios::end);
        int len = fs.tellg();
        int num  = len/8;
        fs.seekg(0,ios::beg);
        if(fs){
          for(int j =0;j<num;j++){
            fs.read((char*)&(current->diritem[j+i*128]),sizeof(DirItem));
            if(current->diritem[j+i*128].addr == 0)continue;
            current->num = current->num+1;
            Inode* inode = getInodeById(current->diritem[j+i*128].addr);
            if(inode->type == true){
              File *file = getFileById(inode->id);
              if(file == NULL){
                file = convertInodeToFile(inode);
              }
              fos->addFile(file);
              if(vis){
                cout<<"  -File- "<<" Name: "<<file->name<<"  Addr: "<<file->id<<"  Privilege: "<<file->rw<<"  flag: "<<file->flag<<"  Dir: "<<fos->getStrDir()<<endl;
              }
            }else{
              Dir *dir = getDirById(inode->id);
              if(dir == NULL){
                dir = convertInodeToDir(inode);
              }
              if(vis){
                cout<<"  -Dir-   "<<"Name: "<<dir->name<<"  Did: "<<dir->id<<"  Uid: "<<dir->uid<<"  Dir: "<<fos->getStrDir()<<endl;
              }
              fos->addDir(dir);
            }
          }
        }else{
          cout<<"File error"<<endl;
        }
        fs.close();
      }
    }
    if(current->num == 0&&vis){
      cout<<"    -It is the empty dir"<<endl;
    }
  }
}
void Fileoperator::diskFormat(){
  DiskFormat df;
  Sys *root = new Sys;
  DiskSpace *diskspace = new DiskSpace;
  list<Inode*> *ls_inodes = new list<Inode*>;
  df.initDisk(root,diskspace,ls_inodes);
  fos->setRoot(root);
  fos->setDiskSpace(diskspace);
  fos->setInodes(ls_inodes);
}
void Fileoperator::reg(string account,string password){
  if (checkAccountIsExited(account)) {
    return;
  }
  int index = checkUsers();
  if (index != -1) {
    User *user = new User(account,password,fos->getUserQuantity()+1000);
    fstream fs;
    int block = (index)/4;
    Inode *root = *(fos->getInodes()->begin());

    if(root->block[block] == -1){
      root->block[block] = checkBlockUnuse();
      changeInodeBlock();
      setBlockUse(root->block[block]);
    }
    int addr = root->block[block];
    stringstream ss;
    ss<<addr;
    string saddr = ss.str()+".fs";
    fs.open(saddr.data(),ios::binary|ios::out|ios::app);
    if (fs) {
      fs.write((char*)user,sizeof(*user));
      fos->addUser(user);
      cout<<"-registe successfully.Now you can login"<<endl;
    }else{
      cout<<"-open file occur error"<<endl;
    }
    fs.close();
  }else{
    cout<<"-The number of user is more than 50"<<endl;
  }
}
void Fileoperator::getUsers(list<User*>*list,Inode *root){
  for(int i = 0;i<10;i++){
    if(root->block[i] >=0){
      fstream fs;
      stringstream ss;
      ss<<root->block[i];
      string addr = ss.str()+".fs";
      fs.open(addr.data(),ios::in|ios::binary);
      if (fs) {
        fs.seekg(0,ios::end);
        int size = fs.tellg();
        fs.seekg(0,ios::beg);
        for(int j =0;j<size/(sizeof(User));j++){
          User *user = new User();
          fs.read((char*)user,sizeof(*user));
          list->push_back(user);
        }
      }else{
        if(i == 0){
          return;
        }
      }
    }
  }
}
int Fileoperator::checkUsers(){
  int index = fos->getUsers().size();
  if(index>=40){
    return -1;
  }
  return index;
}
bool Fileoperator::checkAccountIsExited(string account){
  bool isExited = false;
  list<User*> ls_user = fos->getUsers();
  if (ls_user.size() == 0) {
    return isExited;
  }
  list<User*>::iterator it = ls_user.begin();
  for(;it!=ls_user.end();it++){
    User *user = *it;
    if(user->getAccount() == account){
      cout<<"-The account is existed"<<endl;
      isExited = true;
      break;
    }
  }
  return isExited;
}

void Fileoperator::changeInodeBlock(){
  list<Inode*> *inodes = fos->getInodes();
  fstream fs;
  fs.open("inode.fs",ios::out|ios::binary);
  if(fs){
    list<Inode*>::iterator it = inodes->begin();
    for(;it!=inodes->end();it++){
      fs.write((char*)(*it),sizeof(Inode));
    }
  }else{
    cout<<"-File access error"<<endl;
  }
  fs.close();
}
void Fileoperator::changeUserInfo(User *user){
  //cout<<user->getCharAccount()<<endl;
  //list<User*> ls_users = fos->getUsers();
  int id = user->getUid()-1000;
  int block = id/4;
  Inode* inode = *(fos->getInodes()->begin());
  int addr = inode->block[block];
  stringstream ss;
  ss<<addr;
  string str = ss.str()+".fs";
  int mod = id%4;
  list<User*> ls_users = fos->getUsers();
  list<User*>::iterator it = ls_users.begin();
  for(int i=0;i<block*4;i++){
    it++;
  }
  fstream fs;
  fs.open(str.data(),ios::out|ios::binary);
  if(fs){
    for(int i=0;i<4&&it!=ls_users.end();i++,it++){
      fs.write((char*)(*it),sizeof(User));
    }
  }else{
    cout<<"file access error"<<endl;
  }
  fs.close();
}
void Fileoperator::changeDirInfo(Dir *dir){
  Inode *inode = getInodeById(dir->id);
  int addr = inode->block[0];
  stringstream ss;
  string id;
  ss<<addr;
  id = ss.str()+".fs";
  fstream fs;
  fs.open(id.data(),ios::out|ios::binary|ios::trunc);
  if(fs){
    for(int i=0;i<128;i++){
      if(dir->diritem[i].id >0){
        fs.write((char*)&(dir->diritem[i]),sizeof(DirItem));
      }else{
        break;
      }
    }
  }else{
    cout<<"-File access error"<<endl;
  }
  fs.close();
}

int Fileoperator::checkBlockUnuse(){
  DiskSpace *disk = fos->getDiskSpace();
  for(int i=0;i<sizeof(DiskSpace);i++){
    if(disk->disk[i] == 0){
      return i;
    }
  }
}
void Fileoperator::setBlockUse(int i){
  DiskSpace *d = fos->getDiskSpace();
  d->disk[i] = true;
  fstream fs;
  fs.open("disk.fs",ios::out|ios::binary|ios::trunc);
  if(fs){
    fs.write((char*)d,sizeof(DiskSpace));
  }else{
    cout<<"File error"<<endl;
  }
  fs.close();
}

void Fileoperator::setBlockUnuse(int i){
  DiskSpace *d = fos->getDiskSpace();
  d->disk[i] = false;
  stringstream ss;
  ss<<i;
  string str = ss.str()+".fs";
  remove(str.data());
  fstream fs;
  fs.open("disk.fs",ios::out|ios::binary|ios::trunc);
  if(fs){
    fs.write((char*)d,sizeof(DiskSpace));
  }else{
    cout<<"File error"<<endl;
  }
  fs.close();
}

Inode* Fileoperator::checkInodeUnuse(){
  list<Inode*> *ls_nodes = fos->getInodes();
  list<Inode*>::iterator it = ls_nodes->begin();
  for(;it!=ls_nodes->end();it++){
    if((*it)->use == false){
      return *it;
    }
  }
}
Inode* Fileoperator::getInodeById(unsigned int id){
  list<Inode*> *ls_inodes = fos->getInodes();
  list<Inode*>::iterator it = ls_inodes->begin();
  for(;it!=ls_inodes->end();it++){
    if((*it)->id == id){
      return (*it);
    }
  }
}

struct Dir* Fileoperator::convertInodeToDir(Inode *inode){
  struct Dir *dir = new Dir;
  dir->uid = fos->getUser()->getUid();
  dir->pid = fos->getTopStackOfDir()->id;
  dir->num = 0;
  dir->id = inode->id;
  for(int j =0;j<15;j++){
    dir->name[j] = inode->name[j];
  }

  return dir;
}
struct File* Fileoperator::convertInodeToFile(Inode *inode){
  struct File *file = new File;
  file->id = inode->id;
  file->uid = fos->getUser()->getUid();
  file->pid = fos->getTopStackOfDir()->id;
  file->rw = inode->rw;
  file->flag = inode->flag;
  for(int j =0;j<15;j++){
    file->name[j] = inode->name[j];
  }
  return file;
}

void Fileoperator::disk(){
  DiskSpace *diskspace = fos->getDiskSpace();
  for(int i=0;i<sizeof(DiskSpace);i++){
    cout<<diskspace->disk[i]<<" ";
  }
  cout<<endl;
}
void Fileoperator::inode(){
  list<Inode*> *inode = fos->getInodes();
  list<Inode*>::iterator i = inode->begin();
  for(;i!=inode->end();i++){
    if((*i)->use == true){
      cout<<"inode "<<(*i)->id<<" is using "<<endl;
    }
  }
}
Dir* Fileoperator::getDirById(unsigned int id){
  list<Dir*> ls_dir = fos->getDirs();
  list<Dir*>::iterator i = ls_dir.begin();
  for(;i!=ls_dir.end();i++){
    if((*i)->id == id){
      return *i;
    }
  }
  return NULL;
}
File* Fileoperator::getFileById(unsigned int id){
  list<File*> ls_file = fos->getFiles();
  list<File*>::iterator i = ls_file.begin();
  for(;i!=ls_file.end();i++){
    if((*i)->id == id){
      return *i;
    }
  }
  return NULL;
}
File* Fileoperator::getFileByName(string name){
  Dir *dir = fos->getTopStackOfDir();
  int pid =  dir->id;
  list<File*> ls_file = fos->getFiles();
  list<File*>::iterator i = ls_file.begin();
  for(;i!=ls_file.end();i++){
    string file_name((*i)->name);
    if((*i)->pid == pid&& file_name == name){
      return *i;
    }
  }
  return NULL;
}
Dir* Fileoperator::getDirByName(string name){
  Dir *dir = fos->getTopStackOfDir();
  int pid =  dir->id;
  list<Dir*> ls_dir = fos->getDirs();
  list<Dir*>::iterator i = ls_dir.begin();
  for(;i!=ls_dir.end();i++){
    string dir_name((*i)->name);
    if((*i)->pid == pid&& dir_name == name){
      return *i;
    }
  }
  return NULL;
}

void Fileoperator::removeDirItem(unsigned int id){
  Dir *current_dir = fos->getTopStackOfDir();
  int index = -1;
  for(int i=0;i<128;i++){
    if(current_dir->diritem[i].addr == id){
      index = i;
    }
    if(index != -1&&i<127){
      current_dir->diritem[i]= current_dir->diritem[i+1];
    }
  }
}

User* Fileoperator::getUserByName(string name){
  list<User*> ls_user = fos->getUsers();
  list<User*>::iterator i = ls_user.begin();
  for(;i!=ls_user.end();i++){
    if(string((*i)->getAccount())==name){
      return (*i);
    }
  }
  return NULL;
}
