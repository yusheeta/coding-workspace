// Qwen attempt 1: direct simulation.
// For every subarray, copy it, stable-sort it, and count elements whose
// displacement is even.  Correct but O(n^3 log n) -> TLE for n = 3*10^5.
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;
    long long answer = 0;
    for (int l = 0; l < n; l++) {
        for (int r = l; r < n; r++) {
            vector<pair<long long, int>> seg;
            for (int i = l; i <= r; i++) seg.push_back({a[i], i});
            stable_sort(seg.begin(), seg.end(),
                        [](const auto &p, const auto &q) { return p.first < q.first; });
            for (int k = 0; k < (int)seg.size(); k++) {
                int newPos = l + k;
                int oldPos = seg[k].second;
                if ((newPos - oldPos) % 2 == 0) answer++;
            }
        }
    }
    cout << answer << "\n";
    return 0;
}
