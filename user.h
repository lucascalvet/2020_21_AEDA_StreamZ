#ifndef USER_H
#define USER_H

#include "streamz.h"

class User {
    string nickname;
    Date birthday;
};

class Streamer : public User {
    vector<Stream> streams;
};

class Viewer : public User {
    Stream* viewer_stream;
};

#endif //STREAMZ_AEDA2020_21_USER_H
