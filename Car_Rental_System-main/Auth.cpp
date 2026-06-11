#include "Auth.h"
#include "Customer.h"
#include "Employee.h"
#include "Manager.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void Login()
{
    int count=0;
    cout<<"\n\n------------------------LOGIN-------------------------\n\n";
    string new_name,new_password;
    string s;
    cout<<"Choose your role among the following:\n1.Customer\t\t2.Employee\t\t3.Manager\t\t4.Exit\n";
    cin>>s;
    if( !is_number(s) || stoi(s)>4){
        cout<<"Not a valid option.\n";
        return Login();
    }
    else if(stoi(s)==4){
        return;
    }

    int  a = stoi(s);
    cout<<"Enter your name:(First Name)"<<endl;
    cin>>new_name;

    cout<<"Enter your password:(single word)"<<endl;

    cin>>new_password;


    string filename ;
    if(a==1) 
    {
        filename="Customer.csv";
        customer New_Customer;
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            vector<string> parts = split(line, ','); 
            if (parts[1] ==new_name  && parts[2] == new_password && parts[6]=="Existing") {
                count=1;
                New_Customer.Name=parts[1];
                New_Customer.Id = stoi(parts[0]);
                New_Customer.password= parts[2];
                New_Customer.mobile=parts[5];
                New_Customer.Fine_Due=stoi(parts[3]);
                New_Customer.record=stoi(parts[4]);
                break;
            }
        }
        file.close();
        if(count==1) {
            system("cls");
            cout<<"------------------------------MENU----------------------------------\n\n";
            cout<<"Welcome "<<New_Customer.Name<<"......."<<endl;
            New_Customer.Display_Menu();
        }
    }
    else if(a==2) {
        filename="Employee.csv";
        employ New_Employee;
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            vector<string> parts = split(line, ','); 
            if (parts[1] ==new_name  && parts[2] == new_password && parts[6]=="Existing") {
                
                count=1;
                New_Employee.Name=parts[1];
                New_Employee.Id = stoi(parts[0]);
                New_Employee.password= parts[2];
                New_Employee.mobile=parts[5];
                New_Employee.Fine_Due=stoi(parts[3]);
                New_Employee.record=stoi(parts[4]);
                break;
            }
        }
        file.close();
        if(count==1) {
            system("cls");
            cout<<"------------------------------MENU----------------------------------\n\n";
            cout<<"Welcome "<<New_Employee.Name<<"......."<<endl;
            New_Employee.Display_Menu();
        }
    }
    else if(a==3) {
        filename="Manager.csv";
        manager New_manager;
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            vector<string> parts = split(line, ','); 
            if (parts[1] ==new_name  && parts[2] == new_password) {
                
                count=1;
                New_manager.Name=parts[1];
                New_manager.Id = stoi(parts[0]);
                New_manager.password= parts[2];
                New_manager.mobile=parts[3];
                break;
            }
        }
        file.close();
        if(count==1) {
            system("cls");
            cout<<"------------------------------MENU----------------------------------\n\n";
            cout<<"Welcome Sir......."<<endl;
            New_manager.Display_Menu();
        }
    }
    else if(a==4){
        return;
    }
    
    if(count==0 && a<=3 && a>=1) {cout<<"You entered wrong data!!"<<endl;return Login();}
    
}

void Register()
{
    string Name,password;
    cout<<"\n\n------------------------REGISTER-------------------------\n\n";
    cout<<"Enter your name(First Name):"<<endl;
    cin>>Name;
    cout<<"Enter a password(of minimum 4 letters and should consists of single word):"<<endl;
    cin>>password;
    if(password.size()<4){
        cout<<"Length is less than 4."<<endl;
        return Register();
    }
    string mobile;
    cout<<"Enter your 10-digit mobile number:"<<endl;
    cin>>mobile;
    if(!is_number(mobile) || mobile.size()!=10){
        cout<<"Not a valid number\n";
        return Register();
    }
    else{
        string filename ;
        filename="Customer.csv";
        ifstream file(filename);
        int Id=get_number_of_records(filename);
        if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
        }
        int flag=0;
        string line;
        while(getline(file, line)){
            vector<string> parts = split(line, ',');
            if (parts[1]==Name) {
                flag=1;
                cout<<"This user name already exists. Try using a different one.\n";
                cout<<"3\n";
                break;
            }
        }
        file.close();
        if(flag==1) {
            return Register();
        }
        fstream ofile;
        ofile.open(filename,ios::out | ios::app);
        if(flag==0) {
            ofile<<Id<<","<< Name << ","<<password <<","<<"0"<<","<<"10,"<<mobile<<",Existing\n";
            cout<<"You are successfully registered!"<<endl;
        }
        ofile.close();
    }

}
