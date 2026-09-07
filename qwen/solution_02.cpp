// Qwen attempt 2: incremental right endpoint.
// Fix l, extend r one step at a time.  When a[r] is appended, its own rank is
// 1 + (#smaller elements already present), and every element with value
// greater than a[r] shifts one slot to the right, so its displacement parity
// flips.  A segment tree over compressed values keeps, for every present
// element, the parity of its displacement and supports "flip a value range".
// Correct, but O(n^2 log n) -> TLE for n = 3*10^5.
#include <bits/stdc++.h>
using namespace std;

struct Node { int cnt[2]; bool lazy; };
int m;
vector<Node> tr;

void apply(int v) { swap(tr[v].cnt[0], tr[v].cnt[1]); tr[v].lazy ^= 1; }
void push(int v) { if (tr[v].lazy) { apply(2*v); apply(2*v+1); tr[v].lazy = false; } }
void pull(int v) { for (int p = 0; p < 2; p++) tr[v].cnt[p] = tr[2*v].cnt[p] + tr[2*v+1].cnt[p]; }

void reset(int v, int lo, int hi) {
    tr[v].cnt[0] = tr[v].cnt[1] = 0; tr[v].lazy = false;
    if (lo == hi) return;
    int mid = (lo + hi) / 2; reset(2*v, lo, mid); reset(2*v+1, mid+1, hi);
}
void insert(int v, int lo, int hi, int pos, int par) {
    if (lo == hi) { tr[v].cnt[par]++; return; }
    push(v);
    int mid = (lo + hi) / 2;
    if (pos <= mid) insert(2*v, lo, mid, pos, par); else insert(2*v+1, mid+1, hi, pos, par);
    pull(v);
}
void flip(int v, int lo, int hi, int l, int r) {
    if (r < lo || hi < l) return;
    if (l <= lo && hi <= r) { apply(v); return; }
    push(v);
    int mid = (lo + hi) / 2;
    flip(2*v, lo, mid, l, r); flip(2*v+1, mid+1, hi, l, r);
    pull(v);
}
int countLess(int v, int lo, int hi, int l, int r) {   // number of present elements with value index in [l, r]
    if (r < lo || hi < l) return 0;
    if (l <= lo && hi <= r) return tr[v].cnt[0] + tr[v].cnt[1];
    push(v);
    int mid = (lo + hi) / 2;
    return countLess(2*v, lo, mid, l, r) + countLess(2*v+1, mid+1, hi, l, r);
}

int main() {
    int n;
    scanf("%d", &n);
    vector<long long> a(n);
    for (auto &x : a) scanf("%lld", &x);
    // compress (value, index) pairs so that equal values are ordered by index (stable sort)
    vector<int> ord(n); iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int x, int y){ return a[x] != a[y] ? a[x] < a[y] : x < y; });
    vector<int> key(n);
    for (int i = 0; i < n; i++) key[ord[i]] = i;
    m = n;
    tr.assign(4 * m + 4, Node{{0,0}, false});
    long long answer = 0;
    for (int l = 0; l < n; l++) {
        reset(1, 0, m - 1);
        for (int r = l; r < n; r++) {
            int smaller = countLess(1, 0, m - 1, 0, key[r] - 1);
            int disp = smaller - (r - l);                 // new offset - old offset
            if (key[r] + 1 <= m - 1) flip(1, 0, m - 1, key[r] + 1, m - 1);
            insert(1, 0, m - 1, key[r], ((disp % 2) + 2) % 2);
            answer += tr[1].cnt[0];
        }
    }
    printf("%lld\n", answer);
    return 0;
}
