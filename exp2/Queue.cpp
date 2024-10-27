#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>

using namespace std;

// 计算最大矩形面积的函数
int largestRectangleArea(vector<int>& heights) {
    stack<int> s; // 单调栈
    int maxArea = 0;
    heights.push_back(0); // 添加一个高度为0的柱子，方便计算
    for (int i = 0; i < heights.size(); i++) {
        while (!s.empty() && heights[s.top()] > heights[i]) {
            int h = heights[s.top()];
            s.pop();
            int w = s.empty() ? i : i - s.top() - 1;
            maxArea = max(maxArea, h * w);
        }
        s.push(i);
    }
    return maxArea;
}

// 随机生成测试数据
vector<int> generateRandomHeights(int size) {
    vector<int> heights(size);
    for (int i = 0; i < size; i++) {
        heights[i] = rand() % 10001; // 高度范围为0到10000
    }
    return heights;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // 设置随机种子

    for (int i = 0; i < 10; i++) {
        int size = rand() % 100000 + 1; // 随机长度，范围为1到100000
        vector<int> heights = generateRandomHeights(size);
        
        // 打印随机生成的柱子高度
        cout << "Test case " << (i + 1) << ": ";
        for (int height : heights) {
            cout << height << " ";
        }
        cout << endl;

        int maxArea = largestRectangleArea(heights);
        cout << "Maximum rectangle area: " << maxArea << endl << endl;
    }

    return 0;
}

