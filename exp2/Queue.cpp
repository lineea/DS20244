#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

int largestRectangleArea(vector<int>& heights) {
    int maxArea = 0;
    stack<int> s;
    heights.push_back(0);  // Add a sentinel value to handle the remaining elements in the stack

    for (int i = 0; i < heights.size(); ++i) {
        while (!s.empty() && heights[i] < heights[s.top()]) {
            int h = heights[s.top()];
            s.pop();
            int width = s.empty() ? i : i - s.top() - 1;
            maxArea = max(maxArea, h * width);
        }
        s.push(i);
    }
    
    heights.pop_back();  // Remove the sentinel value
    return maxArea;
}

vector<int> generateRandomHeights(int n, int maxHeight) {
    vector<int> heights;
    for (int i = 0; i < n; ++i) {
        heights.push_back(rand() % (maxHeight + 1));
    }
    return heights;
}

int main() {
    srand(time(0));  // Initialize random seed
    int numTests = 10;
    int maxBars = 10;      // 最大柱子数量
    int maxHeight = 10;    // 最大高度

    for (int i = 1; i <= numTests; ++i) {
        int n = rand() % maxBars + 1;  // 随机柱子数量
        vector<int> heights = generateRandomHeights(n, maxHeight);

        cout << "heights = [";
        for (int j = 0; j < heights.size(); ++j) {
            cout << heights[j] << (j < heights.size() - 1 ? ", " : "");
        }
        cout << "]" << endl;

        int maxArea = largestRectangleArea(heights);
        cout << "最大矩形面积为: " << maxArea << endl << endl;
    }

    return 0;
}
