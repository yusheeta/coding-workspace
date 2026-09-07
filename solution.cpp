// Steady Books -- optimal solution, O(n log n)
//
// For every subarray [l, r] we stable-sort it and count the elements whose
// displacement (new position - old position) is even.  We sum this count over
// all n(n+1)/2 subarrays.
//
// Element j inside [l, r] ends at offset 1 + S(l, r, j), where S is the number
// of elements i in [l, r] that are "smaller" than j in the total order
// (a_i, i) < (a_j, j)  (stable sort => equal values are ordered by index).
// Its original offset is j - l + 1, so its displacement is S - (j - l).
// With C_j(x) = #{ i <= x : (a_i, i) < (a_j, j) }, we have S = C_j(r) - C_j(l-1)
// and the condition "displacement even" becomes
//        C_j(r) mod 2  ==  ( C_j(l-1) + (j-1-(l-1)) ) mod 2 .
// The two sides depend on r only and on l only, so for a fixed j the number of
// good (l, r) pairs is  sum_b  L_b * R_b  where
//   R_b = #{ r in [j, n]   : C_j(r) mod 2 == b }
//   L_b = #{ x in [0, j-1] : (C_j(x) + (j-1) + x) mod 2 == b }.
//
// Processing the elements in the order (a_j, j) and "inserting" each processed
// position p (which flips C(x) for all x >= p) turns C_j into the current
// prefix-parity function.  A lazy segment tree over x in [0, n] storing, per
// node, the number of positions with (parity of C, parity of x) = (p, q)
// supports the suffix flip and both counting queries in O(log n).
#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n;
    vector<array<array<int, 2>, 2>> cnt;   // cnt[node][parityC][parityX]
    vector<char> lz;
    SegTree(int n_) : n(n_), cnt(4 * (n_ + 1)), lz(4 * (n_ + 1), 0) {
        build(1, 0, n);
    }
    void build(int v, int lo, int hi) {
        cnt[v] = {{{0, 0}, {0, 0}}};
        if (lo == hi) { cnt[v][0][lo & 1] = 1; return; }
        int mid = (lo + hi) >> 1;
        build(2 * v, lo, mid);
        build(2 * v + 1, mid + 1, hi);
        pull(v);
    }
    void pull(int v) {
        for (int p = 0; p < 2; p++)
            for (int q = 0; q < 2; q++)
                cnt[v][p][q] = cnt[2 * v][p][q] + cnt[2 * v + 1][p][q];
    }
    void applyFlip(int v) {
        swap(cnt[v][0], cnt[v][1]);
        lz[v] ^= 1;
    }
    void push(int v) {
        if (lz[v]) {
            applyFlip(2 * v);
            applyFlip(2 * v + 1);
            lz[v] = 0;
        }
    }
    void flip(int v, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) return;
        if (l <= lo && hi <= r) { applyFlip(v); return; }
        push(v);
        int mid = (lo + hi) >> 1;
        flip(2 * v, lo, mid, l, r);
        flip(2 * v + 1, mid + 1, hi, l, r);
        pull(v);
    }
    // returns cnt[.][.] aggregated over positions in [l, r]
    array<array<int, 2>, 2> query(int v, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) return {{{0, 0}, {0, 0}}};
        if (l <= lo && hi <= r) return cnt[v];
        push(v);
        int mid = (lo + hi) >> 1;
        auto A = query(2 * v, lo, mid, l, r);
        auto B = query(2 * v + 1, mid + 1, hi, l, r);
        for (int p = 0; p < 2; p++)
            for (int q = 0; q < 2; q++) A[p][q] += B[p][q];
        return A;
    }
    void flip(int l, int r) { flip(1, 0, n, l, r); }
    array<array<int, 2>, 2> query(int l, int r) { return query(1, 0, n, l, r); }
};

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) scanf("%lld", &a[i]);

    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 1);
    sort(ord.begin(), ord.end(), [&](int x, int y) {
        if (a[x] != a[y]) return a[x] < a[y];
        return x < y;
    });

    SegTree st(n);
    long long ans = 0;
    for (int j : ord) {
        auto R = st.query(j, n);        // r in [j, n]
        auto L = st.query(0, j - 1);    // x = l-1 in [0, j-1]
        long long Rb[2], Lb[2];
        for (int b = 0; b < 2; b++) {
            Rb[b] = R[b][0] + R[b][1];
            // need (C(x) + (j-1) + x) mod 2 == b
            //  =>  C(x) mod 2 == b ^ ((j-1)&1) ^ (x&1)
            Lb[b] = 0;
            for (int q = 0; q < 2; q++) Lb[b] += L[b ^ ((j - 1) & 1) ^ q][q];
        }
        ans += Lb[0] * Rb[0] + Lb[1] * Rb[1];
        st.flip(j, n);                  // element j is now "smaller" for later ones
    }
    printf("%lld\n", ans);
    return 0;
}
