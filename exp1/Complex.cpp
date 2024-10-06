#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>
#include"Vector.cpp"

class ComplexNumber {
public:
    double real;
    double imag;

    ComplexNumber(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    double modulus() const {
        return sqrt(real * real + imag * imag);
    }

    bool operator==(const ComplexNumber& other) const {
        return real == other.real && imag == other.imag;
    }

    bool operator<(const ComplexNumber& other) const {
        if (modulus() == other.modulus()) {
            return real < other.real;
        }
        return modulus() < other.modulus();
    }

    friend std::ostream& operator<<(std::ostream& os, const ComplexNumber& c) {
        os << c.real << " + " << c.imag << "i";
        return os;
    }
};
void shuffle_vector(std::vector<ComplexNumber>& vec) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);
}

ComplexNumber* find_complex(std::vector<ComplexNumber>& vec, const ComplexNumber& target) {
    for (auto& c : vec) {
        if (c == target) {
            return &c;
        }
    }
    return nullptr;
}

void insert_complex(std::vector<ComplexNumber>& vec, const ComplexNumber& c) {
    vec.push_back(c);
}

void delete_complex(std::vector<ComplexNumber>& vec, const ComplexNumber& c) {
    vec.erase(std::remove(vec.begin(), vec.end(), c), vec.end());
}

std::vector<ComplexNumber> unique_vector(const std::vector<ComplexNumber>& vec) {
    std::vector<ComplexNumber> unique_vec(vec);
    std::sort(unique_vec.begin(), unique_vec.end());
    unique_vec.erase(std::unique(unique_vec.begin(), unique_vec.end()), unique_vec.end());
    return unique_vec;
}
void bubble_sort(std::vector<ComplexNumber>& vec) {
    size_t n = vec.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (vec[j] > vec[j + 1]) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

void merge(std::vector<ComplexNumber>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<ComplexNumber> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = vec[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            vec[k++] = L[i++];
        }
        else {
            vec[k++] = R[j++];
        }
    }
    while (i < n1) {
        vec[k++] = L[i++];
    }
    while (j < n2) {
        vec[k++] = R[j++];
    }
}

void merge_sort(std::vector<ComplexNumber>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(vec, left, mid);
        merge_sort(vec, mid + 1, right);
        merge(vec, left, mid, right);
    }
}
int main() {
    // 生成随机复数向量
    std::vector<ComplexNumber> complex_vector;
    for (int i = 0; i < 20; ++i) {
        complex_vector.emplace_back(static_cast<double>(rand() % 20 - 10), static_cast<double>(rand() % 20 - 10));
    }

    std::cout << "初始复数向量:\n";
    for (const auto& c : complex_vector) {
        std::cout << c << std::endl;
    }

    // 置乱
    shuffle_vector(complex_vector);
    std::cout << "\n置乱后的向量:\n";
    for (const auto& c : complex_vector) {
        std::cout << c << std::endl;
    }

    // 查找
    ComplexNumber target = complex_vector[0];
    ComplexNumber* found = find_complex(complex_vector, target);
    std::cout << "\n查找复数 " << target << ": " << (found ? "找到" : "未找到") << std::endl;

    // 插入
    ComplexNumber new_complex(5, 5);
    insert_complex(complex_vector, new_complex);
    std::cout << "\n插入后的向量:\n";
    for (const auto& c : complex_vector) {
        std::cout << c << std::endl;
    }

    // 删除
    delete_complex(complex_vector, target);
    std::cout << "\n删除后的向量:\n";
    for (const auto& c : complex_vector) {
        std::cout << c << std::endl;
    }

    // 唯一化
    std::vector<ComplexNumber> unique_vector_result = unique_vector(complex_vector);
    std::cout << "\n唯一化后的向量:\n";
    for (const auto& c : unique_vector_result) {
        std::cout << c << std::endl;
    }

    // 排序效率测试
    std::vector<ComplexNumber> sorted_vector = unique_vector_result;

    for (const auto& order : { "顺序", "乱序", "逆序" }) {
        std::vector<ComplexNumber> to_sort;

        if (order == "顺序") {
            to_sort = sorted_vector;
        }
        else if (order == "乱序") {
            to_sort = unique_vector_result;
            shuffle_vector(to_sort);
        }
        else if (order == "逆序") {
            to_sort = sorted_vector;
            std::reverse(to_sort.begin(), to_sort.end());
        }

        // 测试冒泡排序
        std::vector<ComplexNumber> bubble_vector = to_sort;
        clock_t start = clock();
        bubble_sort(bubble_vector);
        clock_t end = clock();
        std::cout << order << "情况下，冒泡排序耗时: " << double(end - start) / CLOCKS_PER_SEC << "秒\n";

        // 测试归并排序
        std::vector<ComplexNumber> merge_vector = to_sort;
        start = clock();
        merge_sort(merge_vector, 0, merge_vector.size() - 1);
        end = clock();
        std::cout << order << "情况下，归并排序耗时: " << double(end - start) / CLOCKS_PER_SEC << "秒\n";
    }

    // 区间查找
    double m1 = 1.0, m2 = 5.0;
    std::vector<ComplexNumber> range_result = range_search(unique_vector_result, m1, m2);
    std::cout << "\n模介于 [" << m1 << ", " << m2 << ") 的复数:\n";
    for (const auto& c : range_result) {
        std::cout << c << std::endl;
    }

    return 0;
}
