#include <iostream>
#include <vector>
#include <queue>
#include <memory>
using namespace std;

class Pos{
public:
    int row, col, health, steps;
    bool visited, isFire;

    Pos(int _x, int _y){
        row = _x;
        col = _y;
        health = 0;
        steps = 0;
        isFire = false;
        visited = false;
    }
};

void bfs(vector<vector<Pos*>> &matrix, Pos *startPos, Pos *endPos);
bool checkPos(Pos *origin, Pos *neighbour, int maxHealth);
Pos* getPos(int x, int y, vector<vector<Pos*>> &matrix);

int main() {

    int r, c, h;
    scanf("%d %d %d", &r, &c, &h);

    int sr, sc, er, ec;
    scanf("%d %d", &sr, &sc);
    scanf("%d %d", &er, &ec);

    vector<vector<Pos*>> matrix;
    for (int i = 0; i < r; i++) // read matrix
    {
        vector<Pos*> row;
        for (int j = 0; j < c; ++j) {
            row.push_back(new Pos(i, j));
            scanf("%d", &row[j]->isFire);
        }
        matrix.push_back(row);
//        for (int j = 0; j < c; ++j) {
//            printf("[%d %d]:%d ", row[j]->row, row[j]->col, row[j]->isFire);
//        }
//        printf("\n");
    }

    Pos startPos(sr - 1, sc - 1);
    startPos.isFire = matrix[startPos.row][startPos.col]->isFire;;
    startPos.health = h;

    Pos endPos(er - 1, ec - 1);
    endPos.isFire = matrix[endPos.row][endPos.col]->isFire;

    //printf("start: [%d %d], end: [%d %d]\n", startPos.row, startPos.col, endPos.row, endPos.col);
    bfs(matrix, &startPos, &endPos);
}

void bfs(vector<vector<Pos*>> &matrix, Pos *startPos, Pos *endPos)
{
    queue<Pos*> q;
    q.push(startPos);
    int maxHealth = startPos->health;

    while(!q.empty())
    {
        Pos *currPos = q.front();
        q.pop();

        //printf("[%d %d]: h: %d, s: %d -> ", currPos->row, currPos->col, currPos->health, currPos->steps);

        if (currPos->row == endPos->row && currPos->col == endPos->col) {
            printf("%d\n", currPos->steps);
            break;
        }

        // check neighbours
        vector<Pos *> neighbours;
        Pos *r = getPos(currPos->row, currPos->col + 1, matrix);
        neighbours.push_back(r);
        Pos *l = getPos(currPos->row, currPos->col - 1, matrix);
        neighbours.push_back(l);
        Pos *u = getPos(currPos->row - 1, currPos->col, matrix);
        neighbours.push_back(u);
        Pos *d = getPos(currPos->row + 1, currPos->col, matrix);
        neighbours.push_back(d);

        for (auto &n : neighbours) {
            if (checkPos(currPos, n, maxHealth)) {
                n->visited = true;
                n->health = currPos->health - n->isFire;
                n->steps = currPos->steps + 1;
                q.push(n);
                //printf("[%d %d] | ", n->row, n->col);
            }
        }
        //printf("\n");
    }
}

Pos *getPos(int x, int y, vector<vector<Pos*>> &matrix)
{
    if (x < 0 || y < 0 || x >= int(matrix.size()) || y >= int(matrix[0].size()))
        return nullptr;

    return matrix[x][y];
}

bool checkPos(Pos *origin, Pos *neighbour, int maxHealth)
{
    if (origin == nullptr || neighbour == nullptr)
        return false;

    // calculate health if I follow the neighbour's path
    int currHealth = origin->health - neighbour->isFire;

    // if the neighbour is not visited yet, or I have better health solution
    // (e.g. visit neighbour with more heath than previous visit)
    if (currHealth > neighbour->health || !neighbour->visited)
    {
        // step inside, but is neighbour valid?
        if (currHealth <= maxHealth && currHealth >= 0)
        {
            return true;
        }
    }
    return false;
}