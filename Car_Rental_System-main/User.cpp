#include "User.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>

void user::Update_Profile(int role){
    string filename;
    if(role==1) filename="Customer.csv";
    else filename="Employee.csv";

    cout<<"What do you want to update?\n1.Name\t\t2.Password\t\t3.Mobile No\n";
    string b;
    cin>>b;
    string new_data;
    if(b=="1"){
        cout<<"Enter you new Name:\n";
        string name;
        cin>>name;
        new_data=to_string(Id)+","+name+","+password+","+to_string(Fine_Due)+","+to_string(record)+","+mobile+",Existing";
        string old="Each_Cus_DBs/"+this->Name+".csv";
        string newname="Each_Cus_DBs/"+name+".csv";
        rename(old.c_str(),newname.c_str());
    }
    else if(b=="2"){
        cout<<"Enter you new password(minimum 4 letters):\n";
        string pass;
        cin>>pass;
        if(pass.size()<4){
            cout<<"Your password is less than 4 letters.\n";
            return Update_Profile(1);
        }
        new_data=to_string(Id)+","+Name+","+pass+","+to_string(Fine_Due)+","+to_string(record)+","+mobile+",Existing";
    }
    else if(b=="3"){
        cout<<"Enter you new mobile number(of 10 digits):\n";
        string pass;
        cin>>pass;
        if(is_number(pass)==0 || pass.size()!=10){
            cout<<"Invalid mobile number.\n";
            return Update_Profile(1);
        }
        new_data=to_string(Id)+","+Name+","+password+","+to_string(Fine_Due)+","+to_string(record)+","+pass+",Existing";
    }
    else {
        cout<<"Not a valid option.\n";
        return Update_Profile(1);
    }
    update_record(filename,to_string(Id),new_data);
    cout<<"Your profile is updated :))\n";
    cout<<"-----------------------------------------------------------------\n";
}

void user::Available_cars(){
    cout<<"------------------------AVAILABLE CARS-------------------------\n\n";
    ifstream file("cars.csv"); // Open the CSV file
    if (!file.is_open()) {
    cerr << "Error: Unable to open file." << endl;
    return ;
    }
    string line;
    while (getline(file, line)) {
        vector<string> parts = split(line, ','); 
        if(parts[5]=="In_Use")
        {
            cout<<"Id: "<<parts[0]<<"\nModel Name: "<<parts[1]<<"\nCondition: "<<parts[2]<<"\nRental Price per day: "<<parts[4];
            if(parts[3]=="NA") cout<<"\nAvailability: Yes";
            else cout<<"\nAvailability: No";
            cout << "\n\n\t\t\t*********\t\t\t\n";
        }
    }
    file.close();
    cout<<"-----------------------------------------------------------------\n";
}

void user::Pay_dues(int role){
    cout<<"------------------------PAY DUES-------------------------\n\n";
    if(Fine_Due==0){
        cout<<"You don't have dues :)\n";
        return;
    }
    cout<<"Your current dues are Rs."<<Fine_Due<<"\n";
    cout<<"Enter the amount you want to clear:\n";
    string sum;
    cin>>sum;
    if(is_number(sum)==0 || stoi(sum)<0) {
        cout<<"Not a valid number.\n";
        return Pay_dues(role);
    }
    if(stoi(sum)!=0) cout<<"Thank you for paying.\n";
    if(stoi(sum)>=Fine_Due) cout<<"You have no dues left.\n";
    else cout<<"You still have Rs."<<Fine_Due-stoi(sum)<<" as our fine dues.\n";
    Fine_Due=Fine_Due-stoi(sum);
    string data=to_string(this->Id)+","+this->Name+","+this->password+","+to_string(Fine_Due)+","+to_string(record)+","+mobile+",Existing";
    string filename;
    if(role==1) filename="Customer.csv";
    else filename="Employee.csv";
    update_record(filename,to_string(Id),data);
    cout<<"----------------------------------------------------------------\n";
}

void user::Rented_Cars(int role){
    cout<<"------------------------RENTED CARS-------------------------\n\n";
    string filename;
    if(role==1) filename="Each_Cus_DBs/"+this->Name+".csv";
    else filename="Each_Emp_DBs/"+this->Name+".csv";
    ifstream file(filename); 
    if (!file.is_open()) {
    cerr << "You have not rented any cars yet." << endl;
    return ;
    }

    string line;
    while (getline(file, line)) {
    vector<string> parts = split(line, ','); 
    cout<<"Id: "<<parts[0]<<"\nModel Name: "<<parts[1]<<"\nCondition: "<<parts[2]<<"\nStart Date: "<<parts[3]<<"\nEnd Date: "<<parts[4]<<"\nRental Price per day: "<<parts[5]<<"\nStatus: "<<parts[6];
    cout << "\n\n\t\t\t*********\t\t\t\n";
    cout << endl;
    }
    file.close();
    cout<<"------------------------------------------------------------------\n";
}

