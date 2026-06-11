#include "Manager.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>

void manager::Add(){
    cout<<"\n\n---------------------------------------ADD------------------------------\n";
    cout<<"What do you want to add?\n1.Car\t\t2.Customer\t\t3.Employee\n";
    string a;
    cin>>a;
    string filename;
    if(a=="1"){
        filename="cars.csv";
        fstream file;
        string model,condition,price;
        cout<<"Enter the model name of the car:\n";
        cin>>model;
        cout<<"Enter the current condition of the car:\n1.Good\t\t\t\t2.Damaged\n";
        string a1;
        cin>>a1;
        if(a1=="1") condition="Good";
        else if(a1=="2") condition="Damaged";
        else {cout<<"Not a valid option\n";return Add();}
        cout<<"Enter the per day rental price of the car:\n";
        cin>>price;
        if(is_number(price)==0 || stoi(price)<=0){
            cout<<"Not a valid number.\n";
            return Add();
        }
        file.open(filename,ios::out | ios::app);
        int Id=get_number_of_records(filename);
        file<<Id<<","<< model << ","<<condition <<","<<"NA"<<","<<price<<",In_Use"<<"\n";
        file.close();
        cout<<"This car is added to the system!!"<<endl;
    }
    else if(a=="2" ||a=="3"){
        if(a=="2") filename="Customer.csv";
        else filename="Employee.csv";
        fstream file;
        string name,password,mobile;
        cout<<"Enter the name of the person:\n";
        cin>>name;
        cout<<"Enter a password for him(min 4 letters and should consist of a single word):\n";
        cin>>password;
        if(password.size()<4){
            cout<<"Length is less than 4."<<endl;
            return Add();
        }
        cout<<"Enter his mobile number: \n";
        cin>>mobile; 
        if(!is_number(mobile) || mobile.size()!=10){
            cout<<"Not a valid number\n";
            return Add();
        }
        file.open(filename,ios::out | ios::app);
        int Id=get_number_of_records(filename);
        file<<Id<<","<<name << ","<<password<<","<<"0"<<",10,"<<mobile<<",Existing\n";
        file.close();
        cout<<"This person is added to the system!!"<<endl;
    }
    else cout<<"Not a valid option.\n";
    cout<<"-----------------------------------------------------------------\n";
}

void manager::Delete(){
    cout<<"\n\n---------------------------------------DELETE------------------------------\n";
    cout<<"What do you want to delete?\n1.Car\t\t2.Customer\t\t3.Employee\n";
    string a;
    cin>>a;
    string filename;
    int flag=0;
    if(a=="1"){
        filename="cars.csv";
        cout<<"Enter the car Id which you want to delete:\n";
        string id;
        cin>>id;
        if(is_number(id)==0){
            cout<<"The id should consist of only numbers.\n";
            return Update();
        }
        ifstream file(filename); 
        if (!file.is_open()) {
        cerr << "Error: Something is wrong. Please try again." << endl;
        return Delete();
        }
        string line;
        vector<string> parts;
        while (getline(file, line)) {
            parts = split(line, ','); 
            if(parts[0]==id && parts[3]=="NA" && parts[5]=="In_Use"){
                flag=1;
                break;
            }
        }
        file.close();
        if(flag==0) {
            cout<<"Either you entered an invalid id number or this car is rented by someone.\n";
            return Delete();
        }
        string data= parts[0]+","+parts[1]+","+parts[2]+","+parts[3]+","+parts[4]+",Deleted";
        update_record(filename,id,data);
        cout<<"This car is deleted!!!\n";
    }
    else if(a=="2" ||a=="3"){
        if(a=="2") filename="Customer.csv";
        else filename="Employee.csv";
        cout<<"Enter the person Id whom you want to delete:\n";
        string id;
        cin>>id;
        if(is_number(id)==0){
            cout<<"The id should consist of all numbers.\n";
            return Delete();
        }
        ifstream file(filename); // Open the CSV file
        if (!file.is_open()) {
            cerr << "Error: Something is wrong. Please try again." << endl;
            return Delete();
        }
        string line;
        vector<string> parts;
        while (getline(file, line)) {
            parts = split(line, ','); 
            if(parts[0]==id && parts[6]=="Existing"){
                cout<<"You are trying to delete "<<parts[1]<<". He has fine dues of Rs."<<parts[3]<<"\n";
                cout<<"Are you sure you want to delete him?\n1.Yes\t\t\t2.No\n";
                string b;
                cin>>b;
                if(b=="1") {
                    flag=1;
                }
                else if(b=="2") cout<<"------------------------Thank you!----------------------\n";
                else cout<<"Not a valid option.\n";
                break;
            }
        }
        file.close();
        if(flag==1){
            string data= parts[0]+","+parts[1]+","+parts[2]+","+parts[3]+","+parts[4]+","+parts[5]+",Deleted";
            update_record(filename,id,data);
            cout<<"This person is deleted!!!\n";
        }
    }
    else {cout<<"Not a valid option.\n";return Delete();}
    if(flag==0 && stoi(a)<=3 && stoi(a)>=1){
        cout<<"Invalid Id\n";
    }
    cout<<"--------------------------------------------------------------------------\n";
}


