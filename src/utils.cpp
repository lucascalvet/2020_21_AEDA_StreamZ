#include "utils.h"
#include "exceptions.h"
#include <ctime>

using namespace std;

/**
 * Constructor for the Date struct
 *
 * @param day the date's day
 * @param month the date's month
 * @param year the date's year
 */
Date::Date(unsigned int day, unsigned int month, unsigned int year) {
    if (month > 12)
        throw InvalidDate(MONTH, month);
    if (day > 31)
        throw InvalidDate(DAY, day);

    this->day = day;
    this->month = month;
    this->year = year;
}

/**
 * Implementation of the operator<< for printing a date in a legible format.
 *
 * @param out the output stream for printing the date
 * @param date the date to be printed
 * @return a reference to the output stream
 */
std::ostream &operator<<(std::ostream &out, const Date &date) {
    out << date.day << "/" << date.month << "/" << date.year;
    return out;
}

/**
 * Implementation of the operator<
 *
 * @param date1 the date object to compare
 * @param date2 the date object to compare with date1
 * @return true if date1 < date2, false otherwise
 */
bool operator<(const Date &date1, const Date &date2){
    if(date1.year < date2.year) return true;
    if(date1.year == date2.year){
        if(date1.month < date2.month) return true;
        if(date1.month == date2.month && date1.day < date2.day) return true;
    }
    return false;
}

/**
 * Helper function for turning all characters in a string to uppercase
 *
 * @param str the input string
 * @return the input string with all of its characters in uppercase
 */
string strToUpper(const string &str) {
    string::const_iterator it;
    string ret_str;
    for (it = str.begin(); it != str.end(); it++)
        ret_str.push_back(toupper(*it));
    return ret_str;
}

/**
 * Calculate the age of someone born on "birthday", relative to the current day
 * @param birthday the person's date of birth
 * @return the age of the person
 */
Age calculateAge(const Date &birthday) {
    Date current_date = getCurrentDate();
    if (current_date.month > birthday.month || current_date.month == birthday.month && current_date.day >= birthday.day)
        return current_date.year - birthday.year;
    return current_date.year - birthday.year - 1;
}

/**
 * Gets the current date
 * @return a Date object with the current date
 */
Date getCurrentDate() {
    time_t current_time;
    time(&current_time);
    tm *date = localtime(&current_time);
    return Date(date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
}