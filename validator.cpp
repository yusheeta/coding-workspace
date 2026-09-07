// Input validator for "Steady Books".
// Reads a test from stdin and exits with code 0 if it satisfies the statement
// exactly, otherwise prints the reason to stderr and exits with code 1.
//
// Checked:
//   * first line: a single integer n, 1 <= n <= 300000, no leading zeros,
//     no extra characters, terminated by '\n'
//   * second line: exactly n integers separated by single spaces,
//     each 1 <= a_i <= 1e9, no leading zeros, terminated by '\n'
//   * nothing after the second line
#include <bits/stdc++.h>
using namespace std;

static const long long MAX_N = 300000;
static const long long MAX_A = 1000000000LL;

[[noreturn]] void fail(const string& why) {
    fprintf(stderr, "INVALID: %s\n", why.c_str());
    exit(1);
}

// Reads a token of digits from `s` starting at `pos`, advances pos, returns value.
long long readInt(const string& s, size_t& pos, long long lo, long long hi, const string& what) {
    size_t start = pos;
    while (pos < s.size() && isdigit((unsigned char)s[pos])) pos++;
    if (pos == start) fail(what + ": expected a digit, found '" + (pos < s.size() ? string(1, s[pos]) : string("EOF")) + "'");
    if (pos - start > 1 && s[start] == '0') fail(what + ": leading zero");
    if (pos - start > 18) fail(what + ": too many digits");
    long long v = stoll(s.substr(start, pos - start));
    if (v < lo || v > hi) fail(what + ": value " + to_string(v) + " out of range [" + to_string(lo) + ", " + to_string(hi) + "]");
    return v;
}

int main() {
    string all((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
    size_t pos = 0;

    long long n = readInt(all, pos, 1, MAX_N, "n");
    if (pos >= all.size() || all[pos] != '\n') fail("expected newline after n");
    pos++;

    for (long long i = 1; i <= n; i++) {
        readInt(all, pos, 1, MAX_A, "a_" + to_string(i));
        if (i < n) {
            if (pos >= all.size() || all[pos] != ' ') fail("expected single space after a_" + to_string(i));
            pos++;
        }
    }
    if (pos >= all.size() || all[pos] != '\n') fail("expected newline after a_n");
    pos++;
    if (pos != all.size()) fail("extra data after the second line");

    fprintf(stderr, "OK n=%lld\n", n);
    return 0;
}
