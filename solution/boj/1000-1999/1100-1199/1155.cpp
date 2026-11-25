// BOJ 1155 - 변형 하노이
// 난이도: Platinum IV
// 푼 날짜: 2023-10-30
// https://www.acmicpc.net/problem/1155

#include <bits/stdc++.h>

#define fastio ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);

using namespace std;
typedef long long ll;
typedef __int128 xll;

vector<string> pw2;
vector<string> half;

set<string> pow2s = {
    "ABACBCBACACB",
"ABACBCCABACB",
"ABACBCCACBBA",
"ABACCABCBACB",
"ABACCABCCBBA",
"ABACCACBBCBA",
"ABBCACBACACB",
"ABBCACCABACB",
"ABBCACCACBBA",
"ABBCBAACCACB",
"ABBCBACAACCB",
"ABBCBACACBAC",
"ABBCCAACBACB",
"ABBCCAACCBBA",
"ABBCCABAACCB",
"ABBCCABACBAC",
"ABBCCACBACBA",
"ABBCCACBBAAC",
"ABCAACBCBACB",
"ABCAACBCCBBA",
"ABCAACCBBCBA",
"ABCABCACBACB",
"ABCABCACCBBA",
"ABCABCBAACCB",
"ABCABCBACBAC",
"ABCABCCBACBA",
"ABCABCCBBAAC",
"ABCACBACBCBA",
"ABCACBBCACBA",
"ABCACBBCBAAC",
"ACABBABCCBCA",
"ACABBACBBCCA",
"ACABBACBCABC",
"ACABCBBABCCA",
"ACABCBBACABC",
"ACABCBCABABC",
"ACBAABBCCBCA",
"ACBAABCBBCCA",
"ACBAABCBCABC",
"ACBABCABCBCA",
"ACBABCCBABCA",
"ACBABCCBCAAB",
"ACBACBABBCCA",
"ACBACBABCABC",
"ACBACBBCABCA",
"ACBACBBCCAAB",
"ACBACBCAABBC",
"ACBACBCABCAB",
"ACCBABBABCCA",
"ACCBABBACABC",
"ACCBABCABABC",
"ACCBBAABBCCA",
"ACCBBAABCABC",
"ACCBBABCABCA",
"ACCBBABCCAAB",
"ACCBBACAABBC",
"ACCBBACABCAB",
"ACCBCAABBABC",
"ACCBCABAABBC",
"ACCBCABABCAB",
"BAACABBCCBCA",
"BAACABCBBCCA",
"BAACABCBCABC",
"BAACBCABCBCA",
"BAACBCCBABCA",
"BAACBCCBCAAB",
"BAACCBABBCCA",
"BAACCBABCABC",
"BAACCBBCABCA",
"BAACCBBCCAAB",
"BAACCBCAABBC",
"BAACCBCABCAB",
"BABCACABCBCA",
"BABCACCBABCA",
"BABCACCBCAAB",
"BABCCBACABCA",
"BABCCBACCAAB",
"BABCCBCAACAB",
"BACBACABBCCA",
"BACBACABCABC",
"BACBACBCABCA",
"BACBACBCCAAB",
"BACBACCAABBC",
"BACBACCABCAB",
"BACBBCACABCA",
"BACBBCACCAAB",
"BACBBCCAACAB",
"BACBCAACABBC",
"BACBCAACBCAB",
"BACBCABCACAB",
"BCABACBACACB",
"BCABACCABACB",
"BCABACCACBBA",
"BCABBAACCACB",
"BCABBACAACCB",
"BCABBACACBAC",
"BCABCAACBACB",
"BCABCAACCBBA",
"BCABCABAACCB",
"BCABCABACBAC",
"BCABCACBACBA",
"BCABCACBBAAC",
"BCBAABACCACB",
"BCBAABCAACCB",
"BCBAABCACBAC",
"BCBACAABACCB",
"BCBACAABCBAC",
"BCBACACBABAC",
"BCCAABACBACB",
"BCCAABACCBBA",
"BCCAABBAACCB",
"BCCAABBACBAC",
"BCCAABCBACBA",
"BCCAABCBBAAC",
"BCCABAABACCB",
"BCCABAABCBAC",
"BCCABACBABAC",
"BCCACBABACBA",
"BCCACBABBAAC",
"BCCACBBAABAC",
"CAABACBCBACB",
"CAABACBCCBBA",
"CAABACCBBCBA",
"CAABBCACBACB",
"CAABBCACCBBA",
"CAABBCBAACCB",
"CAABBCBACBAC",
"CAABBCCBACBA",
"CAABBCCBBAAC",
"CAABCBACBCBA",
"CAABCBBCACBA",
"CAABCBBCBAAC",
"CABCABACBACB",
"CABCABACCBBA",
"CABCABBAACCB",
"CABCABBACBAC",
"CABCABCBACBA",
"CABCABCBBAAC",
"CABCBAABACCB",
"CABCBAABCBAC",
"CABCBACBABAC",
"CABCCBABACBA",
"CABCCBABBAAC",
"CABCCBBAABAC",
"CACBABACBCBA",
"CACBABBCACBA",
"CACBABBCBAAC",
"CACBBCABACBA",
"CACBBCABBAAC",
"CACBBCBAABAC",
"CBACABBABCCA",
"CBACABBACABC",
"CBACABCABABC",
"CBACBAABBCCA",
"CBACBAABCABC",
"CBACBABCABCA",
"CBACBABCCAAB",
"CBACBACAABBC",
"CBACBACABCAB",
"CBACCAABBABC",
"CBACCABAABBC",
"CBACCABABCAB",
"CBBAACABBCCA",
"CBBAACABCABC",
"CBBAACBCABCA",
"CBBAACBCCAAB",
"CBBAACCAABBC",
"CBBAACCABCAB",
"CBBABCACABCA",
"CBBABCACCAAB",
"CBBABCCAACAB",
"CBBACAACABBC",
"CBBACAACBCAB",
"CBBACABCACAB",
"CBCAACABBABC",
"CBCAACBAABBC",
"CBCAACBABCAB",
"CBCABAACABBC",
"CBCABAACBCAB",
"CBCABABCACAB"
};

