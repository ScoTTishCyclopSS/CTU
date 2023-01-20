#include <iostream>
#include <vector>
using namespace std;

class Station{
public:
    int row, col, price;
    Station(int r, int c, int cell)
    {
        row = r;
        col = c;
        price = cell;
    }
};

bool place_ok(int row, int col);
void fin_sum();
int st_sum();
void foo(vector<vector<int>> &matrix, int col);

int h, w, d, n, result = INT32_MAX;
vector<Station> stations;

int main() {
    scanf("%d %d %d %d", &h, &w, &d, &n);
    vector<vector<int>> matrix(h, vector<int>(w));

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            scanf("%d", &(matrix[i][j]));
        }
    }
    foo(matrix, 0);
    printf("%d\n", result);
    return 0;
}

bool place_ok(int row, int col)
{
    for (auto & station : stations)
    {
        if (station.col == col || station.row == row)
            return false;

        if ((abs(row - station.row) + abs(col - station.col)) < d)
            return false;
    }
    return true;
}

void fin_sum()
{
    if (stations.size() == n)
    {
        int sum = st_sum();
        result = sum < result ? sum : result;
    }
}

int st_sum()
{
    int sum = 0;
    for (Station s : stations)
        sum += s.price;
    return sum;
}


void foo(vector<vector<int>> &matrix, int col)
{
    if (col == w || stations.size() >= n) {
        fin_sum();
        return;
    }

    // try to place station in all rows in curr column
    // to see if any of them leads to the solution
    for (int row = 0; row < h; row++)
    {
        if (place_ok(row, col))
        {
            stations.emplace_back(row, col, matrix[row][col]); // place station

            if (st_sum() < result)
            {
                for (int i = col + 1; i <= w; i++)
                {
                    foo(matrix, i); // try next column (recursion)
                }
            }

            stations.pop_back(); // if placing doesn't lead to the solution -> backtrack
        }
    }
}

