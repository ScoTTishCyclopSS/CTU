// author: @timusfed
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

int main() {
    vector<tuple<int, int>> intervals1, intervals2;
    int d1, d2, n, time_sum = 0;
    int min_v = INT32_MAX, max_v = INT32_MIN;

    // read data
    scanf("%d %d %d", &n, &d1, &d2);

    // read robot1 info
    for (int i = 0; i < n; i++) {
        int k = 0;
        scanf("%d", &k);

        int previous = i == 0 ? 0 : get<1>(intervals1[i - 1]);
        intervals1.emplace_back(previous, previous + k);

        time_sum += k;

        // printf("%d - %d\n", previous, previous + k);
    }

    // cout << '\n';

    // read robot2 info
    for (int i = 0; i < n; i++) {
        int k = 0;
        scanf("%d", &k);

        int previous = i == 0 ? time_sum : get<0>(intervals2[i - 1]);
        intervals2.emplace_back(previous - k, previous);

        // printf("%d - %d\n", previous - k, previous);
    }

    // main block
    for (int i = 0; i < n; i++) {
        // FORMULA: influence_robot1 + influence_robot2 - same_time_influence

        // 1st robot
        int left = i - d1;
        if (left < 0)
            left = 0;

        int right = i + d1;
        if (right >= n)
            right = n - 1;

        int l1 = get<0>(intervals1[left]);
        int r1 = get<1>(intervals1[right]);

        int time_on_cell_r1 = r1 - l1;


        // 2nd robot
        left = i - d2;
        if (left < 0)
            left = 0;

        right = i + d2;
        if (right >= n)
            right = n - 1;

        int l2 = get<1>(intervals2[left]);
        int r2 = get<0>(intervals2[right]);

        int time_on_cell_r2 = l2 - r2;

        int intersection = min(r1, l2) - max(l1, r2);
        if (intersection < 0)
            intersection = 0;

        int final = (time_on_cell_r1 + time_on_cell_r2) * 2 - intersection;

        // cout << final << ' ';
        if (final < min_v)
            min_v = final;

        if (final > max_v)
            max_v = final;
    }

    printf("%d %d", min_v, max_v);
    exit(0);
}