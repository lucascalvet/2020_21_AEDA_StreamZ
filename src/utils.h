#ifndef STREAMZ_UTILS_H
#define STREAMZ_UTILS_H

#include <iostream>
#include <string>

#define MIN_AGE_STREAMER 15
#define MIN_AGE_VIEWER 12
#define VALIDATION_STRING "STREAMZ_FORMATTED_FILE"

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

Age calculateAge(const Date& birthday);

Date getCurrentDate();

bool operator==(const Date &date1, const Date &date2);

bool operator<(const Date &date1, const Date &date2);

bool operator<=(const Date &date1, const Date &date2);

std::ostream& operator<< (std::ostream& out, const Date& date);

std::string strToUpper(const std::string& str);

bool passwordStrength(const std::string& password, std::string &strength);

bool sha256Verifier(const std::string& hash,const std::string& password);

std::string sha256Encode(const std::string& password);

#endif //STREAMZ_UTILS_H
