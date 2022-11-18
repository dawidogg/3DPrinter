#include <bits/stdc++.h>

using namespace std;

struct {
    int speed;
    bool moving;
    void incSpeed() {
        speed++;
    }
} myObject;

int main() {
    myObject = {50, true};
    cout << myObject.speed << " " << myObject.moving << endl;
    myObject.incSpeed();
    cout << myObject.speed << " " << myObject.moving << endl;
    
    return 0;
}