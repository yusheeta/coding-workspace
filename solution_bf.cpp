// Brute force: O(n^3 log n). Enumerate every subarray, stable-sort it,
// and count elements whose displacement is even.
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n; scanf("%d", &n);
    vector<long long> a(n);
    for (auto &x : a) scanf("%lld", &x);
    long long ans = 0;
    for (int l = 0; l < n; l++)
        for (int r = l; r < n; r++) {
            vector<pair<long long,int>> v;
            for (int i = l; i <= r; i++) v.push_back({a[i], i});
            stable_sort(v.begin(), v.end(),
                        [](const pair<long long,int>& p, const pair<long long,int>& q){ return p.first < q.first; });
            for (int k = 0; k < (int)v.size(); k++) {
                int newpos = l + k, oldpos = v[k].second;
                if (((newpos - oldpos) % 2 + 2) % 2 == 0) ans++;
            }
        }
    printf("%lld\n", ans);
}
