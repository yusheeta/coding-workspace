# Steady Books

**Time limit:** 2 seconds  **Memory limit:** 256 megabytes

A librarian keeps $n$ books in a row on one long shelf. The book at position $i$
($1 \le i \le n$) has height $a_i$. The slots of the shelf are painted alternately
black and white, so slot $i$ and slot $j$ have the same colour exactly when
$i - j$ is even.

The librarian likes to tidy up *contiguous blocks* of books. Tidying the block of
positions $[l, r]$ means the following: the books at positions $l, l+1, \dots, r$
are rearranged so that their heights become non-decreasing from left to right. If
several books have the same height, they keep their original relative order (that
is, the block is sorted **stably**). Books outside the block are not touched.

A book is called **steady** with respect to the block $[l, r]$ if, after tidying
that block, it stands on a slot of the same colour as before. Equivalently, the
book was moved by an even number of positions (zero counts as even).

For every one of the $\frac{n(n+1)}{2}$ blocks $[l, r]$ with $1 \le l \le r \le n$
the librarian tidies that block **of the original shelf** (each block is
considered independently, starting from the original arrangement) and counts the
steady books in it. Find the sum of these counts over all blocks.

## Input

The first line contains one integer $n$ ($1 \le n \le 3 \cdot 10^5$), the number
of books.

The second line contains $n$ integers $a_1, a_2, \dots, a_n$
($1 \le a_i \le 10^9$), the heights of the books.

## Output

Print one integer: the total number of steady books, summed over all blocks
$[l, r]$.

## Examples

### Input
```
5
3 1 2 2 5
```
### Output
```
23
```

### Input
```
4
2 2 2 2
```
### Output
```
20
```

### Input
```
3
3 1 2
```
### Output
```
6
```

## Note

In the first example the shelf is $[3, 1, 2, 2, 5]$. Consider the block $[1, 4]$.
Stable sorting turns $[3, 1, 2, 2]$ into $[1, 2, 2, 3]$: the book of height $3$
moves from slot $1$ to slot $4$ (three positions), the book of height $1$ moves
from slot $2$ to slot $1$ (one position), the first book of height $2$ moves from
slot $3$ to slot $2$, the second one from slot $4$ to slot $3$. Every book moved an
odd number of positions, so this block contributes $0$.

For the block $[2, 5]$ the books $[1, 2, 2, 5]$ are already sorted, nothing moves
and all $4$ books are steady.

For the block $[1, 3]$, sorting $[3, 1, 2]$ gives $[1, 2, 3]$; the book of height
$3$ moves by two positions and is steady, the other two move by one position. The
block contributes $1$.

The contributions of all $15$ blocks are
$1, 0, 1, 0, 1, 1, 2, 3, 4, 1, 2, 3, 1, 2, 1$ (in the order
$[1,1],[1,2],\dots,[1,5],[2,2],\dots,[5,5]$), which sum to $23$.

In the second example all heights are equal, so stable sorting never moves a
book and every block of length $k$ contributes $k$. Note that if the equal books
were allowed to swap places the answer would be different, because the relative order
of equal heights is preserved.
