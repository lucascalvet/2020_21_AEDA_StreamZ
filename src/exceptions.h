#ifndef STREAMZ_EXCEPTIONS_H
#define STREAMZ_EXCEPTIONS_H

#include "utils.h"

class InvalidLanguage : std::exception {
private:
    Language lang;
public:
    InvalidLanguage(const Language &lang) { this->lang = lang; }
    Language getLanguage() const { return lang; }
};

enum DateField {
    DAY, MONTH, YEAR
};

class InvalidDate : std::exception {
private:
    DateField field;
    unsigned value;
public:
    InvalidDate(DateField field, unsigned value) {
        this->field = field;
        this->value = value;
    }
    DateField getField() const { return field; }
    unsigned getValue() const { return value; }
};

class InvalidFile : std::exception {
private:
    std::string filename;
public:
    explicit InvalidFile(const std::string &filename) { this->filename = filename; }
    std::string getFileName() const { return filename; }
};

class AlreadyViewing : std::exception {};

class AlreadyStreaming : std::exception {};

class AlreadyInteracted : std::exception {};;

class HasNotInteracted : std::exception {};;

class FullCapacity : std::exception{};

class InactiveUser : std::exception{};

class UnauthorizedViewer : std::exception{};

class NotInPrivateStream : std::exception{};

class NoMinimumAge : std::exception{};

class NameAlreadyInUse : std::exception{};

class OrderAlreadyExists : std::exception{};

class OrderDoesNotExist : std::exception{};

class ExceededMaxQuantityPerPurchase : std::exception{};

class InvalidPriority : std::exception{};

class InactiveAccount : std::exception{};

class ProductNotFound : std::exception{};

class QuantityOverTheStock : std::exception{};

class NotEnoughCapital : std::exception{};

#endif //STREAMZ_EXCEPTIONS_H
