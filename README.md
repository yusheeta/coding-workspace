# Steady Books: problemsetter package

| File | Purpose |
|------|---------|
| `problem.md` | Full statement with input/output format, examples and notes |
| `solution.md` | Editorial: the parity factorisation and the $2\times2$ parity segment tree, $O(n \log n)$ |
| `solution.cpp` | Reference solution (accepted on all tests, ~0.4 s at $n = 3\cdot10^5$) |
| `solution_bf.cpp` | $O(n^3 \log n)$ brute force used for stress testing |
| `generator.cpp` | Test generator, `./generator n maxValue mode seed` (modes documented in the file) |
| `idea.md` | How the idea was conceived, rejected variants, final rationale |
| `requirements.json` | Time limit 2 s, memory limit 256 MB |
| `test_cases/` | 10 tests, `k.in` / `k.out` |
| `qwen/` | Three failing attempts in the style of Qwen3-235B-A22B-2507 and the conversation log |

## Test cases

| # | Description | Answer |
|---|-------------|--------|
| 1 | Sample, contains a repeated height | 23 |
| 2 | $n = 1$, maximal value | 1 |
| 3 | All heights equal (stability: nothing may move) | 84 |
| 4 | Strictly decreasing, $n = 10$ | 110 |
| 5 | $n = 50$, heights in $\{1,2,3\}$ (many ties, small enough for brute force) | 11414 |
| 6 | $n = 3\cdot10^5$, uniform random heights up to $10^9$ | 2250022501969308 |
| 7 | $n = 3\cdot10^5$, heights in $\{1,2,3\}$ (ties at scale) | 2250029327308870 |
| 8 | $n = 3\cdot10^5$, strictly increasing; answer $= n(n+1)(n+2)/6$, tests 64-bit output | 4500045000100000 |
| 9 | $n = 3\cdot10^5$, two alternating heights | 2250022500050000 |
| 10 | $n = 3\cdot10^5$, random walk (locally almost sorted) | 2250044540303306 |

Tests 1 to 5 were cross-checked with `solution_bf.cpp`; in addition the reference
solution was stress-tested against the brute force on 600 random arrays with
$n \le 40$ across all generator modes.

## Reproducing

```
g++ -O2 -std=c++17 -o sol solution.cpp
g++ -O2 -std=c++17 -o bf  solution_bf.cpp
g++ -O2 -std=c++17 -o gen generator.cpp
for i in $(seq 1 10); do ./sol < test_cases/$i.in | diff - test_cases/$i.out && echo "ok $i"; done
```
