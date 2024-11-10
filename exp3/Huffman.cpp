#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstring>

using namespace std;

// 二叉树节点
struct BinTreeNode {
    char ch;       // 字符
    int freq;      // 频率
    BinTreeNode* left;
    BinTreeNode* right;

    BinTreeNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// 二叉树
class BinTree {
public:
    BinTreeNode* root;

    BinTree() : root(nullptr) {}
    BinTree(BinTreeNode* r) : root(r) {}

    // 释放内存
    ~BinTree() {
        deleteTree(root);
    }

    void deleteTree(BinTreeNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};
class HuffTree {
public:
    BinTree* tree;

    HuffTree(const unordered_map<char, int>& freq_map) {
        priority_queue<pair<int, BinTreeNode*>, vector<pair<int, BinTreeNode*>>, greater<pair<int, BinTreeNode*>>> minHeap;

        // 将所有字符及其频率放入最小堆
        for (const auto& entry : freq_map) {
            minHeap.push({entry.second, new BinTreeNode(entry.first, entry.second)});
        }

        // 构建 Huffman 树
        while (minHeap.size() > 1) {
            auto left = minHeap.top(); minHeap.pop();
            auto right = minHeap.top(); minHeap.pop();

            BinTreeNode* merged = new BinTreeNode('\0', left.first + right.first);
            merged->left = left.second;
            merged->right = right.second;

            minHeap.push({merged->freq, merged});
        }

        // 最终只剩一个节点，它就是根节点
        tree = new BinTree(minHeap.top().second);
    }

    // 递归生成 Huffman 编码
    void generateCodes(BinTreeNode* node, const string& code, unordered_map<char, string>& huffman_codes) {
        if (!node) return;
        if (node->ch != '\0') {  // 非内部节点
            huffman_codes[node->ch] = code;
        }
        generateCodes(node->left, code + "0", huffman_codes);
        generateCodes(node->right, code + "1", huffman_codes);
    }

    // 获取字符的 Huffman 编码
    unordered_map<char, string> getHuffmanCodes() {
        unordered_map<char, string> huffman_codes;
        generateCodes(tree->root, "", huffman_codes);
        return huffman_codes;
    }
};
class Bitmap {
private:
    unsigned char* M;
    int N, _sz;  // 位图空间 M[]，N*sizeof(char)*8 个比特中含 _sz 个有效位

protected:
    void init(int n) {
        M = new unsigned char[N = (n + 7) / 8];
        memset(M, 0, N);
        _sz = 0;
    }

public:
    Bitmap(int n = 8) { init(n); }
    ~Bitmap() { delete[] M; M = nullptr; _sz = 0; }

    void set(int k) {
        expand(k);
        _sz++;
        M[k >> 3] |= (0x80 >> (k & 0x07));
    }

    void clear(int k) {
        expand(k);
        _sz--;
        M[k >> 3] &= ~(0x80 >> (k & 0x07));
    }

    bool test(int k) {
        expand(k);
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }

    void expand(int k) {
        if (k < 8 * N) return;
        int oldN = N;
        unsigned char* oldM = M;
        init(2 * k);
        memcpy(M, oldM, oldN);
        delete[] oldM;
    }

    string bits2string(int n) {
        expand(n - 1);
        string s = "";
        for (int i = 0; i < n; i++) {
            s += test(i) ? '1' : '0';
        }
        return s;
    }
};
void huffmanCoding(const string& text) {
    // 计算每个字符的频率
    unordered_map<char, int> freq_map;
    for (char c : text) {
        freq_map[c]++;
    }

    // 构建 Huffman 树
    HuffTree huffTree(freq_map);
    unordered_map<char, string> huffman_codes = huffTree.getHuffmanCodes();

    // 打印每个字符的 Huffman 编码
    cout << "Huffman Codes:" << endl;
    for (const auto& entry : huffman_codes) {
        cout << entry.first << ": " << entry.second << endl;
    }

    // 将 "dream" 和其他单词编码
    string word = "dream";
    string encoded = "";
    for (char c : word) {
        encoded += huffman_codes[c];
    }
    cout << "Encoded 'dream': " << encoded << endl;
}

int main() {
    string text = "I have a dream that one day this nation will rise up and live out the true meaning of its creed";
    huffmanCoding(text);
    return 0;
}

