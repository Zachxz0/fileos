#ifndef FILEOS_H
#define FILEOS_H
class Command;
class Fileoperator;
class User;
struct Sys;
struct File;
struct Dir;
struct Inode;
struct DiskSpace;
#include <stack>
#include <fstream>
#include <list>
#include <iostream>
#include <string>
using namespace std;
class Fileos
{
private:
    Command * cp;
    Fileoperator * fop;
    string current_dir;
    fstream * fs;
    Sys *root;
    DiskSpace *diskspace;
    User *user;
    list<User*> ls_user;
    stack<Dir*> stack_dir;
    list<File*> ls_files;
    list<Dir*> ls_dirs;
    list<Inode*> *ls_inodes;
    File *file;
public:
    Fileos();
    void startGetCmd();
    void addUser(User *user);
    unsigned int getUserQuantity();
    stack<Dir*> getStackOfDir();
    void setRoot(Sys *root);
    void setDiskSpace(DiskSpace *disk);
    void setInodes(list<Inode*> *ls_inodes);
    struct Sys* getRoot();
    list<User*> getUsers();
    list<Dir*> getDirs();
    list<File*> getFiles();
    DiskSpace* getDiskSpace();
    list<Inode*>* getInodes();
    void pushDir(Dir *dir);
    Dir* getTopStackOfDir();
    void popDir();
    Dir* convertToDir(Sys root);
    Dir* convertToDir(User user);
    void addRootToDir();
    string getStrDir();
    void setUser(User *user);
    void removeUser();
    User* getUser();
    bool isLogin();
    void initData();
    void addFile(File *file);
    void addDir(Dir *dir);
    Dir* isExitedDirInList(unsigned int id,unsigned int uid,unsigned int pid);
    Dir* isExitedDirInList(string name,unsigned int uid,unsigned int pid);
    File* isExitedFileInList(unsigned int id,unsigned int uid,unsigned int pid);
    bool removeDirFromList(unsigned int id);
    bool removeFileFromList(unsigned int id);
    void setFile(File*);
    File* getFile();
};
#endif
