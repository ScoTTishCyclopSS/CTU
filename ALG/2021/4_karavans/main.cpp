#include <cstdio>
#include <vector>
#include <queue>

using namespace std;


int addSupp = 0, noAddSuppVill = 0, saturation = 0, villagesCount = 0, friendlyCount = 0;
vector<int> paths;
queue<int> q;

typedef vector<vector<int>> village;
village villages;

void startJourney();

int recalculateDestination(int v1, int v2);

int main(int argc, char* argv[])
{
    int routesCount = 0, v1, v2;

    // read start info
    if (scanf("%d %d %d %d", &villagesCount, &routesCount, &friendlyCount, &saturation) != 4)
    {
        return 1;
    }

    paths = vector<int>(villagesCount + 1);
    for (int i = 0; i <= villagesCount; i++)
    {
        paths[i] = -1; // "infinity"
    }

    // read villages info
    villages = village (villagesCount + 1, vector<int>());
    while (scanf("%d %d", &v1, &v2) == 2)
    {
        villages[v1].push_back(v2);
        villages[v2].push_back(v1);
    }

    q.push(1);
    paths[1] = 0;

    startJourney();

    for (int i = 1; i <= villagesCount; i++)
    {
        int suppUsed = paths[i] / (saturation + 1);
        if (suppUsed > addSupp)
        {
            addSupp = suppUsed;
        }
        if (suppUsed == 0)
        {
            noAddSuppVill += 1;
        }
    }

    printf("%d %d\n", addSupp, noAddSuppVill);

    return 0;
}

void startJourney() {
    while (!q.empty())
    {
        int currV = q.front();
        q.pop();

        for (size_t i = 0; i < villages[currV].size(); i++)
        {
            int neighbor = villages[currV][i];
            int updated = recalculateDestination(currV, neighbor);
            if (paths[neighbor] == -1 || (paths[neighbor] != -1 && updated < paths[neighbor]))
            {
                paths[neighbor] = updated;
                q.push(neighbor);
            }
        }
    }
}

int recalculateDestination(int v1, int v2) {
    int updated = paths[v1] + 1;
    if (v2 <= friendlyCount)
    {
        int suppNeed = updated / (saturation + 1);
        updated = suppNeed * (saturation + 1);
    }
    return updated;
}
