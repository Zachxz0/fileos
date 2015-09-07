#include "Command.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include "Fileoperator.h"
using namespace std;
Command::Command(Fileoperator *fo):fo(fo){
  cout<<"-start CommandParser OK"<<endl;
}
Command Command::parser(string cmd){
  splitCommand(cmd,' ');
  parserCommand((string)*list_cmd.begin());
  return *this;
}
void Command::parserCommand(string cmd){
  if (cmd == "help")
  {
    help();
  }else if (cmd == "quit"){
    quit();
  }else if(cmd == "create"){
    f_create();
  }else if(cmd == "delete"){
    f_delete();
  }else if(cmd == "close"){
    close();
  }else if(cmd == "read"){
    f_read();
  }else if(cmd == "write"){
    f_write();
  }else if(cmd == "login"){
    login();
  }else if(cmd == "reg"){
    reg();
  }else if(cmd == "df"){
    diskFormat();
  }else if(cmd == "dir"){
    dir();
  }else if(cmd == "back"){
    back();
  }else if(cmd == "opendir"){
    openDir();
  }else if(cmd == "disk"){
    disk();
  }else if(cmd == "inode"){
    inode();
  }else if(cmd == ""){
    cout<<endl;
  }else if(cmd == "open"){
    open();
  }else{
    cout<<"<!------Wrong command------!>"<<endl;
  }
}
void Command::back(){
  fo->back();
}
void Command::dir(){
  fo->dir(true);
}
void Command::f_create(){
  //cout<<"create dir name"<<endl;
  //cout<<"create file name rw flag"<<endl;
  list_cmd.pop_front();
  list<string>::iterator i = list_cmd.begin();
  if(list_cmd.size()>0){
    string type = (string)*i;
    if(type == "file"){
      string name = (string)(*(++i));
      string str_rw = (string)(*(++i));
      bool rw;
      if(str_rw=="r"){
        rw = false;
      }else if(str_rw =="rw"){
        rw = true;
      }else{
        cout<<"<!------create command should like create file zoson r(rw) p(np)------!>"<<endl;
        return;
      }
      string str_flag = (string)(*(++i));
      bool flag;
      if(str_flag == "p"){
        flag = true;
      }else if(str_flag == "np"){
        flag = false;
      }else{
        cout<<"<!------create command should like create file zoson r(rw) p(np)------!>"<<endl;
        return;
      }
      fo->f_create_file(name,rw,flag);
    }else if(type == "dir"){
      if(list_cmd.size()==2){
        string name = (string)(*(++i));
        fo->f_create_dir(name);
      }else{
        cout<<"<!------create command should like create dir zoson ------!>"<<endl;
      }
    }else{
      cout<<"<!------create command should like create file zoson r(rw) p(np)------!>"<<endl;
      cout<<"<!------create command should like create dir zoson ------!>"<<endl;
    }
  }
}
void Command::openDir(){
  list_cmd.pop_front();
  list<string>::iterator i = list_cmd.begin();
  if(list_cmd.size()==1){
    string name = (string)*i;
    fo->openDir(name);
  }else{
    cout<<"<!------opendir command should like opendir doc------!>"<<endl;
  }
}
void Command::f_delete(){
  list_cmd.pop_front();
  list<string>::iterator i = list_cmd.begin();
  if(list_cmd.size() == 2){
    string type = (string)*i;
    ++i;
    string name = (string)*i;
    fo->f_delete(type,name);
  }else{
    cout<<"<!------delete command should like delete type(file/dir) name------!>"<<endl;
  }
}
void Command::f_read(){
  fo->f_read();
}
void Command::f_write(){
  fo->f_write();
}
void Command::quit(){
  exit(0);
}
void Command::login(){
  list_cmd.pop_front();
  list<string>::iterator i = list_cmd.begin();
  if(list_cmd.size()==2){
    string account = (string)*i;
    ++i;
    string password = (string)*i;
    fo->login(account,password);
  }else{
    cout<<"<!------login command should like login zoson 123------!>"<<endl;
  }
}
void Command::reg(){
  list_cmd.pop_front();
  list<string>::iterator i = list_cmd.begin();
  if(list_cmd.size() == 2){
    string account = (string)*i;
    ++i;
    string password = (string)*i;
    fo->reg(account,password);
  }else{
    cout<<"<!------register command should like reg zoson 123------!>"<<endl;
  }
}
void Command::diskFormat(){
  fo->diskFormat();
}
void Command::help(){
  cout<<"-login    --input  your account and password to log in the file system"<<endl;
  cout<<"-reg      --reg account ,so you can login"<<endl;
  cout<<"-create   --create file or dir in any direction with name and privilege"<<endl;
  cout<<"-dir      --list the current  direction of item"<<endl;
  cout<<"-delete   --appoint the item you want to delete from the file system"<<endl;
  cout<<"-open     --open the file so you can read or write it"<<endl;
  cout<<"-close    --when don't use the file any longer,close it and release the memory"<<endl;
  cout<<"-opendir  --open the dir ,you can go into the direction"<<endl;
  cout<<"-back     --back to the parent dir"<<endl;
  cout<<"-disk     --get the disk block using status"<<endl;
  cout<<"-inode    --get the inode using status"<<endl;
  cout<<"-read     --after opening the file ,you can read the content in the file"<<endl;
  cout<<"-write    --after opening the file,you can write the content in the file"<<endl;
  cout<<"-quit     --quit the system"<<endl;
}

void Command::splitCommand(string str,char split){
  list<string> list;
  int p = 0;
  for(int i = 0;i<str.size();i++){
    if (str.at(i) == split) {
      string sub = str.substr(p,i-p);
      list.push_back(sub);
      p=i+1;
    }
  }
  list.push_back(str.substr(p,str.size()));
  this->list_cmd = list;
}

void Command::disk(){
  fo->disk();
}

void Command::inode(){
  fo->inode();
}

void Command::open(){
  list_cmd.pop_front();
  list<string>::iterator i = list_cmd.begin();
  if(list_cmd.size() == 1){
    string name = (string)*i;
    fo->f_open(name);
  }else{
    cout<<"<!------open command should like open name------!>"<<endl;
  }
}

void Command::close(){
  fo->f_close();
}
