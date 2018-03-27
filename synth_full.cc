#include <iostream>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

int main() {
    string entry;
    cin >> entry;

    for(int i = 0; i < entry.length; i++) {
        cout << entry[i] << '\n';
    }

    return 0;
}