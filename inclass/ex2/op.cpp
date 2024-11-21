#include <iostream>
using namespace std;

extern "C" {
    int asmtest(int,int);

}


int main() {
    cout << asmtest << endl;
}