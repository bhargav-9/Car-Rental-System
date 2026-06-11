#include "utils.h"
#include <fstream>
#include <sstream>
#include <ctime>

bool is_number(const string &s) {
    if (s.empty()) {
        return false;
    }
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

string take_date(){
    string s3;
    cout<<"Enter the year(a 4-digit number):\n";
    cin>>s3;
    if(!is_number(s3) || !(stoi(s3)>0 && stoi(s3)<=9999)){
        cout<<"This year is not valid.\n";
        return take_date();
    }

    string s2;
    cout<<"Enter a month(between 1 to 12):\n";
    cin>>s2;
    if(!is_number(s2) || !(stoi(s2)>0 && stoi(s2)<13)){
        cout<<"This month is not valid.\n";
        return take_date();
    }
    string s1;
    cout<<"Enter the day(between 1 to 31):\n";
    cin>>s1;
    if( !is_number(s1) ){
        cout<<"This day is not valid.\n";
        return take_date();
    }
    else {
        if((stoi(s2)==2 && !(stoi(s1)>0 && stoi(s1)<30)) || (!((stoi(s3) % 100 != 0) && (stoi(s3) % 4 == 0)) && stoi(s1)==29)){
            cout<<"This day is not valid.\n";
            return take_date();
        }
        else if((stoi(s2)==4 || stoi(s2)==6 || stoi(s2)==9 || stoi(s2)==11) && !(stoi(s1)>0 && stoi(s1)<31)){
            cout<<"This day is not valid.\n";
            return take_date();
        }
        else if((stoi(s2)==1 || stoi(s2)==3 || stoi(s2)==5 || stoi(s2)==7 || stoi(s2)==8 || stoi(s2)==10 || stoi(s2)==12) && !(stoi(s1)>0 && stoi(s1)<32)){
            cout<<"This day is not valid.\n";
            return take_date();
        }
    }
    string date=(s3)+"-"+(s2)+"-"+(s1);
    return date;        
}

int get_number_of_records(const string& filename) {
    ifstream file(filename);
    string line;
    int count = 0;

    while (getline(file, line)) {
        count++;
    }

    file.close();
    return count;
}

void update_record(const string& filename, const string& record_id, const string& new_data) {
    fstream file(filename, ios::in | ios::out);
    string line;

    if (!file){
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    file.seekg(0, ios::beg);

    ofstream temp("temp.csv");

    while (getline(file, line)){
        stringstream ss(line);
        vector<string> data;
        string item;
        while (getline(ss, item, ',')) {
            data.push_back(item);
        }

        if (!data.empty() && data[0] == record_id) {
            temp << new_data << endl;
        } else {
            for (size_t i = 0; i < data.size(); ++i){
                temp << data[i];
                if (i != data.size() - 1) {
                    temp << ",";
                }
            }
            temp << endl;
        }
    }

    file.close();
    temp.close();

    remove(filename.c_str());
    rename("temp.csv", filename.c_str());
}

int daysBetweenDates(string date1, string date2)
{
	stringstream ss(date1 + "-" + date2);
	int year, month, day;
	char hyphen;

	ss >> year >> hyphen >> month >> hyphen >> day;
	struct tm starttm = { 0, 0, 0, day,month - 1, year - 1900 };
	time_t start = mktime(&starttm);

	ss >> hyphen >> year >> hyphen>> month >> hyphen >> day;
	struct tm endtm = { 0, 0, 0, day,month - 1, year - 1900 };
	time_t end = mktime(&endtm);

	return (end - start) / 86400;
}

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
