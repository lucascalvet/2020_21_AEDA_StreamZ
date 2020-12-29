#ifndef STREAMZ_DONATION_H
#define STREAMZ_DONATION_H

#include <string>

class Donation {
private:
    std::string recipient_nickname;
    unsigned amount;
    unsigned evaluation;
public:
    Donation(std::string nick, unsigned amnt, unsigned eval);
    std::string getInfo() const;
    std::string getRecipient() const {return recipient_nickname;}
    unsigned getAmount() const {return amount;}
    unsigned getEvaluation() const {return evaluation;}
    void setRecipient(const std::string &nick) {recipient_nickname = nick;}
    void setAmount(unsigned amnt) {amount = amnt;}
    void setEvaluation(unsigned eval);
    bool operator<(const Donation &donation) const;
    bool operator==(const Donation &donation) const;
};


#endif //STREAMZ_DONATION_H
