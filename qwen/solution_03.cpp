// Qwen attempt 3: contribution technique, O(n log n).
// Process values in increasing order.  For element j, the number of smaller
// elements in [l, r] is C(r) - C(l-1) where C(x) = #{i <= x : a_i < a_j}.
// Displacement of j is (C(r) - C(l-1)) - (j - l); count the (l, r) pairs for
// which this is even with a parity segment tree (suffix flip when an element
// becomes "smaller").
//
// Bug: elements with EQUAL values are handled as one group: all of them are
// queried before any of them is inserted, i.e. "smaller" means a_i < a_j
// strictly.  Stable sorting orders equal values by index, so an equal element
// on the LEFT must count as smaller.  Wrong answer whenever values repeat.
#include <bits/stdc++.h>
using namespace std;

int N;
vector<array<int,4>> cnt;   // cnt[v][p*2+q]: parity of C = p, parity of x = q
vector<char> lz;

void pull(int v){ for(int k=0;k<4;k++) cnt[v][k]=cnt[2*v][k]+cnt[2*v+1][k]; }
void app(int v){ swap(cnt[v][0],cnt[v][2]); swap(cnt[v][1],cnt[v][3]); lz[v]^=1; }
void push(int v){ if(lz[v]){ app(2*v); app(2*v+1); lz[v]=0; } }
void build(int v,int lo,int hi){
    cnt[v]={0,0,0,0};
    if(lo==hi){ cnt[v][lo&1]=1; return; }
    int mid=(lo+hi)/2; build(2*v,lo,mid); build(2*v+1,mid+1,hi); pull(v);
}
void flip(int v,int lo,int hi,int l,int r){
    if(r<lo||hi<l) return;
    if(l<=lo&&hi<=r){ app(v); return; }
    push(v); int mid=(lo+hi)/2; flip(2*v,lo,mid,l,r); flip(2*v+1,mid+1,hi,l,r); pull(v);
}
array<int,4> query(int v,int lo,int hi,int l,int r){
    if(r<lo||hi<l) return {0,0,0,0};
    if(l<=lo&&hi<=r) return cnt[v];
    push(v); int mid=(lo+hi)/2;
    auto A=query(2*v,lo,mid,l,r), B=query(2*v+1,mid+1,hi,l,r);
    for(int k=0;k<4;k++) A[k]+=B[k];
    return A;
}

int main(){
    int n; scanf("%d",&n);
    vector<long long> a(n+1);
    for(int i=1;i<=n;i++) scanf("%lld",&a[i]);
    N=n; cnt.assign(4*(n+1),{0,0,0,0}); lz.assign(4*(n+1),0);
    build(1,0,n);
    vector<int> ord(n); iota(ord.begin(),ord.end(),1);
    sort(ord.begin(),ord.end(),[&](int x,int y){ return a[x]<a[y]; });
    long long ans=0;
    for(int s=0;s<n;){
        int e=s;
        while(e<n && a[ord[e]]==a[ord[s]]) e++;
        // query the whole group of equal values first ...
        for(int k=s;k<e;k++){
            int j=ord[k];
            auto R=query(1,0,n,j,n);
            auto L=query(1,0,n,0,j-1);
            long long Rb[2],Lb[2];
            for(int b=0;b<2;b++){
                Rb[b]=R[b*2]+R[b*2+1];
                Lb[b]=0;
                for(int q=0;q<2;q++){ int p=b^((j-1)&1)^q; Lb[b]+=L[p*2+q]; }
            }
            ans+=Lb[0]*Rb[0]+Lb[1]*Rb[1];
        }
        // ... then insert them all
        for(int k=s;k<e;k++) flip(1,0,n,ord[k],n);
        s=e;
    }
    printf("%lld\n",ans);
}