set<string> halves = {
    "ABACBCBACBCA",
"ABACBCCBBACA",
"ABACBCCBCABA",
"ABACCBBCBACA",
"ABACCBBCCABA",
"ABACCBCABCBA",
"ABBCACBACBCA",
"ABBCACCBBACA",
"ABBCACCBCABA",
"ABBCBAACCBCA",
"ABBCBACBACCA",
"ABBCBACBCAAC",
"ABBCCBACBACA",
"ABBCCBACCABA",
"ABBCCBBAACCA",
"ABBCCBBACAAC",
"ABBCCBCAACBA",
"ABBCCBCABAAC",
"ABCBACBCBACA",
"ABCBACBCCABA",
"ABCBACCABCBA",
"ABCBBCACBACA",
"ABCBBCACCABA",
"ABCBBCBAACCA",
"ABCBBCBACAAC",
"ABCBBCCAACBA",
"ABCBBCCABAAC",
"ABCBCAACBCBA",
"ABCBCABCACBA",
"ABCBCABCBAAC",
"ACABBCBACBCA",
"ACABBCCBBACA",
"ACABBCCBCABA",
"ACABCBBCBACA",
"ACABCBBCCABA",
"ACABCBCABCBA",
"ACBCABBACBCA",
"ACBCABCBBACA",
"ACBCABCBCABA",
"ACBCBAABCBCA",
"ACBCBACBABCA",
"ACBCBACBCAAB",
"ACBCCBABBACA",
"ACBCCBABCABA",
"ACBCCBBAABCA",
"ACBCCBBACAAB",
"ACBCCBCAABBA",
"ACBCCBCABAAB",
"ACCBABBCBACA",
"ACCBABBCCABA",
"ACCBABCABCBA",
"ACCBBCABBACA",
"ACCBBCABCABA",
"ACCBBCBAABCA",
"ACCBBCBACAAB",
"ACCBBCCAABBA",
"ACCBBCCABAAB",
"ACCBCAABBCBA",
"ACCBCABCABBA",
"ACCBCABCBAAB",
"BCABACBACBCA",
"BCABACCBBACA",
"BCABACCBCABA",
"BCABBAACCBCA",
"BCABBACBACCA",
"BCABBACBCAAC",
"BCABCBACBACA",
"BCABCBACCABA",
"BCABCBBAACCA",
"BCABCBBACAAC",
"BCABCBCAACBA",
"BCABCBCABAAC",
"BCACABBACBCA",
"BCACABCBBACA",
"BCACABCBCABA",
"BCACBAABCBCA",
"BCACBACBABCA",
"BCACBACBCAAB",
"BCACCBABBACA",
"BCACCBABCABA",
"BCACCBBAABCA",
"BCACCBBACAAB",
"BCACCBCAABBA",
"BCACCBCABAAB",
"BCBAABACCBCA",
"BCBAABCBACCA",
"BCBAABCBCAAC",
"BCBAACABCBCA",
"BCBAACCBABCA",
"BCBAACCBCAAB",
"BCBACBABACCA",
"BCBACBABCAAC",
"BCBACBACABCA",
"BCBACBACCAAB",
"BCBACBCAABAC",
"BCBACBCAACAB",
"BCCBABACBACA",
"BCCBABACCABA",
"BCCBABBAACCA",
"BCCBABBACAAC",
"BCCBABCAACBA",
"BCCBABCABAAC",
"BCCBACABBACA",
"BCCBACABCABA",
"BCCBACBAABCA",
"BCCBACBACAAB",
"BCCBACCAABBA",
"BCCBACCABAAB",
"BCCBBAABACCA",
"BCCBBAABCAAC",
"BCCBBAACABCA",
"BCCBBAACCAAB",
"BCCBBACAABAC",
"BCCBBACAACAB",
"BCCBCAABACBA",
"BCCBCAABBAAC",
"BCCBCAACABBA",
"BCCBCAACBAAB",
"BCCBCABAABAC",
"BCCBCABAACAB",
"CBABACBCBACA",
"CBABACBCCABA",
"CBABACCABCBA",
"CBABBCACBACA",
"CBABBCACCABA",
"CBABBCBAACCA",
"CBABBCBACAAC",
"CBABBCCAACBA",
"CBABBCCABAAC",
"CBABCAACBCBA",
"CBABCABCACBA",
"CBABCABCBAAC",
"CBACABBCBACA",
"CBACABBCCABA",
"CBACABCABCBA",
"CBACBCABBACA",
"CBACBCABCABA",
"CBACBCBAABCA",
"CBACBCBACAAB",
"CBACBCCAABBA",
"CBACBCCABAAB",
"CBACCAABBCBA",
"CBACCABCABBA",
"CBACCABCBAAB",
"CBBCABACBACA",
"CBBCABACCABA",
"CBBCABBAACCA",
"CBBCABBACAAC",
"CBBCABCAACBA",
"CBBCABCABAAC",
"CBBCACABBACA",
"CBBCACABCABA",
"CBBCACBAABCA",
"CBBCACBACAAB",
"CBBCACCAABBA",
"CBBCACCABAAB",
"CBBCBAABACCA",
"CBBCBAABCAAC",
"CBBCBAACABCA",
"CBBCBAACCAAB",
"CBBCBACAABAC",
"CBBCBACAACAB",
"CBBCCAABACBA",
"CBBCCAABBAAC",
"CBBCCAACABBA",
"CBBCCAACBAAB",
"CBBCCABAABAC",
"CBBCCABAACAB",
"CBCAABACBCBA",
"CBCAABBCACBA",
"CBCAABBCBAAC",
"CBCAACABBCBA",
"CBCAACBCABBA",
"CBCAACBCBAAB",
"CBCABCABACBA",
"CBCABCABBAAC",
"CBCABCACABBA",
"CBCABCACBAAB",
"CBCABCBAABAC",
"CBCABCBAACAB"
};

