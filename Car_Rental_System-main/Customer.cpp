#include "Customer.h"
#include "utils.h"
#include <iostream>

using namespace std;

void customer::Display_Menu()
{
    if(this->Fine_Due>50000) {
        cout<<"Sorry...your fine due exceeds the upper bound we had set. So, you need to pay the dues to avail our facilities.\n";
        cout<<"Choose one of the following options:\n1.Pay dues\t\t\t2.Exit\n";
        string a;
        cin>>a;
        if(a=="1") Pay_dues(1);
        else if(a=="2") return ;
        else {
            cout<<"Not a valid option.\n";
            return Display_Menu();
        }
    }
    else{
    cout<<"Please choose one of the below option number."<<endl;
    cout<<"1.Show the list of available cars."<<endl;
    cout<<"2.Show the list of rented cars."<<endl;
    cout<<"3.Rent a Car."<<endl;
    cout<<"4.Return a rented car."<<endl;
    cout<<"5.Pay dues."<<endl;
    cout<<"6.Show Profile."<<endl;
    cout<<"7.Update Profile."<<endl;
    cout<<"8.Logout."<<endl;
    string s;
    cin>>s;
    if(!is_number(s)){
        cout<<"It should be a number.\n";
        return Display_Menu();
    }
    int a = stoi(s);
    switch(a){
        case 1:
            Available_cars();
            break;
        case 2:
            Rented_Cars(1);
            break;
        case 3:
            book_car(1);
            break;
        case 4:
            Return_Car(1);
            break;
        case 5:
            Pay_dues(1);
            break;
        case 6:
            Profile(1);
            break;
        case 7:
            Update_Profile(1);
            break;
        case 8:
            cout<<"You are successfully logged out.\n";
            return;
        default:
            cout<<"Not a valid option.\n";
    }
    cout<<"Do you want to exit from the application?\n1.Yes\t\t\t2.No\n";
    string a1;
    cin>>a1;
    if(a1=="1"){
        cout<<"Thank you for using our services !!\n";
        return ;
    }
    else if(a1=="2"){
        system("cls");
        Display_Menu();
    }
    else {
        cout<<"Not a valid option.\n";
        Display_Menu();
    }
    }
}
