#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;
#define rep(i,n) for(int i = 0; i < (n); ++i)


int main(void) {

    int n,w,k,V;
    cin>>n>>w>>k>>V;
    vi c(n),v(n);
    int a;
    rep(i,n)cin>>c[i]>>v[i];

    vi res(n);
    vector<pair<int,int>> p(n);
    vvi tree(w);
    for(int i = 0; i < n; i++) {
        int put_pos = 0;
        bool is_find_pos = false;
        for(int j = w - 1; j > 0; j--) {
            if(tree[j].size() == -1 + tree[j - 1].size()) {
                if(tree[j - 1][tree[j].size()] == c[i]) {
                    put_pos = j;
                    is_find_pos = true;
                    break;
                }
            }
        }
        //cout<<is_find_pos<<endl;
        if(!is_find_pos) {
            int min_h = tree[w - 1].size();
            for(int j = 0; j < w; j++) {
                if(tree[j].size() == min_h) {
                    put_pos = j;
                    is_find_pos = true;
                    break;
                }
            }
        }
        tree[put_pos].push_back(c[i]);
        res[i] = put_pos;
    }
    {
        rep(i, n) {
            p[i] = make_pair(c[i], res[i]);
        }
        // 入力の同じ者の順番を変える
        for(int i = 0; i < n; i++) {
            int pos = i;
            while(pos < n && p[pos].first == p[i].first) {
                pos++;
            }
            sort(p.begin(), p.begin() + (pos - i));
            pos--;
            i = pos;
        }
        for(int i = 0; i < n; i++) {
            cout<<p[i].second<<endl;
        }
    }
}

