// BOJ 1759 - 암호 만들기
// 난이도: Gold V
// 푼 날짜: 2023-09-14
// https://www.acmicpc.net/problem/1759

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;

vector<char> vs;
vector<char> cs;
vector<set<string>> vstrs;
vector<set<string>> cstrs;
set<string> ans;

void vtrack(string str, int last, int lim) {
    if (str.size() == lim) {
        vstrs[lim].insert(str);
        return;
    }
    
    for (int i = last + 1; i < (int) vs.size(); i++) {
        string next = str + vs[i];
        vtrack(next, i, lim);
    }
}

void ctrack(string str, int last, int lim) {
    if (str.size() == lim) {
        cstrs[lim].insert(str);
        return;
    }
    
    for (int i = last + 1; i < (int) cs.size(); i++) {
        string next = str + cs[i];
        ctrack(next, i, lim);
    }
}

int main() {
    fastio;
    
    int L, C;
    cin >> L >> C;
    
    vstrs = vector<set<string>>(15);
    cstrs = vector<set<string>>(15);
    
    for (int i = 0; i < C; i++) {
        char c;
        cin >> c;
        
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') vs.push_back(c);
        else cs.push_back(c);
    }
    
    for (int i = 1; i <= (int) vs.size(); i++) {
        if (L - i >= 2 && L - i <= (int) cs.size()) {
            string em;
            vtrack(em, -1, i);
            ctrack(em, -1, L - i);
            
            for (string s: vstrs[i]) {
                for (string t: cstrs[L - i]) {
                    string k = s + t;
                    sort(k.begin(), k.end());
                    
                    ans.insert(k);
                }
            }
        }
    }

    for (string s: ans) {
        cout << s << "\n";
    }
    
    return 0;
}
