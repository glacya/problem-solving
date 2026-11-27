// BOJ 2133 - 타일 채우기
// 난이도: Gold IV
// 푼 날짜: 2023-06-06
// https://www.acmicpc.net/problem/2133

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
typedef long long ll;

ll factorial(ll n) {
    if (n <= 0) return 1;
    else return factorial(n - 1) * n;
}

ll comb(ll n, ll r) {
    if (n == r) return 1;
    return factorial(n) / factorial(n - r) / factorial(r);
}

ll calculate(vector<ll> &partition, ll total, ll maximum) {
    if (total == maximum) {
        unordered_map<ll, ll> nums;
        ll value = 1;
        for (ll x: partition) {
            value *= ((x == 1) ? 3 : 2);
            if (nums.find(x) != nums.end()) {
                nums[x]++;
            }
            else {
                nums[x] = 1;
            }
            // cout << x << " ";
        }
        
        ll length = partition.size();
        for (auto p: nums) {
            value *= comb(length, p.second);
            length -= p.second;
        }
        
        // cout << endl;
        return value;
    }
    
    ll last = maximum;
    if (!partition.empty()) {
        last = partition.back();
        last = maximum - total > last ? last : maximum - total;
    }
    
    ll answer = 0;
    for (ll i = last; i > 0; i--) {
        vector<ll> next_p(partition);
        next_p.push_back(i);
        
        answer += calculate(next_p, total + i, maximum);
    }
    return answer;
}

int main()
{
    ll n;
    cin >> n;
    
    if (n % 2) {
        cout << 0;
    }
    else {
        n /= 2;
        vector<ll> empty;
        cout << calculate(empty, 0, n);
    }

    return 0;
}
