#include <iostream>
#include "streamz.h"
using namespace std;

int main() {
    /* TEST STREAM INITIALISATION
    string title;
    Language lang;
    Age min_age;
    cout << "Input the stream's title: ";
    getline(cin, title);
    cout << "Input the stream's language: "; cin >> lang;
    cout << "Input the stream's minimum age: "; cin >> min_age;
    try {
        Stream s1(title, lang, min_age);
        cout << "Title: " << s1.getTitle() << "\tLanguage: " << s1.getLanguage() <<
        "\tMinimum age: " << s1.getMinAge() << "\tStarting date: " << s1.getDate();
    }
    catch(Stream::InvalidLanguage& il) {cout << "\"" << il.lang << "\" is an invalid language!";}
*/


    //TEST TO THE STATIC ID

    Date d;

    Streamer s("0000joa", d);
    Streamer s1("111josdaasfa", d);
    Streamer s2("222jaDSoa", d);
    Streamer s3("j3333oSDGDSGa", d);
    Viewer v0("V0000", d);
    Streamer s4("jo4444ASDa", d);
    Viewer v1("V111", d);

    cout << s.getName() << "  " << s.getID() << endl;
    cout << s1.getName() << "  " << s1.getID() << endl;
    cout << s2.getName() << "  " << s2.getID() << endl;
    cout << s3.getName() << "  " << s3.getID() << endl;
    cout << s4.getName() << "  " << s4.getID() << endl;
    cout << v0.getName() << "  " << v0.getID() << endl;
    cout << v1.getName() << "  " << v1.getID() << endl;

    return 0;
}
