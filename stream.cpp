#include "stream.h"

const vector<Language> Stream::langs = {"AB", "AA", "AF", "AK", "SQ", "AM", "AR", "AN", "HY", "AS", "AV", "AE", "AY", "AZ",
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


Stream::Stream(const string& title, const Language& lang, unsigned min_age) {
    this->title = title;
    string lang_upper = strToUpper(lang);
    if(find(langs.begin(), langs.end(), lang_upper) == langs.end())
        throw InvalidLanguage(lang);
    this->lang = lang_upper;
    this->min_age = min_age;
    time_t current_time;
    time(&current_time);
    tm* date = localtime(&current_time);
    this->starting_date.day = date->tm_mday;
    this->starting_date.month = date->tm_mon + 1;
    this->starting_date.year = date->tm_year + 1900;
}

string Stream::strToUpper(const string& str) {
    string::const_iterator it;
    string ret_str;
    for (it = str.begin(); it != str.end(); it++)
        ret_str.push_back(toupper(*it));
    return ret_str;
}

string Stream::getTitle() const{
    return this->title;
}

Date Stream::getDate() const{
    return this->starting_date;
}

Language Stream::getLanguage() const{
    return this->lang;
}

Age Stream::getMinAge() const {
    return this->min_age;
}

string Stream::getInfo() const {
    ostringstream info
    return std::__cxx11::string();
}


PublicStream::PublicStream(string title, Language lang, Age min_age) : Stream( title, lang, min_age){
}

PublicStream::~PublicStream(){
}

PrivateStream::PrivateStream(string title, Language lang, Age min_age) : Stream(title, lang, min_age){
}

PrivateStream::~PrivateStream() {
}
