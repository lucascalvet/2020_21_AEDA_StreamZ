#include "utils.h"

using namespace std;

/**
 * Constructor for the Date struct
 *
 * @param day the date's day
 * @param month the date's month
 * @param year the date's year
 */
Date::Date(unsigned int day, unsigned int month, unsigned int year){
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
std::ostream& operator<< (std::ostream& out, const Date& date) {
    out << date.day << "/" << date.month << "/" << date.year;
    return out;
}

/**
 * Helper function for turning all characters in a string to uppercase
 *
 * @param str the input string
 * @return the input string with all of its characters in uppercase
 */
string strToUpper(const string& str) {
    string::const_iterator it;
    string ret_str;
    for (it = str.begin(); it != str.end(); it++)
        ret_str.push_back(toupper(*it));
    return ret_str;
}