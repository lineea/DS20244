#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <random>
#include <chrono>
#include <set>
#include "Vector.cpp"
class ComplexNumber {
public:
    double real;
    double imag;

    ComplexNumber(double r, double i) : real(r), imag(i) {}

    double modulus() const {
        return std::sqrt(real * real + imag * imag);
    }

    bool operator==(const ComplexNumber& other) const {
        return (real == other.real && imag == other.imag);
    }

    bool operator<(const ComplexNumber& other) const {
        if (modulus() == other.modulus())
            return real < other.real;
        return modulus() > other.modulus();
    }
    
    bool operator>(const ComplexNumber& other) const {
    if (modulus() == other.modulus())
        return real > other.real;
    return modulus() > other.modulus();
}


    friend std::ostream& operator<<(std::ostream& os, const ComplexNumber& c) {
        os << c.real << " + " << c.imag << "i";
        return os;
    }
};

std::vector<ComplexNumber> generate_random_complex_vector(size_t size) {
    std::vector<ComplexNumber> vec;
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dis(-10, 10);

    for (size_t i = 0; i < size; ++i) {
        vec.emplace_back(dis(gen), dis(gen));
    }
    return vec;
}

void shuffle_vector(std::vector<ComplexNumber>& vec) {
    std::random_shuffle(vec.begin(), vec.end());
}

int find_in_vector(const std::vector<ComplexNumber>& vec, const ComplexNumber& c) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == c) {
            return i;
        }
    }
    return -1;
}

void insert_to_vector(std::vector<ComplexNumber>& vec, const ComplexNumber& c) {
    vec.push_back(c);
}

void delete_from_vector(std::vector<ComplexNumber>& vec, const ComplexNumber& c) {
    vec.erase(std::remove(vec.begin(), vec.end(), c), vec.end());
}

std::vector<ComplexNumber> unique_vector(const std::vector<ComplexNumber>& vec) {
    std::set<ComplexNumber> seen;
    std::vector<ComplexNumber> unique_vec;

    for (const auto& comp : vec) {
        if (seen.insert(comp).second) {
            unique_vec.push_back(comp);
        }
    }
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

    std::vector<ComplexNumber> L(vec.begin() + left, vec.begin() + mid + 1);
    std::vector<ComplexNumber> R(vec.begin() + mid + 1, vec.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            vec[k++] = L[i++];
        } else {
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

std::vector<ComplexNumber> interval_search(const std::vector<ComplexNumber>& vec, double m1, double m2) {
    std::vector<ComplexNumber> result;
    for (const auto& c : vec) {
        double mod = c.modulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(c);
        }
    }
    return result;
}

int main() {
    // 1. 生成随机复数向量
    auto vector = generate_random_complex_vector(100);
    std::cout << "原始向量: \n";
    for (const auto& c : vector) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    // 置乱
    shuffle_vector(vector);
    std::cout << "置乱后的向量: \n";
    for (const auto& c : vector) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    // 查找
    ComplexNumber c(1, 1);
    int index = find_in_vector(vector, c);
    std::cout << "查找复数 " << c << " 的索引: " << index << std::endl;

    // 插入
    insert_to_vector(vector, c);
    std::cout << "插入后的向量: \n";
    for (const auto& comp : vector) {
        std::cout << comp << " ";
    }
    std::cout << std::endl;

    // 删除
    delete_from_vector(vector, c);
    std::cout << "删除后的向量: \n";
    for (const auto& comp : vector) {
        std::cout << comp << " ";
    }
    std::cout << std::endl;

    // 唯一化
    auto unique_vec = unique_vector(vector);
    std::cout << "唯一化后的向量: \n";
    for (const auto& comp : unique_vec) {
        std::cout << comp << " ";
    }
    std::cout << std::endl;

    // 2. 排序效率比较
    auto sorted_vector = unique_vec;

    // 冒泡排序时间测量
    auto start = std::chrono::high_resolution_clock::now();
    bubble_sort(sorted_vector);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> bubble_time = end - start;
    std::cout << "冒泡排序时间: " << bubble_time.count() << "秒" << std::endl;

    // 归并排序时间测量
    sorted_vector = unique_vec;
    start = std::chrono::high_resolution_clock::now();
    merge_sort(sorted_vector, 0, sorted_vector.size() - 1);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> merge_time = end - start;
    std::cout << "归并排序时间: " << merge_time.count() << "秒" << std::endl;

    // 3. 区间查找
    double m1 = 1.0, m2 = 5.0;
    auto interval_result = interval_search(sorted_vector, m1, m2);
    std::cout << "模介于 [" << m1 << ", " << m2 << ") 的复数: \n";
    for (const auto& c : interval_result) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    return 0;
}

