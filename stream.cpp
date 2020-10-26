//
// Created by sergi on 26/10/2020.
//
#include "stream.h"

Stream::Stream(string title, Language lang) {
    this->title = title;
    this->lang = lang;
}

Stream::~Stream() {
}

PublicStream::PublicStream(string title, Language lang) : Stream( title, lang){
}

PublicStream::~PublicStream(){
}

PrivateStream::PrivateStream(string title, Language lang) : Stream(title, lang){
}

PrivateStream::~PrivateStream() {
}
