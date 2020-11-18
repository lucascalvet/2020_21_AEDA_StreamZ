#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

#define MIN_AGE_STREAMER 12
#define MIN_AGE_VIEWER 15

/// Unsigned type for representing an age
typedef unsigned Age;
/// String type for representing a language, in the predefined format
typedef std::string Language;

/**
 * Combination of the day, month and year of a date
 */
struct Date {
    Date() = default;
    Date(unsigned day, unsigned month, unsigned year);
    unsigned day;
    unsigned month;
    unsigned year;
};

std::ostream& operator<< (std::ostream& out, const Date& date);

std::string strToUpper(const std::string& str);

Age calculateAge(const Date& birthday);

Date getCurrentDate();

#endif //UTILS_H
