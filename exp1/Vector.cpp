#include<iostream>
using namespace std;
typedef int Rank; 
using Rank = unsigned int; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

template <typename T> class Vector { //向量模板类
protected:
   Rank _size; Rank _capacity;  T* _elem; //规模、容量、数据区
   void copyFrom ( T const* A, Rank lo, Rank hi ); //复制数组区间A[lo, hi)
   void expand(); //空间不足时扩容
   void shrink(); //装填因子过小时压缩
   bool bubble ( Rank lo, Rank hi ); //扫描交换
   void bubbleSort ( Rank lo, Rank hi ); //起泡排序算法
   Rank maxItem ( Rank lo, Rank hi ); //选取最大元素
   void selectionSort ( Rank lo, Rank hi ); //选择排序算法
   void merge ( Rank lo, Rank mi, Rank hi ); //归并算法
   void mergeSort ( Rank lo, Rank hi ); //归并排序算法
   void heapSort ( Rank lo, Rank hi ); //堆排序（稍后结合完全堆讲解）
   Rank partition ( Rank lo, Rank hi ); //轴点构造算法
   void quickSort ( Rank lo, Rank hi ); //快速排序算法
   void shellSort ( Rank lo, Rank hi ); //希尔排序算法
public:
// 构造方法
   Vector ( Rank c = DEFAULT_CAPACITY ) //容量为c的空向量
   { _elem = new T[_capacity = c]; _size = 0; }
   Vector ( Rank c, Rank s, T v ) //容量为c、规模为s、所有元素初始为v；s<=c
   { _elem = new T[_capacity = c]; for ( _size = 0; _size < s; _elem[_size++] = v ); }
   Vector ( T const* A, Rank n ) { copyFrom ( A, 0, n ); } //数组整体复制
   Vector ( T const* A, Rank lo, Rank hi ) { copyFrom ( A, lo, hi ); } //区间
   Vector ( Vector<T> const& V ) { copyFrom ( V._elem, 0, V._size ); } //向量整体复制
   Vector ( Vector<T> const& V, Rank lo, Rank hi ) { copyFrom ( V._elem, lo, hi ); } //区间
// 析构方法
   ~Vector() { delete [] _elem; } //释放内部空间
// 只读访问接口
  Rank size() const { return _size; } //规模
  bool empty() const { return !_size; } //判空
  Rank find ( T const& e ) const { return find ( e, 0, _size ); } //无序向量整体查找
  Rank find ( T const& e, Rank lo, Rank hi ) const; //无序向量区间查找
  Rank select( Rank k ) { return quickSelect( _elem, _size, k ); } //从无序向量中找到第k大的元素
  Rank search( T const& e ) const //有序向量整体查找
  { return ( 0 >= _size ) ? -1 : search ( e, 0, _size ); }
  Rank search ( T const& e, Rank lo, Rank hi ) const; //有序向量区间查找
// 可写访问接口
   T& operator[] ( Rank r ); //重载下标操作符，可以类似于数组形式引用各元素
   const T& operator[] ( Rank r ) const; //仅限于做右值的重载版本
   Vector<T> & operator= ( Vector<T> const& ); //重载赋值操作符，以便直接克隆向量
   T remove ( Rank r ); //删除秩为r的元素
   Rank remove ( Rank lo, Rank hi ); //删除秩在区间[lo, hi)之内的元素
   Rank insert ( Rank r, T const& e ); //插入元素
   Rank insert ( T const& e ) { return insert ( _size, e ); } //默认作为末元素插入
   void sort ( Rank lo, Rank hi ); //对[lo, hi)排序
   void sort() { sort ( 0, _size ); } //整体排序
   void unsort ( Rank lo, Rank hi ); //对[lo, hi)置乱
   void unsort() { unsort ( 0, _size ); } //整体置乱
   Rank dedup(); //无序去重
   Rank uniquify(); //有序去重
// 遍历
   void traverse ( void (* ) ( T& ) ); //遍历（使用函数指针，只读或局部性修改）
   template <typename VST> void traverse ( VST& ); //遍历（使用函数对象，可全局性修改）
}; //Vector
template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
    _elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;
    while (lo < hi)
        _elem[_size++] = A[lo++];
} //复制的构造方法 
template <typename T> Vector<T>& Vector<T>::operator= (Vector<T> const& V) { //重载 
    if (_elem) delete[] _elem;
    copyFrom(V._elem, 0, V.size());
    return *this;
}
template <typename T> void Vector<T>::expand() {
    if (_size < _capacity) return;
    T* oldElem = _elem; copyFrom(oldElem, 0, _capacity);
    delete[] oldElem;
}//扩容 
template <typename T> void Vector<T>::shrink() {
    if (_capacity < DEFAULT_CAPACITY << 1) return;
    if (_size << 2 > _capacity) return;
    T* oldElem = _elem; _elem = new T[_capacity >>= 1];
    for (Rank i = 0; i < _size; i++) _elem[i] = oldElem[i];
    delete[] oldElem;
}//缩容 
template <typename T> T& Vector<T>::operator[] (Rank r) //重载下标操作符
{
    return _elem[r];
}

