#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>
#include <vector>
#include <omp.h>
#include <stack>
using namespace std;

#define INF 1e8
#define destiny 0
#define source 0
#define N 300000
#define NUM_THREAD 4



int maze[10000][10000] = {};
int step[10000][10000] = {};
// int tsx = 7, tsy = 0, tex = 2, tey = 5;
// short exp[8][8] = {1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 3, 2, 2, 1, 3, 2, 1, 2, 5, 1, 4, 0, 3, 4, 1, 2, 3, 1, 1, 3, 4, INF, 1, 2, 2, 1, 1, 1, 5, INF, 1, INF, INF, 2, 2, 2, 2, 2, INF, 1, INF, INF, 2, 2, 3, 4, 3, 0, 1, 4, 3, 1, 1, 3, 1}
int nex[4][2]={1, 0, -1, 0, 0, -1, 0, 1};

struct node{
    int x, y;
	int weight;
    bool friend operator < (node a, node b){
        return a.weight > b.weight;
    }
    bool friend operator == (node a, node b){
        return (a.x == b.x && a.y == b.y);
    }
};
priority_queue<node> q;
stack<node> past_path;
vector<node> B[N];
vector<node> S[NUM_THREAD];
vector<node> REQ[NUM_THREAD];
short road[10000][10000][2] = {}; 
int sx, sy, ex, ey, n;
int delta = 3;

int isok(int x, int y){
    return x >= 0 && x < 10000 && y >= 0 && y < 10000 && maze[x][y] != INF;
}

int dijkstra()
{
    while(!q.empty()) 
		q.pop();
	q.push((node){sx, sy, 0});
    while(!q.empty())
    {
        node cur = q.top();
		q.pop();
        int x = cur.x;
		int y = cur.y;
		int cw = cur.weight;
		int nx, ny, nw;
        for(int k = 0; k < 4; ++ k)
        {
            nx = x + nex[k][0];
			ny = y + nex[k][1];
            if(isok(nx,ny))
            {
                nw = cw + maze[nx][ny];
                if(nw < step[nx][ny]){
                	q.push((node){nx, ny, nw});
            		step[nx][ny] = nw;
            		road[nx][ny][0] = x;
            		road[nx][ny][1] = y;
				}
            }
        }
    }
    if(step[ex][ey] != INF){
        int prev_x = road[ex][ey][0];
        int prev_y = road[ex][ey][1];
        while(prev_x != sx && prev_y != sy){
            past_path.push((node){prev_x, prev_y, 0});
        	prev_x = road[prev_x][prev_y][0];
        	prev_y = road[prev_x][prev_y][1];
    	}
    }
    if(step[ex][ey] != INF)
    	return step[ex][ey];
	
    return -1;
}


void read_maze(string target){
    ifstream in(target.c_str());
    string filename;
    string line;
    int maze_row_index = 0;
	
    for(int i = 0; i < 10000; ++ i){
    	for(int j = 0; j < 10000; ++ j){
    		step[i][j] = INF;
		}
	}
	
    if (in) 
    {
        while (getline(in, line))
        {
			const char* str_line;
			str_line = line.c_str();
			// cout << line.c_str() << endl;
			int length = strlen(str_line);
			int index = -1;
			int start_index = 0;
			int end_index = 0;
			int maze_index = 0;
			while(index < length){
				index ++;
				if(str_line[index] == ' '){
					continue;
				}
				else if(str_line[index] == 'D'){
					ex = maze_row_index;
					ey = maze_index;
					maze[maze_row_index][maze_index++] = destiny;
				}
				else if(str_line[index] == 'S'){
					sx = maze_row_index;
					sy = maze_index;
					maze[maze_row_index][maze_index++] = source;					
				}

				else if(str_line[index] == '-'){
					index ++;
					maze[maze_row_index][maze_index++] = INF;
				}
				else if(str_line[index] >= '0' and str_line[index] <= '5')
					maze[maze_row_index][maze_index++] = str_line[index] - '0';
				else{
					continue;
				}	
			}
			// cout << maze_row_index << endl;
            maze_row_index ++;
        }
    }
    else 
    {
        cout << "no such file" << endl;
    }
}

int main()
{
    int length[50] = {};
    for(int i = 0; i < 50; ++ i)
        length[i] = i;
    ofstream fout("result.txt"); 
    streambuf* pOld =cout.rdbuf(fout.rdbuf());
    for(int i = 0; i < 50; ++ i){
        char tmp[2];
        string s;
        sprintf(tmp, "%d", i);
        s = tmp;
        string target = "//home/pdc2022/maze_benchmark/case" + s + ".txt";
        read_maze(target);
        int result;
        double t1 = omp_get_wtime();
        // int result = dijkstra();
        double t2 = omp_get_wtime();
        for(int i = 0; i < 10000; ++ i){
            for(int j = 0; j < 10000; ++ j){
                step[i][j] = INF;
            }
        }
        t1 = omp_get_wtime();
    	result = dijkstra();
        t2 = omp_get_wtime();
         
        cout << "case" << s << " " << result << " ";
        if(step[ex][ey] != INF){
            while (!past_path.empty())
            {
                cout << "(" << past_path.top().x << "," << past_path.top().y << ")";
                past_path.pop();
                if (!past_path.empty())
                    cout << ' ';
            }
        }
        cout << endl;
    }
    return 0;
}
