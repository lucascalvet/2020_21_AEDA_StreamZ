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
    time_t current_time;
    time(&current_time);
    tm *date = localtime(&current_time);
    if (date->tm_mon + 1 > birthday.month || date->tm_mon + 1 == birthday.month && date->tm_mday >= birthday.day)
        return date->tm_year - birthday.year;
    return date->tm_year - birthday.year - 1;
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