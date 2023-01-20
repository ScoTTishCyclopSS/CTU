#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    vector<int> input_array;
    map<int, int> solution; // fast 'unique' method

    int x;
    while(cin >> x)
    {

        input_array.push_back(x);
    }

    // solve: sort, then check next after
    sort(input_array.begin(), input_array.end());
    for (int i = 0; i < input_array.size() - 1; i++) // -1 'cause we don't need to check last
    {
        if (input_array[i] == input_array[i + 1])
        {
            solution[input_array[i]] = 1;
        }
    }

    // print solution
    for (pair<const int, int> i : solution)
    {
        cout << i.first << ' ';
    }
    return 0;
}
