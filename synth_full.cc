#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

class BNFGrammar {
        int variables;
        vector< vector<string> > rules;

    public:
        BNFGrammar (int);
        void add(int i, string s) {
            if(i >= variables) {
                cout << "error: variable " << i << " does not exist\n==> not adding rule to variable\n";
                return;
            }

            if(rules[i].size() > 0) {
                rules[i].push_back(s);
            } else {
                vector<string> r(1, s);
                rules[i] = r;
            }
        }
        void print() { 
            for(int i=0; i<rules.size(); i++) {
                cout << "rules for " << i << '\n';
                for(int j=0; j<rules[i].size(); j++) {
                    cout << "\t" << rules[i][j] << '\n';
                }
            }
        }
};

BNFGrammar::BNFGrammar (int v) {
    variables = v;
    rules.resize(v);
}   

int main() {
    string entry;
    
    BNFGrammar bigboss(5);
    bigboss.add(0, "a |1| |2|");
    bigboss.add(1, "b bas");
    bigboss.add(1, "big |2| boss");
    bigboss.add(2, "");
    bigboss.add(2, "c");
    bigboss.add(6, "c");

    bigboss.print();

    return 0;
}