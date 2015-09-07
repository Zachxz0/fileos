#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include <iostream>
#include <list>
class Fileoperator;
using namespace std;
class Command{
private:
  list<string> list_cmd;
  Fileoperator *fo;
public:
  Command(Fileoperator *fo);
  Command parser(string cmd);

protected:
  void parserCommand(string c);
  void splitCommand(string str,char split);
  void f_create();
  void f_delete();
  void f_read();
  void f_write();
  void open();
  void close();
  void openDir();
  void login();
  void reg();
  void help();
  void quit();
  void diskFormat();
  void dir();
  void back();
  void disk();
  void inode();
};
#endif
