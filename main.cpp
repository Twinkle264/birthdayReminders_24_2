#include <iostream>
#include <ctime>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Friend {
    string name;
    tm birthday;
};

tm getCurrentDate() {
    time_t t_now = time(nullptr);
    return *localtime(&t_now);
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) {
    const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (month == 2 && isLeapYear(year)) ? 29 : days[month - 1];
}

bool isCorrectDate(string &date) {
    string day, month, year;
    if (date.empty() || date.size() != 10) {
        cerr << "Invalid date" << endl;
        return false;
    }
    for (int i = 0; i < date.size(); ++i) {
        if (i == 2 or i == 5) {
            if (date[i] != '/' and date[i] != '.') {
                cerr << "ERROR" << endl;
                return false;
            }
        } else if (!isdigit(date[i])) {
            cerr << "error" << endl;
            return false;
        }

        if (i == 0 || i == 1) {
            day += date[i];
        } else if (i == 3 || i == 4) {
            month += date[i];
        } else if (i > 5 && i < 10) {
            year += date[i];
        }

    }

    if (stoi(day) > daysInMonth(stoi(month), stoi(year)) || stoi(day) < 1) {
        cerr << "Invalid date" << endl;
        return false;
    }
    return true;
}

int main() {
    vector<Friend> friends;
    string name, birthday_str;
    tm birthday;

    while (true) {
        cout << "Enter the name (or 'end'): ";
        getline(cin, name);

        if (name == "end") {
            break;
        }

        while (true) {
            cout << "Enter the date of birthday (day/month/year): ";
            getline(cin, birthday_str);

            if (isCorrectDate(birthday_str)) {
                break;
            }
        }

        istringstream iss(birthday_str);
        iss >> get_time(&birthday, "%d/%m/%Y");

        Friend friendData;
        friendData.name = name;
        friendData.birthday = birthday;
        friends.push_back(friendData);
    }

    if (!friends.empty()) {
        tm current_date = getCurrentDate();

        for (auto &person: friends) {
            person.birthday.tm_year = current_date.tm_year;
            if (person.birthday.tm_mon < current_date.tm_mon)
                person.birthday.tm_year += 1;
            else if (person.birthday.tm_mon == current_date.tm_mon) {
                if (person.birthday.tm_mday < current_date.tm_mday)
                    person.birthday.tm_year += 1;
            }
        }

        vector<Friend> near_birthday;
        int ind;
        time_t tmp = time(nullptr);
        tm tmp_tm = *localtime(&tmp);
        tmp_tm.tm_year += 1;
        double diff_t = difftime(mktime(&tmp_tm), mktime(&current_date));
        for (int i = 0; i < friends.size(); i++) {
            time_t tt = time(nullptr);
            time_t bd_t = mktime(&friends[i].birthday);
            if (friends[i].birthday.tm_mon == current_date.tm_mon and
                friends[i].birthday.tm_mday == current_date.tm_mday) {
                cout << "\tToday " << put_time(&friends[i].birthday, "%d/%m") << " is " << friends[i].name
                     << "'s birthday"
                     << endl;
            } else if (difftime(bd_t, tt) < diff_t) {
                diff_t = difftime(bd_t, tt);
                ind = i;
            }
        }

        std::time_t nearest_t = std::mktime(&friends[ind].birthday);
        near_birthday.push_back(friends[ind]);
        for (int i = 0; i < friends.size(); i++) {
            std::time_t bd_t = std::mktime(&friends[i].birthday);
            if (i != ind and difftime(bd_t, nearest_t) == 0) {
                near_birthday.push_back(friends[i]);
            }
        }
        if (!near_birthday.empty()) {
            for (auto &person: near_birthday) {
                cout << "\tNearest birthday: " << person.name << ' ' << put_time(&person.birthday, "%d/%m") << endl;
            }
        } else {
            cout << "\tNo nearest birthdays!" << endl;
        }
    } else {
        cout << "\tYou didn't enter the data!" << endl;
    }

    return 0;
}