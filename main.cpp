#include <iostream>
#include "streamz.h"
using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;

    //TEST TO THE STATIC ID

    Date d;

    Streamer s("0000joa", d);
    Streamer s1("111josdaasfa", d);
    Streamer s2("222jaDSoa", d);
    Streamer s3("j3333oSDGDSGa", d);
    Viewer v0("V0000", d);
    Streamer s4("jo4444ASDa", d);
    Viewer v1("V111", d);

    cout << s.name << "  " << s.id << endl;
    cout << s1.name << "  " << s1.id << endl;
    cout << s2.name << "  " << s2.id << endl;
    cout << s3.name << "  " << s3.id << endl;
    cout << s4.name << "  " << s4.id << endl;
    cout << v0.name << "  " << v0.id << endl;
    cout << v1.name << "  " << v1.id << endl;

    return 0;
}
