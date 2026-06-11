#Car Rental System

A car rental system in C++ using Object Oriented Programming paradigms. The code is present in `main.cpp`. There are 3 kinds of users which are customers, employees and manager. They have different functionalities and specifications. These details are mentioned in the `Assignment`. All the required functionalities of the assignment have been implemented accordingly.

There are 4 main databases of this system.
1. `cars.csv` stores the details of each car available in this system.One row stores details of one car[Id,Model Name,Condition,Due Date,Rental Price,Status,Rented by(if rented)].
2. `Customer.csv` stores the details of each customer. One row stores details of one customer[Id,Name,password,Fine Due,Record,Mobile,Status].
3. `Employee.csv` stores the details of each employee. One row stores details of one customer[Id,Name,password,Fine Due,Record,Mobile,Status].
4. `Manager.csv` stores the details of manager. One row stores details of one customer[Id,Name,password,Mobile].

There are two other folders.
1.`Each_Cus_DBs` stores the csv files corresponding to some customer which has the list and details of the cars rented by them[Id,Model Name,Condition,Start Date,End Date,Total Price,Status].
2.`Each_Emp_DBs` stores the csv files corresponding to some employee which has the list and details of the cars rented by them[Id,Model Name,Condition,Start Date,End Date,Total Price,Status].


I have made the following assumptions in this software:

1. The car has only two conditions. a)Good and b)Damaged
2. No customer can rent a car for more than 15 days.
3. If a customer/employee returns the car late by n days, then he will be charged a fine which is calculated as Fine = (1.5 * rental price of that car)*n and his record decreases by 1.
4. If a customer/employee causes damage to the car, he is fined Rs.5000 and his record decreases by 2.
5. If a customer has fine due greater than 50,000 , then he can't avail other facilities until his fine due is less than 50,000.
6. Also, if a customer/employee has a record less than or equal to 0, then he can't rent a car.
7. If a customer/employee improves the condition of the car, his record increases by 2.
8. A new customer/employee has a record of 10.

To run the system, type the following commands in the console:

```
g++ main.cpp -o main
.\main
```

The system will run on the console. The data modified throughout the program run will be reflected in the csv files.
