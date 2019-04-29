#ifndef BinaryHeap_h
#define BinaryHeap_h

#include <vector>
#include <iostream>
#include <cmath>
#include <map>
using namespace std;

class BinaryHeap;

class HeapNode{
    public:
        HeapNode(): _element(0), _key(0) {}
        HeapNode(int element, double key): _element(element), _key(key) {}
        ~HeapNode() {}
        friend class BinaryHeap;
    private:
        int _element;
        double _key;
};

class BinaryHeap{
    public:
        BinaryHeap() {};
        BinaryHeap(int n) { _heap.resize(n + 1); }
        ~BinaryHeap() {};

        int Minimum() { return _heap[1]._element; }
        int ExtractMin();
        bool IsEmpty() { return _heap.size() == 1; } 
        void MinHeapify(int node, int length);
        void BuildMinHeap(std::vector<std::pair<int, double>> keys);
        void DecreaseKey(int node, int new_key);

    private:
        vector<HeapNode> _heap;
        map<int, int> _positions; // map element to position in _heap

        int FindPosition(int node);
        int GetParentNode(int node) { return std::floor(node / 2); }
        void Swap(HeapNode &n1, HeapNode &n2);
        void Swap(int n1, int n2);
};

#endif
