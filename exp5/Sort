#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

// 冒泡排序
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 插入排序
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// 选择排序
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

// 归并排序
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// 快速排序
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            swap(arr[++i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 堆排序
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i);
    for (int i = n - 1; i >= 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 辅助函数，用于测试和计时排序算法
void testSortingAlgorithm(void (*sortFunc)(vector<int>&), vector<int> arr, const string& algorithmName) {
    clock_t start = clock();
    sortFunc(arr);
    clock_t end = clock();
    cout << algorithmName << " 用时 " << double(end - start) / CLOCKS_PER_SEC << " 秒\n";
}

int main() {
    srand(time(0));
    int n = 10000; // 数组大小
    vector<int> sequential(n), reverse(n), random(n);

    for (int i = 0; i < n; ++i) {
        sequential[i] = i;
        reverse[i] = n - i;
        random[i] = rand() % n;
    }

    cout << "测试排序算法在顺序数据上的表现:\n";
    testSortingAlgorithm(bubbleSort, sequential, "冒泡排序");
    testSortingAlgorithm(insertionSort, sequential, "插入排序");
    testSortingAlgorithm(selectionSort, sequential, "选择排序");
    testSortingAlgorithm([](vector<int>& arr) { mergeSort(arr, 0, arr.size() - 1); }, sequential, "归并排序");
    testSortingAlgorithm([](vector<int>& arr) { quickSort(arr, 0, arr.size() - 1); }, sequential, "快速排序");
    testSortingAlgorithm(heapSort, sequential, "堆排序");

    cout << "\n测试排序算法在逆序数据上的表现:\n";
    testSortingAlgorithm(bubbleSort, reverse, "冒泡排序");
    testSortingAlgorithm(insertionSort, reverse, "插入排序");
    testSortingAlgorithm(selectionSort, reverse, "选择排序");
    testSortingAlgorithm([](vector<int>& arr) { mergeSort(arr, 0, arr.size() - 1); }, reverse, "归并排序");
    testSortingAlgorithm([](vector<int>& arr) { quickSort(arr, 0, arr.size() - 1); }, reverse, "快速排序");
    testSortingAlgorithm(heapSort, reverse, "堆排序");

    cout << "\n测试排序算法在随机数据上的表现:\n";
    testSortingAlgorithm(bubbleSort, random, "冒泡排序");
    testSortingAlgorithm(insertionSort, random, "插入排序");
    testSortingAlgorithm(selectionSort, random, "选择排序");
    testSortingAlgorithm([](vector<int>& arr) { mergeSort(arr, 0, arr.size() - 1); }, random, "归并排序");
    testSortingAlgorithm([](vector<int>& arr) { quickSort(arr, 0, arr.size() - 1); }, random, "快速排序");
    testSortingAlgorithm(heapSort, random, "堆排序");

    return 0;
}

