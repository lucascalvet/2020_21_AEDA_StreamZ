#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "utils.h"

class InvalidLanguage{
public:
    Language lang;
    explicit InvalidLanguage(Language lang) {this->lang = lang;}
};

enum DateField {
    DAY, MONTH, YEAR
};

class InvalidDate{
public:
    DateField field;
    unsigned value;
    InvalidDate(DateField field, unsigned value) {this->field = field; this->value = value;}
};


#endif //EXCEPTIONS_H
