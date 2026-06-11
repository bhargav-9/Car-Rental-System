#ifndef MANAGER_H
#define MANAGER_H

#include <string>

using namespace std;

class manager{
public:
    int Id;
    string Name;
    string password;
    string mobile;
    void Add();
    void Delete();
    void Update();
    void Available_Cars();
    void Current_Data(int role);
    void Update_Profile();
    void Display_Menu();
};

#endif
