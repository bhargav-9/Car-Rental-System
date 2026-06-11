#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class user{
    public:
        int Id;
        string Name={};
        string password={};
        string mobile={};
        int Fine_Due;
        int record;
        void Available_cars();
        void Rented_Cars(int role);
        void Pay_dues(int role);
        void Return_Car(int role);
        void book_car(int role);
        void Profile(int role);
        void Update_Profile(int role);
};

#endif
