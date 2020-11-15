#include "Streamer.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Streamer::Streamer(const std::string &nickname, const std::string &name, const Date &birth_date, Platform & platform) : User(nickname, name, birth_date, platform) {
    if (getAge() <= MINIMUM_STREAMER_AGE)
        throw InvalidAge(getAge());
}

Streamer::Streamer(const std::string &nickname, const std::string &name, const Date &birth_date, Platform & platform, const std::vector<unsigned int> &streams_history) : User(nickname, name, birth_date, platform) {
    if (getAge() <= MINIMUM_STREAMER_AGE)
        throw InvalidAge(getAge());
    this->streams_history = streams_history;
}

Streamer::Streamer(const std::string &nickname, const std::string &name, const Date &birth_date, Platform & platform, const std::vector<unsigned int> &streams_history, const std::weak_ptr<Stream> &current_stream) : User(nickname, name, birth_date, platform) {
    if (getAge() <= MINIMUM_STREAMER_AGE)
        throw InvalidAge(getAge());
    this->streams_history = streams_history;
    this->current_stream = current_stream;
}

std::vector<unsigned int> Streamer::getStreamsHistory() const {
    return streams_history;
}

void Streamer::startPublicStream(const string &title, const string &language, const unsigned minimum_age) {
    if (!current_stream.expired())
        throw InvalidAction("Stream already occurring");
    current_stream = platform.startPublicStream(title, getNickname(), language, minimum_age);
}

void Streamer::startPrivateStream(const string &title, const string &language, const unsigned minimum_age, const unsigned max_capacity, const vector<string> &allowed_viewers) {
    if (!current_stream.expired())
        throw InvalidAction("Stream already occurring");
    current_stream = platform.startPrivateStream(title, getNickname(), language, minimum_age, max_capacity, allowed_viewers);
}

void Streamer::endStream() {
    if (current_stream.expired())
        throw InvalidAction("No stream is occurring");
    unsigned int id = current_stream.lock()->getId();
    current_stream.lock()->endStream();
    platform.endStream(id);
    streams_history.push_back(id);
    current_stream.reset();
}

void Streamer::removeStream(const unsigned int id) {
    unsigned int left = 0, right = streams_history.size() - 1, middle;
    while (left <= right) {
        middle = (left + right) / 2;
        if (streams_history.at(middle) < id)
            left = middle + 1;
        else if (id < streams_history.at(middle))
            right = middle - 1;
        else {
            streams_history.erase(streams_history.begin() + middle);
            return;
        }
    }
    throw StreamDoesNotExist(id);
}

void Streamer::show() const {
    std::cout << std::left << std::setw(10) << "Streamer";
    User::show();
}

std::ostream& Streamer::print(std::ostream & os) const {
    os << "(streamer) ";
    User::print(os);
    os << "    history: ";
    for (unsigned int id : streams_history)
        os << id << " ";
    os << std::endl;
    return os;
}

bool Streamer::operator==(const Streamer &other) const {
    return this->getNickname() == other.getNickname();
}