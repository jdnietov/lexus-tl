#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

class BNFGrammar {
    private:
        int variables;
        vector< vector<string> > rules;
        
        static int stov(string str, int idx) {
            int i = idx;
            while(str[i] != '|')
                i++;
            
            return stoi(str.substr(idx, i-1));
        }

        vector<string> first(string str) {
            cout << "string examined: " << str << '\n';
            vector<string> res;

            if(str[0] != '|') { // terminal
                if(str.find(" ") == string::npos)
                    res.push_back(str);
                else 
                    res.push_back(str.substr(0, str.find(" ")));
            }

            // if(str.size() == 0) {
            //     res.push_back("#");
            // }

            if(str[0] == '|') { // variable
                int var = stov(str, 1);
                vector<string> var_rules = this->rules[var];

                for(int i=0; i<var_rules.size(); i++) {
                    vector<string> rule_res = first(var_rules[i]);
                    for(int j=0; j<rule_res.size(); j++) {
                        string rule = rule_res[j];

                        int leap = to_string(var).length()+3; // +3: |, |, space
                        if(rule == "#" && str.length() > leap) {
                            vector<string> next_res = first(str.substr(leap));    // TODO: come on man.
                            res.insert( res.end(), next_res.begin(), next_res.end() );
                        }

                        if(find(res.begin(), res.end(), rule) == res.end())
                            res.push_back(rule);
                    }
                }

            }
            return res;
        }

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

        void print_first(string s) {
            vector<string> first = this->first(s);

            cout << "*** first of " << s << '\n';
            for(int i=0; i<first.size(); i++) {
                cout << '\t' << first[i] << '\n';
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
    bigboss.add(0, "|1| uno |2| |3|");
    bigboss.add(0, "|0| dos");
    bigboss.add(1, "|2| |3| |4|");
    bigboss.add(1, "|1| tres");
    bigboss.add(1, "#");
    bigboss.add(2, "|4| cuatro |3| tres");
    bigboss.add(2, "#");
    bigboss.add(3, "cinco |4| |2|");
    bigboss.add(3, "#");
    bigboss.add(4, "seis");
    bigboss.add(4, "#");

    while( getline(cin, entry) ) {
        bigboss.print_first(entry);
    }

    return 0;
}