void user::Return_Car(int role){
    cout<<"------------------------RETURN CAR-------------------------\n\n";
    cout<<"Enter the car Id:\n";
    string id;
    cin>>id;
    if(is_number(id)==0) {
        cout<<"The Id should consist of only numbers.\n";
        return Return_Car(role);
    }
    int ret_id=stoi(id);
    cout<<"Enter the following details of today's date\n";
    string s1;
    s1=take_date();

    cout<<"What is the condition of car currently?"<<endl;
    cout<<"1.Good\t\t\t2.Damaged"<<endl;
    string s;
    cin>>s;
    if(s!="1" && s!="2") {
        cout<<"Not a valid option.\n";
        return Return_Car(role);
    }
    string filename;
    if(role==1) filename="Each_Cus_DBs/"+this->Name+".csv";
    else filename="Each_Emp_DBs/"+this->Name+".csv";
    ifstream file(filename);
    if (!file.is_open()) {
    cerr << "Error: You have not rented any car yet." << endl;
    return;
    }
    int flag=0;
    string line;
    while (getline(file, line)) {
        vector<string> parts = split(line, ','); 
        if (stoi(parts[0]) ==  ret_id && parts[6].compare("Rented")==0){
            if(daysBetweenDates(parts[3],s1)<0){
                cout<<"You entered a date of a day before you rented the car.\n";
                return Return_Car(role);
            }
            if(daysBetweenDates(parts[4],s1)>0 ){
                int fine = daysBetweenDates(parts[4],s1)*(stoi(parts[5])*1.5) ;
                cout<<"You are returning the car after the due date and this leads to a fine of Rs."<<fine<<".\n";
                record -= 1;
                Fine_Due +=fine;
            }
            int con;
            if(parts[2].compare("Good")==0) con=1;
            else con=2;
            if(stoi(s)-con!=0){
                if(stoi(s)==2){
                    cout<<"As you have caused damage to the car. You will be fined Rs.5000.\n";
                    Fine_Due+=5000;
                    record -=2;
                    parts[2]="Damaged";
                }
                else {
                    cout<<"Thank you for improving the car's condition:)))\n";
                    record +=2;
                    parts[2]="Good";
                }
            }
            string new_data = parts[0]+","+parts[1]+","+parts[2]+","+parts[3]+","+parts[4]+","+parts[5]+",Returned";
            file.close();
            string filename;
            if(role==1) filename="Each_Cus_DBs/"+this->Name+".csv";
            else filename="Each_Emp_DBs/"+this->Name+".csv";
            flag=1;
            cout<<"You successfully returned the car."<<endl;
            update_record(filename,parts[0],new_data);
            string fname="cars.csv";
            string car=parts[0]+","+parts[1]+","+parts[2]+",NA,"+parts[5]+",In_Use";
            update_record(fname,parts[0],car);
            string f1name;
            if(role==1)  f1name="Customer.csv";
            else f1name="Employee.csv";
            string data=to_string(this->Id)+","+this->Name+","+this->password+","+to_string(Fine_Due)+","+to_string(record)+","+mobile+",Existing";
            update_record(f1name,to_string(this->Id),data);
            break;
        }
        if(flag==0) cout<<"You have not rented any car with this id.\n";
    }
    cout<<"-----------------------------------------------------------------------\n";
}

void user::Profile(int role){
    cout<<"\n\n----------------------------------YOUR PROFILE-----------------------------------\n";
    string filename;
    if(role==1) filename="Customer.csv";
    else filename = "Employee.csv";
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
            cout<<"Fine Due: "<<Fine_Due<<endl;
            cout<<"Your record: "<<record<<endl;
        }
    }
    file.close();
    cout<<"\n------------------------------------------------------------------------\n";
}

