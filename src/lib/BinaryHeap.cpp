#include "BinaryHeap.h"

int BinaryHeap::ExtractMin()
{
    if(IsEmpty()) {
        std::cout << "heap is empty" << std::endl;
        exit(1);
    }

    int min = _heap[1]._element;

    _positions[_heap.back()._element] = 1;
    _positions.erase(_heap[1]._element);

    _heap[1] = _heap[_heap.size() - 1];
    _heap.erase(_heap.begin() + _heap.size() - 1);
    MinHeapify(1, (int)_heap.size());

    return min;
}
void BinaryHeap::MinHeapify(int node, int length)
{
    int left = 2 * node;
    int right = 2 * node + 1;
    int smallest;

    if(left <= length && _heap[left]._key < _heap[node]._key) smallest = left;
    else smallest = node;

    if(right <= length && _heap[right]._key < _heap[smallest]._key) smallest = right;

    if(smallest != node) {
        Swap(_heap[smallest]._element, _heap[node]._element);
        Swap(_heap[smallest], _heap[node]);
        MinHeapify(smallest, length);
    }
}
void BinaryHeap::BuildMinHeap(std::vector<std::pair<int, double>> grids)
{
    for(int i = 0; i < (int)grids.size(); ++i) {
        _heap[i + 1]._element = grids[i].first;
        _heap[i + 1]._key = grids[i].second;
        _positions.insert(make_pair(grids[i].first, i + 1));
    }

    for(int i = (int)_heap.size() / 2; i >= 1; --i) {
        MinHeapify(i, (int)_heap.size() - 1);
    }
}
void BinaryHeap::DecreaseKey(int node, int new_key)
{
    int idx = FindPosition(node);
    if(idx == -1) return;
    if(new_key > _heap[idx]._key) {
        return;
    }
    _heap[idx]._key = new_key;
    while(idx > 1 && _heap[GetParentNode(idx)]._key > _heap[idx]._key) {
        Swap(_heap[idx]._element, _heap[GetParentNode(idx)]._element);
        Swap(_heap[idx], _heap[GetParentNode(idx)]);
        idx = GetParentNode(idx);
    }
}
int BinaryHeap::FindPosition(int node)
{
    map<int, int>::iterator it = _positions.find(node);
    if(it == _positions.end()) return -1;
    return it->second; 
}
void BinaryHeap::Swap(HeapNode &n1, HeapNode &n2)
{
    HeapNode temp = n1;
    n1 = n2;
    n2 = temp;
}
void BinaryHeap::Swap(int n1, int n2)
{
    int temp = _positions.find(n1)->second;
    _positions.find(n1)->second = _positions.find(n2)->second;
    _positions.find(n2)->second = temp;
}