void manager::Update(){
    cout<<"\n\n---------------------------------------UPDATE------------------------------\n";
    cout<<"What do you want to update?\n1.Car\t\t2.Customer\t\t3.Employee\n";
    string a;
    cin>>a;
    string filename;
    int flag=0;
    if(a=="1"){
        cout<<"Enter the id of the car you want to update:\n";
        string s;
        cin>>s;
        if(is_number(s)==0){
            cout<<"The id should consist of only numbers.\n";
            return Update();
        }
        filename="cars.csv";
        ifstream file(filename); // Open the CSV file
        if (!file.is_open()) {
        cerr << "Error: Something is wrong. Please try again." << endl;
        return Update();
        }
        string line;
        vector<string> parts;
        while (getline(file, line)) {
            parts = split(line, ','); 
            if(parts[0]==s && parts[5]=="In_Use" && parts[3]=="NA"){
                cout<<"You are updating a car of model "<<parts[1]<<"."<<endl;
                flag=1;
                break;
            }
            else if(parts[0]==s && parts[5]=="In_Use" ){
                cout<<"This car is currently rented. You can only update attributes of cars that are not rented.\n";
                return Update();
            }
        }
        file.close();
        if(flag==0) {cout<<"You entered an invalid car id.\n";return Update();}
        cout<<"Which attribute of the car do you want to update?\n1.Model\t\t2.Condition\t\t3.Rental price per day\n";
        string b;
        cin>>b;
        int change=0;
        if(b=="1") {
            cout<<"Enter the new model name for this car.\n";
            string s1;
            cin>>s1;
            change=1;
            parts[1]=s1;
        }
        else if(b=="2"){
            cout<<"Enter the new condition of this car.\n1.Good\t\t\t2.Damaged\n";
            string s1;
            cin>>s1;
            if(s1=="1") {parts[2]="Good";change=1;}
            else if(s1=="2") {parts[2]="Damaged";change=1;}
            else {cout<<"Not a valid option.\n";return Update();}
        }
        else if(b=="3"){
            cout<<"Enter the new rental price per day for this car.\n";
            string s1;
            cin>>s1;
            change=1;
            parts[4]=s1;
        }
        else {cout<<"Not a valid option.\n";return Update();}
        if(change==1){
            flag=2;
            string new_data=s+","+parts[1]+","+parts[2]+","+parts[3]+","+parts[4]+","+parts[5];
            update_record(filename,s,new_data);
        }
    }
    else if(a=="2" || a=="3"){
        if(a=="2") filename="Customer.csv";
        else filename="Employee.csv";
        cout<<"Enter the id of the person whose data you want to update:\n";
        string s;
        cin>>s;
        if(is_number(s)==0){
            cout<<"The id should consist of only numbers.\n";
            return Update();
        }
        ifstream file(filename); // Open the CSV file
        if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return ;
        }
        string line;
        vector<string> parts;
        while (getline(file, line)) {
            parts = split(line, ',');
            if(parts[0]==s && parts[6]=="Existing"){
                cout<<"You are updating the data of "<<parts[1]<<"."<<endl;
                flag=1;
                break;
            }
        }
        file.close();
        if(flag==0) {cout<<"You entered an invalid id number.\n";return Update();}
        cout<<"Which attribute of "<<parts[1]<<" do you want to update?\n1.Fine_Due\t2.Record\n";
        string b;
        cin>>b;
        int change=0;
        if(b=="1") {
            cout<<"Enter the new fine dues for this person.\n";
            string s1;
            cin>>s1;
            if(is_number(s1)==0 || stoi(s1)<0){
                cout<<"The fine due should consist of only positive numbers.\n";
                return Update();
            }
            else if(stoi(s1)>50000){
                cout<<"The fine dues cannot exceed Rs.50000.\n";
                return Update();
            }
            change=1;
            parts[3]=s1;
        }
        else if(b=="2"){
            cout<<"Enter the new record number for this person.\n";
            string s1;
            cin>>s1;
            if(is_number(s)==0){
                cout<<"The record should consist of only numbers.\n";
                return Update();
            }
            else if(stoi(s1)<0){
                cout<<"Record can't be negative.\n";
                return Update();
            }
            else if(stoi(s1)<10){
                cout<<"You cannot set the record greater than 20.\n";
                return Update();
            }
            change=1;
            parts[4]=s1;
        }
        else {cout<<"Not a valid option.\n";return Update();}
        if(change==1){
            flag=2;
            string new_data=s+","+parts[1]+","+parts[2]+","+parts[3]+","+parts[4]+","+parts[5]+",Existing";
            update_record(filename,s,new_data);
        }
    }
    else {cout<<"Not a valid option.\n";return Update();}
    if(flag==2) cout<<"You have successfully updated the data.\n";
    cout<<"-----------------------------------------------------------\n";
}