void user::book_car(int role)
{
    cout<<"\n\n------------------------RENT A CAR-------------------------\n\n";
    cout<<"Do you want to see the list of available cars?\n1.Yes\t\t\t2.No\t\t\t3.Exit\n";
    string a;
    cin>>a;
    if(a=="1") Available_cars();
    else if(a=="3"){
        return;
    }
    else if(a!="2") {
        cout<<"Not a valid option.\n";
        return book_car(role);
    }
    cout<<"Enter the car id you want to rent\n";
    string s;
    cin>>s;
    if(!is_number(s)){
        cout<<"The Id should be consist of numbers only.\n";
        return book_car(role);
    }
    cout<<"Enter the following details of today's date i.e., the start date\n";
    string s1;
    s1=take_date();
    cout<<"Enter the following details of end date\n";
    string s2;
    s2=take_date();
    if(daysBetweenDates(s1,s2)<0){
        cout<<"Your dates are not correct!\n";
        return book_car(role);
    }
    else if(daysBetweenDates(s1,s2)>15){
        cout<<"Sorry, we don't allow renting cars for more than 15 days :(((\n";
        return ;
    }

    ifstream file("cars.csv");
    if (!file.is_open()) {
    cerr << "Error: Unable to open file." << endl;
    return;
    }

    string line;
    int flag=0;
    string change ;
    string new_data;
    string id,price,cond;
    while (getline(file, line)) {
        vector<string> parts = split(line, ','); 
        
        if(parts[0].compare(s)==0){
            if (parts[3].compare("NA") == 0 && parts[5]!="Deleted") {
                id=parts[0];
                price=parts[4];
                cond=parts[2];
                cout << "\nCar is Available." << endl;
                cout<<"It has a per day rental price of Rs."<<parts[4]<<"."<<endl;
                vector<string> sdate=split(s1,'-');
                vector<string> edate=split(s2,'-');
                cout<<"The total cost for these days will be Rs."<<stoi(parts[4])*(daysBetweenDates(s1,s2)+1)<<".\n";
                if(role==2) cout<<"Being a part of our company,you get a discount of 15%. The discounted price for you will be Rs."<<0.85*(float)(stoi(parts[4])*(daysBetweenDates(s1,s2)+1))<<endl;
                cout<<"The car is in "<<parts[2]<<" state."<<endl;
                cout<<"\nChoose one of the two options\n";
                cout<<"1.You want to rent the car and are ready to pay the amount now.\n";
                cout<<"2.You don't want to rent the car.\n";
                int cfm;
                cin>>cfm;
                if(cfm!=1 && cfm!=2) {
                    cout<<"Not a valid option.\n";
                    return book_car(role);
                }
                if(cfm==1){
                    if(record>0){
                    flag=1;
                    new_data = parts[0]+","+parts[1]+","+parts[2]+","+s2+","+parts[4]+",In_Use"+","+this->Name;
                    change=parts[0];
                    fstream file;
                    string fname;
                    if(role==1) fname="Each_Cus_DBs/"+this->Name+".csv";
                    else fname="Each_Emp_DBs/"+this->Name+".csv";
                    file.open(fname, ios::out | ios::app);
                    file <<parts[0]<<","<< parts[1] << ","<<parts[2] <<","<<s1<<","<<s2<<","<<stoi(parts[4])<<","<<"Rented"<<"\n";
                    file.close();
                    }
                    else{
                        flag=2;
                        cout<<"Sorry....you don't have a good record enough to rent a car :(((\n";
                    }
                }
                else{
                    flag=2;
                    cout<<"\n-----------------Thank you!!------------------";
                }
                break;
            }
        }  
    }
    file.close();
    if(flag==0) cout<<"This car is not available :(\n";
    else if(flag==1) {
        string filename="cars.csv";
        cout<<"You successfully rented the car :)\nBelow is the invoice."<<endl;
        cout<<"\n\n----------------------ASMITA'S CAR RENTAL SYSTEM-------------------------\n\n";
        cout<<"You rented the following car.\n";
        cout<<"Car Model   \t\t\t"<<s<<"\n";
        cout<<"Car Id       \t\t\t"<<id<<"\n";
        cout<<"Rental Price\t\t\tRs."<<price<<"\n";
        cout<<"Start Date  \t\t\t"<<s1<<"\n";
        cout<<"End Date    \t\t\t"<<s2<<"\n";
        cout<<"Total Price  \t\t\tRs."<<(daysBetweenDates(s1,s2)+1)*stoi(price)<<"\n\n";
        cout<<"Please remember this car id. It will be required while returning the car.\n";
        update_record(filename,change,new_data);
    }
    cout<<"------------------------------------------------------------------------\n";
}