template <typename T> const T& Vector<T>::operator[] (Rank r) const //仅限于做右值
{
    return _elem[r];
}//常规向量直接引用 
template <typename T> void permute(Vector<T>& V) {
    for (int i = V.size(); i > 0; i--)
        swap(V[i - 1], V[rand() % i]);
}//置乱算法 
template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) {
    T* V = _elem + lo;
    for (Rank i = hi - lo; 1 < i; --i)
        swap(V[i - 1], V[rand() % i]);
}//区间置乱接口 
template <typename T> static bool lt(T* a, T* b) { return lt(*a, *b); }
template <typename T> static bool lt(T& a, T& b) { return a < b; }
template <typename T> static bool lt(T* a, T* b) { return lt(*a, *b); }
template <typename T> static bool lt(T& a, T& b) { return a < b; }
//重载比较器以比较对象指针 
template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
    while ((lo < hi--) && (e != _elem[hi]));
    return hi;
}//顺序查找 
template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
    expand();
    for (Rank i = _size; r < i; i--)
        _elem[i] = _elem[i - 1];
    _elem[r] = e; _size++;
    return r;
}//插入 
template <typename T> Rank Vector<T>::remove(Rank lo, Rank hi) {
    if (lo == hi) return 0;
    while (hi < _size) _elem[lo++] = _elem[hi++];
    _size = lo; shrink();
    return hi - lo;
}//区间删除 
template <typename T> T Vector<T>::remove(Rank r) {
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}//单元素删除 
template <typename T> Rank Vector<T>::dedup() {
    Rank oldSize = _size;
    for (Rank i = 1; i < _size; )
        if (-1 == find(_elem[i], 0, i))
            i++;
        else
            remove(i);
    return oldSize - _size;
}//唯一化 
template <typename T> void Vector<T>::traverse(void (*visit)(T&))
{
    for (Rank i = 0; i < _size; i++) visit(_elem[i]);
} //遍历向量

template <typename T> template <typename VST>
void Vector<T>::traverse(VST& visit)
{
    for (Rank i = 0; i < _size; i++) visit(_elem[i]);
} //遍历向量
template <typename T> struct Increase
{
    virtual void operator()(T& e) { e++; }
}; //假设T可直接递增或已重载++
template <typename T> void increase(Vector<T>& V)
{
    V.traverse(Increase<T>());
} //以Increase<T>()为基本操作进行遍历
template <typename T> Rank Vector<T>::uniquify() {
    Rank i = 0, j = 0;
    while (++j < _size)
        if (_elem[i] != _elem[j])
            _elem[++i] = _elem[j];
    _size = ++i; shrink();
    return j - i;
} //有序向量重复元素剔除算法（高效版）
template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
    return (rand() % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
} //等概率地随机使用二分查找、Fibonacci查找
template <typename T> static Rank fibSearch(T* S, T const& e, Rank lo, Rank hi) {
    for (Fib fib(hi - lo); lo < hi; ) {
        while (hi - lo < fib.get()) fib.prev();
        Rank mi = lo + fib.get() - 1;
        if (e < S[mi]) hi = mi;
        else if (S[mi] < e) lo = mi + 1;
        else                  return mi;
    }
    return -1;
}//fibonacci查找算法 
template <typename T> static Rank binSearch(T* S, T const& e, Rank lo, Rank hi) {
    while (lo < hi) {
        Rank mi = (lo + hi) >> 1;
        (e < S[mi]) ? hi = mi : lo = mi + 1;
    }
    return lo - 1;
}//二分查找算法（版本C）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <typename T> void Vector<T>::sort(Rank lo, Rank hi) {
    switch (rand() % 5) {
    case 1: bubbleSort(lo, hi); break;
    case 2: selectionSort(lo, hi); break;
    case 3: mergeSort(lo, hi); break;
    case 4: heapSort(lo, hi); break;
    case 5: quickSort(lo, hi); break;
    default: shellSort(lo, hi); break;
    }
}//向量排序器接口
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    while (!bubble(lo, hi--));
}//起泡排序
template <typename T> bool Vector<T>::bubble(Rank lo, Rank hi) {
    bool sorted = true;
    while (++lo < hi)
        if (_elem[lo - 1] > _elem[lo]) {
            sorted = false;
            swap(_elem[lo - 1], _elem[lo]);
        }
    return sorted;
}//扫描交换单趟 
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;
    Rank mi = (lo + hi) / 2;
    mergeSort(lo, mi); mergeSort(mi, hi);
    merge(lo, mi, hi);
}//向量归并排序 
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    Rank i = 0; T* A = _elem + lo;
    Rank j = 0, lb = mi - lo; T* B = new T[lb];
    for (Rank i = 0; i < lb; i++) B[i] = A[i];
    Rank k = 0, lc = hi - mi; T* C = _elem + mi;
    while ((j < lb) && (k < lc))
        A[i++] = (B[j] <= C[k]) ? B[j++] : C[k++];
    while (j < lb)
        A[i++] = B[j++];
    delete[] B;
}//有序向量二路归并 