void manager::Available_Cars(){
    cout<<"\n\n------------------------AVAILABLE CARS-------------------------\n\n";
    ifstream file("cars.csv"); // Open the CSV file
    if (!file.is_open()) {
    cerr << "Error: Unable to open file." << endl;
    return ;
    }

    string line;
    while (getline(file, line)){
        vector<string> parts = split(line, ','); 
        if(parts[5]=="In_Use")
        {
            cout<<"Id: "<<parts[0]<<"\nModel: "<<parts[1]<<"\nPresent Condition: "<<parts[2]<<"\nRental price for a day: "<<parts[4];
            if(parts[3]!="NA"){
                cout<<"\nAvailability: No\nRented By: "<<parts[6]<<endl;
            }
            else cout<<"\nAvailability: Yes.\n";
            cout<<"\t\t\t***********\t\t\t\n";
        }
    }
    file.close();
    cout<<"\n\n-----------------------------------------------------------------\n";

}


void manager::Current_Data(int role){
    string filename;
    string name;
    if(role==1) {filename="Customer.csv";name="CUSTOMERS";}
    else {filename="Employee.csv";name="EMPLOYEES";}
    cout<<"\n\n------------------------CURRENT "<<name<<"-------------------------\n\n";

    ifstream file(filename); // Open the CSV file
    if (!file.is_open()) {
    cerr << "Error: No data available." << endl;
    return ;
    }
    if(get_number_of_records(filename)==0) {
        cout<<"You don't have any customer currently.\n";
        return;
    }
    string line;
    while (getline(file, line)){
        vector<string> parts = split(line, ','); 
        if(parts[6]=="Existing")
        {
            cout<<"Id: "<<parts[0]<<"  |  Name: "<<parts[1]<<"  |  password: "<<parts[2]<<"  |  Fine Due: "<<parts[3]<<"  |  Record: "<<parts[4]<<" | Mobile No: "<<parts[5]<<endl;
        }
    }
    file.close();
    cout<<"\n\n---------------------------------------------------------------------\n";
}