int main() {
    fastio;
    
    ll n;
    cin >> n;
    
    ll v = 4;
    ll ans = 1;
    
    for (ll i = 1; i < n; i++) {
        ans += v;
        v *= 3;
    }
    
    ll half_ans = (ans + 1) / 2;
    ll p2_ans = (1 << n) - 1;
    
    string k;
    for (int i = 0; i < 6; i++) {
        string s;
        cin >> s;
        
        k += s;
    }
    
    if (pow2s.find(k) != pow2s.end()) {
        cout << p2_ans;
    }
    else if (halves.find(k) != halves.end()) {
        cout << half_ans;
    }
    else cout << ans;
    
    // vector<string> pri(6);
    
    // for (int i = 0; i < 6; i++) cin >> pri[i];
    
    // while (next_permutation(pri.begin(), pri.end())) {
    //     // for (string z: pri) {
    //     //     cout << z << " ";
    //     // }
        
    //     string hsh;
        
    //     for (string z: pri) {
    //         hsh += z;
    //     }
        
    //     vector<stack<ll>> pole(3);
        
    //     for (ll i = n; i > 0; i--) {
    //         pole[0].push(i);
    //     }
        
    //     int last_moved = -1;
    //     ll moves = 0;
    //     while (pole[1].size() != n && pole[2].size() != n) {
    //         for (int i = 0; i < 6; i++) {
    //             int from = (int) (pri[i][0] - 'A');
    //             int to = (int) (pri[i][1] - 'A');
                
    //             if (last_moved == from) continue;
    //             if (pole[from].empty()) continue;
    //             if (!pole[to].empty() && pole[to].top() < pole[from].top()) continue;
                
    //             last_moved = to;
    //             pole[to].push(pole[from].top());
    //             pole[from].pop();
                
    //             break;
    //         }
            
    //         moves++;
    //     }
        
    //     // if (moves == 1023) {
    //     //     string k;
    //     //     for (int i = 0; i < 6; i++) {
    //     //         k += pri[i];
    //     //     }
    //     //     pw2.push_back(k);
    //     // }
    //     // else if (moves == 19683) {
    //     //     string k;
    //     //     for (int i = 0; i < 6; i++) {
    //     //         k += pri[i];
    //     //     }
    //     //     half.push_back(k);
    //     // }
        
    //     ll truth = ans;
    //     if (pow2s.find(hsh) != pow2s.end()) {
    //         truth = p2_ans;
    //     }
    //     else if (halves.find(hsh) != halves.end()) {
    //         truth = half_ans;
    //     }
    //     assert(truth == moves);
    // }
    
    // for (string p2: pw2) {
    //     cout << "\"" << p2 << "\",\n";
    // }
    // cout << "=====\n";
    
    // for (string hf: half) {
    //     cout << "\"" << hf << "\",\n";
    // }
}
