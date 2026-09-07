// Test generator.
// usage: ./generator <n> <maxValue> <mode> <seed>
//   mode 0: uniform random values in [1, maxValue]
//   mode 1: strictly increasing
//   mode 2: strictly decreasing
//   mode 3: all equal
//   mode 4: alternating two values
//   mode 5: sorted blocks of random lengths, shuffled within blocks (locally messy)
//   mode 6: random walk (adjacent values close)
//   mode 7: long runs of equal heights, run lengths random in [1, 1000], run values random
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) {
    if (argc < 5) { fprintf(stderr, "usage: n maxV mode seed\n"); return 1; }
    long long n = atoll(argv[1]), maxV = atoll(argv[2]);
    int mode = atoi(argv[3]);
    mt19937_64 rng(atoll(argv[4]));
    auto rnd = [&](long long lo, long long hi) { return lo + (long long)(rng() % (unsigned long long)(hi - lo + 1)); };
    vector<long long> a(n);
    if (mode == 0) for (auto &x : a) x = rnd(1, maxV);
    else if (mode == 1) { long long cur = 0; for (auto &x : a) { cur += rnd(1, max(1LL, maxV / n)); x = cur; } }
    else if (mode == 2) { long long cur = maxV; for (auto &x : a) { x = cur; cur -= rnd(1, max(1LL, maxV / n)); if (cur < 1) cur = 1; } }
    else if (mode == 3) { long long v = rnd(1, maxV); for (auto &x : a) x = v; }
    else if (mode == 4) { long long u = rnd(1, maxV), v = rnd(1, maxV); for (long long i = 0; i < n; i++) a[i] = (i & 1) ? u : v; }
    else if (mode == 5) {
        long long cur = 1; long long i = 0;
        while (i < n) {
            long long len = min(n - i, rnd(1, 20));
            for (long long k = 0; k < len; k++) a[i + k] = cur + rnd(0, len);
            shuffle(a.begin() + i, a.begin() + i + len, rng);
            cur += len + 1; i += len;
        }
    }
    else if (mode == 6) { long long cur = maxV / 2; for (auto &x : a) { cur += rnd(-3, 3); cur = max(1LL, min(maxV, cur)); x = cur; } }
    else {
        long long i = 0;
        while (i < n) {
            long long len = min(n - i, rnd(1, 1000)), v = rnd(1, maxV);
            for (long long k = 0; k < len; k++) a[i + k] = v;
            i += len;
        }
    }
    printf("%lld\n", n);
    for (long long i = 0; i < n; i++) printf("%lld%c", a[i], i + 1 == n ? '\n' : ' ');
}
