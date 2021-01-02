#include "donation.h"
#include <stdexcept>
#include <sstream>

using namespace std;

/**
 * Constructor for donation
 * @param nick the recipient's (streamer) nickname
 * @param amnt the donation amount
 * @param eval the evaluation of the streamer
 */
Donation::Donation(string nick, unsigned amnt, unsigned eval) {
    if (eval < 1 || eval > 5) throw out_of_range("The evaluation must be between 1 and 5!");
    recipient_nickname = nick;
    amount = amnt;
    evaluation = eval;
}

/**
 * Get the donation's information
 *
 * @return a string with the donation's information
 */
string Donation::getInfo() const {
    ostringstream info;
    info << "Recipient: " << recipient_nickname << "\tAmount: " << amount << "\tEvaluation: " << evaluation << "\n";
    return info.str();
}

/**
 * Get the donation's recipient
 *
 * @return the donation's recipient
 */
std::string Donation::getRecipient() const {
    return recipient_nickname;
}

/**
 * Get the donation's amount
 *
 * @return the donation's amount
 */
unsigned Donation::getAmount() const {
    return amount;
}

/**
 * Get the streamer's evaluation
 *
 * @return the streamer's evaluation
 */
unsigned Donation::getEvaluation() const {
    return evaluation;
}

/**
 * Set the donation's recipient
 *
 * @param nick the new recipient
 */
void Donation::setRecipient(const string &nick) {
    recipient_nickname = nick;
}

/**
 * Set the streamer's evaluation
 *
 * @param eval the new evaluation
 */
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
