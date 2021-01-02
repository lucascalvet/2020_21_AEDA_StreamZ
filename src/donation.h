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
    std::string getRecipient() const;
    unsigned getAmount() const;
    unsigned getEvaluation() const;
    void setRecipient(const std::string &nick);
    void setAmount(unsigned amnt) {amount = amnt;}
    void setEvaluation(unsigned eval);
    bool operator<(const Donation &donation) const;
    bool operator==(const Donation &donation) const;
};


#endif //STREAMZ_DONATION_H
