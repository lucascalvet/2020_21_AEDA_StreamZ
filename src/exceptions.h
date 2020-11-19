#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "utils.h"

class InvalidLanguage{
private:
    Language lang;
public:
    InvalidLanguage(const Language& lang) {this->lang = lang;}
    Language getLanguage() const {return lang;}
};

enum DateField {
    DAY, MONTH, YEAR
};

class InvalidDate{
private:
    DateField field;
    unsigned value;
public:
    InvalidDate(DateField field, unsigned value) {this->field = field; this->value = value;}
    DateField getField() const {return field;}
    unsigned getValue() const {return value;}
};

class InvalidFile{
private:
    std::string filename;
public:
    InvalidFile(const std::string& filename) {this->filename = filename;}
    std::string getFileName() const {return filename;}
};

class UserNotFound{
private:
public:
};

#endif //EXCEPTIONS_H
