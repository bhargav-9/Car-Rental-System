#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

bool is_number(const string &s);
string take_date();
int get_number_of_records(const string& filename);
void update_record(const string& filename, const string& record_id, const string& new_data);
int daysBetweenDates(string date1, string date2);
vector<string> split(const string &s, char delimiter);

#endif
