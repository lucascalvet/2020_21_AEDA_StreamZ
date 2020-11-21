#include <ctime>
#include <string>
#include "utils.h"
#include "exceptions.h"
#include "../hashlib++/hashlibpp.h"

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
 * Implementation of the operator==
 *
 * @param date1 the first date object to compare
 * @param date2 the second date object to compare
 * @return true if date1 == date2, false otherwise
 */
bool operator==(const Date &date1, const Date &date2) {
    return date1.day == date2.day && date1.month == date2.month && date1.year == date2.year;
}

/**
 * Implementation of the operator<
 *
 * @param date1 the first date object to compare
 * @param date2 the second date object to compare
 * @return true if date1 < date2, false otherwise
 */
bool operator<(const Date &date1, const Date &date2) {
    if (date1.year < date2.year) return true;
    if (date1.year == date2.year) {
        if (date1.month < date2.month) return true;
        if (date1.month == date2.month && date1.day < date2.day) return true;
    }
    return false;
}

/**
 * Implementation of the operator<=
 *
 * @param date1 the first date object to compare
 * @param date2 the second date object to compare
 * @return true if date1 <= date2, false otherwise
 */
bool operator<=(const Date &date1, const Date &date2) {
    return date1 < date2 || date1 == date2;
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

/**
 * Verifies a password strength
 * @param password the password inputted by user
 * @param strength the strength of the password
 * @return true if password is strong or moderate, false otherwise
 */
bool passwordStrength(const string &password, string &strength) {
    bool has_lower = false, has_upper = false;
    bool has_digit = false, special_char = false;

    string normal_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";

    for (int i = 0; i < password.size(); i++) {
        if (islower(password[i]))
            has_lower = true;
        if (isupper(password[i]))
            has_upper = true;
        if (isdigit(password[i]))
            has_digit = true;
    }

    size_t special = password.find_first_not_of(
            normal_chars); // finds first character in password which is not present in normal_chars

    if (special != string::npos)
        special_char = true;

    if (has_lower && has_upper && has_digit && special_char && (password.size() >= 8))
        strength = "strong";
    else if (has_lower && has_upper && (password.size() >= 6))
        strength = "moderate";
    else
        strength = "weak";

    if (strength == "strong" || strength == "moderate") return true;
    else return false;
}

/**
 * Encodes a string using sha256 algorithm
 *
 * It uses the hashlib++ free library, that implements the sha256
 *
 * @param password the password inputted by user
 * @return hashed the encrypted string
 */
string sha256Encode(const string &password) {
    hashwrapper *sha_hasher = new sha256wrapper(); //wrapper object

    std::string hashed = sha_hasher->getHashFromString(password); //creating hash for string password

    delete sha_hasher;

    return hashed;
}

/**
 * Verifies two strings are the same using sha256 algorithm
 * @param hash the string that is already encrypted
 * @param password the string inputted by user
 * @return true if hash and password correspond
 */
bool sha256Verifier(const string &hash, const string &password) {
    if (hash == sha256Encode(password)) return true;

    else return false;
}