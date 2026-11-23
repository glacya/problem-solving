#include <bits/stdc++.h>

using ll = long long;
using namespace std;

ll gcd(ll a, ll b) {
    if (a == b) return a;
    if (a < b) swap(a, b);
    if (b == 1) return 1;
    if (b == 0) return a;

    return gcd(b, a % b);
}

class IntegerFraction {
    public:
    ll numerator;
    ll denominator;

    IntegerFraction(ll a, ll b) {
        this->numerator = a;
        this->denominator = b;
    }

    IntegerFraction operator+(IntegerFraction &infrac) {
        ll new_denom = this->denominator * infrac.denominator;
        ll new_numer = this->numerator * infrac.denominator + infrac.numerator * this->denominator;

        ll g = gcd(new_denom, new_numer);

        return IntegerFraction(new_numer / g, new_denom / g);
    }

    IntegerFraction operator*(ll x) {
        ll g = gcd(this->numerator * x, this->denominator);
        return IntegerFraction(this->numerator * x / g, this->denominator / g);
    }

    IntegerFraction operator*(IntegerFraction &infrac) {
        ll new_denom = this->denominator * infrac.denominator;
        ll new_numer = this->numerator * infrac.numerator;
        ll g = gcd(new_denom, new_numer);

        return IntegerFraction(new_numer / g, new_denom / g);
    }
};