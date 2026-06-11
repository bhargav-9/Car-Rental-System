#include <iostream>
#include <string>
#include "Auth.h"

using namespace std;

int main()
{   
    cout<<"--------------------------------------------------------------------------------------------\n\n";
    cout<<"\t\t******** WELCOME TO BHARGAV's CAR RENTAL SYSTEM ********\t\t\t\t\t\n\n";
    cout<<"--------------------------------------------------------------------------------------------\n";
    cout<<"Choose one of the two options."<<endl;
    cout<<"1.Login \t 2.Register(Only if you are a new customer) \t 3.Exit"<<endl;
    string a;
    cin>>a;
    if(a=="1"){
        Login();
        return main();
    }
    else if(a=="2"){
        Register();
        return main();
    }
    else if(a=="3") return 0;

    else {
        cout<<"Invalid option.\n";
        return main();
    }
    return 0;
}