void manager::Update_Profile(){
    cout<<"\n\n----------------------------------YOUR PROFILE-----------------------------------\n";
    string filename="Manager.csv";
    ifstream file(filename);
    if (!file.is_open()) {
    cerr << "This profile doesn't exist." << endl;
    return ;
    }

    string line;
    while (getline(file, line)) {
        vector<string> parts = split(line, ','); 
        if(parts[0]==to_string(this->Id)){
            cout<<"Id: "<<Id<<endl;
            cout<<"Name: "<<Name<<endl;
            cout<<"password: "<<password<<endl;
            cout<<"Mobile No: "<<mobile<<endl;
        }
    }
    file.close();

    cout<<"\n\nDo you want to update your profile?\n1.Yes\t\t\t2.No\n";
    string a;
    cin>>a;
    if(a=="1"){
        cout<<"What do you want to update?\n1.Name\t\t2.Password\t\t3.Mobile No.\n";
        string b;
        cin>>b;
        string new_data;
        if(b=="1"){
            cout<<"Enter you new Name.\n";
            string name;
            cin>>name;
            new_data=to_string(Id)+","+name+","+password+","+mobile;
        }
        else if(b=="2"){
            cout<<"Enter you new password(minimum 4 letters).\n";
            string pass;
            cin>>pass;
            if(pass.size()<4){
                cout<<"Your password is less than 4 letters.\n";
                return Update_Profile();
            }
            new_data=to_string(Id)+","+Name+","+pass+","+mobile;
        }
        else if(b=="3"){
            cout<<"Enter you new mobile number(of 10 digits).\n";
            string pass;
            cin>>pass;
            if(is_number(pass)==0 || pass.size()!=10){
                cout<<"Invalid mobile number.\n";
                return Update_Profile();
            }
            new_data=to_string(Id)+","+Name+","+password+","+pass;
        }
        else {
            cout<<"Not a valid option.\n";
            return Update_Profile();
        }
        update_record(filename,to_string(Id),new_data);
        cout<<"Your profile is updated :))\n";
    }
    else if(a!="2"){
        cout<<"Invalid option.\n";
        return Update_Profile();
    }
    cout<<"\n------------------------------------------------------------------------\n";
}


void manager::Display_Menu(){
    cout<<"\n\nPlease choose one of the below option number."<<endl;
    cout<<"1.Show the list of available cars."<<endl;
    cout<<"2.Add a car/customer/employee."<<endl;
    cout<<"3.Delete a car/customer/employee."<<endl;
    cout<<"4.Update a car/customer/employee."<<endl;
    cout<<"5.Show the list of all customers.\n";
    cout<<"6.Show the list of all employees.\n";
    cout<<"7.Show/Update my profile.\n";
    cout<<"8.Logout."<<endl;
    string s;
    cin>>s;
    if(!is_number(s)){
        cout<<"Not a valid option.\n";
        Display_Menu();
    }
    int a=stoi(s);
    switch(a){
        case 1:
            Available_Cars();
            break;
        case 2:
            Add();
            break;
        case 3:
            Delete();
            break;
        case 4:
            Update();
            break;
        case 5:
            Current_Data(1);
            break;
        case 6:
            Current_Data(2);
            break;
        case 7:
            Update_Profile();
            break;
        case 8:
            cout<<"Thank you for using our services !!\n";
            cout<<"You are successfully logged out.\n";
            return;
        default:
            cout<<"Not a valid option.\n";
            Display_Menu();
    }
    cout<<"Do you want to exit from the application?\n1.Yes\t\t\t2.No\n";
    string s1;
    cin>>s1;
    if(!is_number(s1)){
        cout<<"Not a valid option.\n";
        return Display_Menu();
    }
    int a1= stoi(s1);
    if(a1==1){
        cout<<"Thank you for using our services !!\n";
        return ;
    }
    else if(a1==2){
        system("cls");
        Display_Menu();
    }
    else {
        cout<<"Not a valid option.\n";
        Display_Menu();
    } 
}    
