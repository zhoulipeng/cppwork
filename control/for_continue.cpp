#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    int i = 0;
    for (i = 0; i < 10; i++) {
        cout << "continue" << endl;
        continue;// continue 会跳到i++, 不是 i < x 这个位置
        cout << "not print" << endl;
    }

}
