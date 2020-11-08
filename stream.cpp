#include "stream.h"

using namespace std;

/// All the possible languages a stream may have
const vector<Language> Stream::LANGS = {"AB", "AA", "AF", "AK", "SQ", "AM", "AR", "AN", "HY", "AS", "AV", "AE", "AY", "AZ",
                          "BM", "BA", "EU", "BE", "BN", "BH", "BI", "BS", "BR", "BG", "MY", "CA", "CH", "CE",
                          "NY", "ZH", "CV", "KW", "CO", "CR", "HR", "CS", "DA", "DV", "NL", "DZ", "EN", "EO",
                          "ET", "EE", "FO", "FJ", "FI", "FR", "FF", "GL", "KA", "DE", "EL", "GN", "GU", "HT",
                          "HA", "HE", "HZ", "HI", "HO", "HU", "IA", "ID", "IE", "GA", "IG", "IK", "IO", "IS",
                          "IT", "IU", "JA", "JV", "KL", "KN", "KR", "KS", "KK", "KM", "KI", "RW", "KY", "KV",
                          "KG", "KO", "KU", "KJ", "LA", "LB", "LG", "LI", "LN", "LO", "LT", "LU", "LV", "GV",
                          "MK", "MG", "MS", "ML", "MT", "MI", "MR", "MH", "MN", "NA", "NV", "ND", "NE", "NG",
                          "NB", "NN", "NO", "II", "NR", "OC", "OJ", "CU", "OM", "OR", "OS", "PA", "PI", "FA",
                          "PL", "PS", "PT", "QU", "RM", "RN", "RO", "RU", "SA", "SC", "SD", "SE", "SM", "SG",
                          "SR", "GD", "SN", "SI", "SK", "SL", "SO", "ST", "ES", "SU", "SW", "SS", "SV", "TA",
                          "TE", "TG", "TH", "TI", "BO", "TK", "TL", "TN", "TO", "TR", "TS", "TT", "TW", "TY",
                          "UG", "UK", "UR", "UZ", "VE", "VI", "VO", "WA", "CY", "WO", "FY", "XH", "YI", "YO",
                          "ZA", "ZU"};

/**
 * Constructs a stream object
 *
 * @param title the stream´s title
 * @param lang the stream's language (must be one of the languages in LANG)
 * @param min_age the stream's minimum age
 */
Stream::Stream(const string& title, const Language& lang, unsigned min_age) {
    this->title = title;
    string lang_upper = strToUpper(lang);
    if(find(LANGS.begin(), LANGS.end(), lang_upper) == LANGS.end())
        throw InvalidLanguage(lang);
    this->lang = lang_upper;
    this->min_age = min_age;
    time_t current_time;
    time(&current_time);
    tm* date = localtime(&current_time);
    starting_date = Date(date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
}

/**
 * Gets the stream's title
 *
 * @return a string with the stream's title
 */
string Stream::getTitle() const{
    return this->title;
}

/**
 * Gets the stream's starting date
 *
 * @return a Date object with the stream's starting date
 */
Date Stream::getDate() const{
    return this->starting_date;
}

/**
 * Gets the stream's language
 *
 * @return the stream's language in the predefined language initials
 */
Language Stream::getLanguage() const{
    return this->lang;
}

/**
 * Gets the stream's minimum viewing age
 * @return the stream's minimum viewing age
 */
Age Stream::getMinAge() const {
    return this->min_age;
}

void Stream::addLike() {
    likes++;
}

void Stream::addDislike() {
    dislikes++;
}

void Stream::remLike() {
    likes--;
}

void Stream::remDislike() {
    dislikes--;
}

/**
 * Gets a formatted string with the information of a stream (its title, starting date, language and minimum age
 *
 * @return a string with the stream's information
 */
string Stream::getInfo() const {
    ostringstream info;
    info << "Title: " << this->title << "\tStarting Date: " << this->starting_date <<
    "\tLanguage: " << this->lang << "\tMin Age: " << this->min_age;
    return info.str();
}

/**
 * Constructs a public stream
 *
 * Just calls the constructor for a stream, since it doesn't have unique methods or attributes
 *
 * @param title the stream's title
 * @param lang the stream's language (must be one of the languages in LANG)
 * @param min_age the stream's minimum age
 */
PublicStream::PublicStream(string title, Language lang, Age min_age) : Stream( title, lang, min_age){
}

PublicStream::~PublicStream(){
}

/**
 * Constructs a private stream
 *
 * @param title the stream's title
 * @param lang the stream's language (must be one of the languages in LANG)
 * @param min_age the stream's minimum age
 */
PrivateStream::PrivateStream(string title, Language lang, Age min_age) : Stream(title, lang, min_age){
}

PrivateStream::~PrivateStream() {
}
