#include "donation.h"
#include <stdexcept>
#include <sstream>

using namespace std;

Donation::Donation(string nick, unsigned amnt, unsigned eval) {
    if (eval < 1 || eval > 5) throw out_of_range("The evaluation must be between 1 and 5!");
    recipient_nickname = nick;
    amount = amnt;
    evaluation = eval;
}

string Donation::getInfo() const {
    ostringstream info;
    info << "Recipient: " << recipient_nickname << "\tAmount: " << amount << "\tEvaluation: " << evaluation << "\n";
    return info.str();
}

void Donation::setEvaluation(unsigned eval) {
    if (eval < 1 || eval > 5) throw out_of_range("The evaluation must be between 1 and 5!");
    evaluation = eval;
}

bool Donation::operator<(const Donation &donation) const {
    if (amount != donation.amount) return amount > donation.amount;
    if (evaluation != donation.evaluation) return evaluation > donation.evaluation;
    return recipient_nickname > donation.recipient_nickname;
}

bool Donation::operator==(const Donation &donation) const {
    return recipient_nickname == donation.recipient_nickname && amount == donation.amount &&
            evaluation == donation.evaluation;
}
