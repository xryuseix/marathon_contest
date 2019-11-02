#include <iostream>
#include <vector>
#include <queue>
using namespace std;
typedef vector<int> vi;
typedef vector<char> vc;
typedef vector<vector<int> > vvi;
typedef vector<vector<char> > vvc;
#define rep(i,n) for(int i = 0; i < (n); ++i)
#define mp(p,q) make_pair(p, q)
struct io{io(){ios::sync_with_stdio(false);cin.tie(0);};};

int main(void) {

    // 入力
    int n, m, b;
    cin >> n >> m >> b;
    int gy, gx;
    cin >> gy >> gx;
    vi ry(m), rx(m);
    vc c(m);
    rep(i, m){
        cin >> ry[i] >> rx[i] >> c[i];
    }
    vi by(b), bx(b);
    rep(i, b){
        cin >> by[i] >> bx[i];
    }
    
    vvi grid(n, vi(n, 0));
    // 0..何もなし
    // 1..ロボット
    // 2..ブロック
    // 3..ゴール

    vvi info(n, vi(n, 0));
    // 0..何もなし
    // 1..案内板(上)
    // 2..案内板(右)
    // 3..案内板(下)
    // 4..案内板(左)

    // ゴールを設置
    grid[gy][gx] = 3;


    // ロボットとその向きを設置
    rep(i,m){
        grid[ry[i]][rx[i]] = 1;
    }
    
    // ブロックを設置
    rep(i,b){
        grid[by[i]][bx[i]] = 2;
    }

    // ゴールへBFS
    for(int r = 0; r < m; r++) {
        vector<vector<int> > prev_x(n, vector<int>(n, -1));
        vector<vector<int> > prev_y(n, vector<int>(n, -1));
        vector<vector<bool> > passed(n, vector<bool>(n, false));
        for(int i = 0; i < b; i++) {
            passed[by[i]][bx[i]] = true;
        }
        int lastx = gx, lasty = gy;
        queue<pair<int, int> > q;
        q.push(mp(rx[r], ry[r]));
        while(!q.empty()) {
            pair<int, int> now = q.front(); q.pop();
            passed[now.second][now.first] = true;
            if(now.first == gx && now.second == gy) {
                //ゴール
                break;
            }
            if(info[now.second][now.first] > 0) {
                lastx = now.first;
                lasty = now.second;
                //既に書かれているから終了
                break;
            } 
            {
                int dx[] = {1, 0, -1, 0};
                int dy[] = {0, 1, 0, -1};
                for(int i = 0; i < 4; i++) {
                    int next_x = now.first + dx[i];
                    int next_y = now.second + dy[i];

                    if(next_x >= n)next_x -= n;
                    if(next_x < 0)next_x += n;
                    if(next_y >= n)next_y -= n;
                    if(next_y < 0)next_y += n;

                    if (grid[next_y][next_x] == 2) continue;//ブロック
                    if(passed[next_y][next_x])continue;
                    q.push(mp(next_x, next_y));
                    prev_x[next_y][next_x] = now.first;
                    prev_y[next_y][next_x] = now.second;
                }
            }
        }
        vvc field(n, vc(n, '_'));
        {
            //経路を復元
            int x = lastx, y = lasty;
            while (x != -1 && y != -1) {
                field[y][x] = 'o'; // 通過した
                // 前の頂点へ行く
                int px = prev_x[y][x];
                int py = prev_y[y][x];
                x = px, y = py;
            }
        }
        {
            // 最短経路を元に，矢印を作成してinfoに写す
            int x = rx[r], y = ry[r];
            field[y][x] = '_';
            while(1){
                if(x == gx && y == gy) break;
                bool ch = false;
                int dx[] = {1, 0, -1, 0};
                int dy[] = {0, 1, 0, -1};
                for(int i = 0; i < 4; i++) {
                    int next_x = x + dx[i];
                    int next_y = y + dy[i];

                    if(next_x >= n) next_x -= n;
                    if(next_x < 0) next_x += n;
                    if(next_y >= n) next_y -= n;
                    if(next_y < 0) next_y += n;

                    if(field[next_y][next_x] == 'o') {
                        ch = true;
                        if(i == 0){
                            info[y][x] = 2;
                        } else if(i == 1){
                            info[y][x] = 3;
                        } else if(i == 2){
                            info[y][x] = 4;
                        } else if(i == 3){
                            info[y][x] = 1;
                        }
                        field[y][x] = '_';
                        y = next_y;
                        x = next_x;
                        break;
                    }
                }
                if(!ch)break;
            }
        }
    }
    // 岩に囲まれたところはいらなそう
    for(int i = 1; i < n - 1; i++) {
        for(int j = 1; j < n - 1; j++) {
            if(info[i][j] != info[i + 1][j])continue;
            if(info[i][j] != info[i - 1][j])continue;
            if(info[i][j] == 2||info[i][j] == 4)continue;
            if(info[i][j - 1] == 2)continue;
            if(info[i][j + 1] == 4)continue;
            if(grid[i][j] == 1)continue;
            info[i][j] = 0;
        }
    }
    for(int i = 1; i < n - 1; i++) {
        for(int j = 1; j < n - 1; j++) {
            if(info[i][j] != info[i][j + 1])continue;
            if(info[i][j] != info[i][j - 1])continue;
            if(info[i][j] == 1||info[i][j] == 3)continue;
            if(info[i - 1][j] == 3)continue;
            if(info[i + 1][j] == 1)continue;
            if(grid[i][j] == 1)continue;
            info[i][j] = 0;
        }
    }

    // ここから出力系
    vi ansy(1610), ansx(1610);
    vc ansr(1610);
    int num = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(info[i][j] > 0){
                num++;
                ansy[num] = i;
                ansx[num] = j;
                if(info[i][j] == 1) {
                    ansr[num] = 'U';
                } else if(info[i][j] == 2) {
                    ansr[num] = 'R';
                } else if(info[i][j] == 3) {
                    ansr[num] = 'D';
                } else if(info[i][j] == 4) {
                    ansr[num] = 'L';
                }
            }
        }
    }
    cout << num << endl;
    for(int i = 0; i < num; i++) {
        cout << ansy[i] << " " << ansx[i] << " " << ansr[i] << endl;
    }

}

