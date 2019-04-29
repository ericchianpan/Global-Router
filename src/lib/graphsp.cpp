#include "graphsp.h"
#include "BinaryHeap.h"

graphsp::graphsp() {;}
graphsp::~graphsp() {;}
void graphsp::Initialization(short hori_tile, short verti_tile, short layer)
{
    _hori_tile_no = hori_tile;
    _verti_tile_no = verti_tile;
    _layer_no = layer;
    _num_vertex = hori_tile * verti_tile;
    _adj_list.resize(_num_vertex);
    _edge_info.resize(_num_vertex);
    _predecessor.resize(_num_vertex);
    _distance.resize(_num_vertex);
}
void graphsp::InitialGraph(short hori_capacity, short verti_capacity)
{
    for(short x = 0; x < _verti_tile_no; ++x) {
        for(short y = 0; y < _hori_tile_no; ++y) {
            int from = ToIndex(x, y, 0);
            if(x != _verti_tile_no) _edge_info[from].insert(make_pair(ToIndex(x + 1, y, 0), make_pair(0, hori_capacity)));
            if(x != 0) _edge_info[from].insert(make_pair(ToIndex(x - 1, y, 0), make_pair(0, hori_capacity)));
            if(y != _hori_tile_no) _edge_info[from].insert(make_pair(ToIndex(x, y + 1, 0), make_pair(0, verti_capacity))); 
            if(y != 0) _edge_info[from].insert(make_pair(ToIndex(x, y - 1, 0), make_pair(0, verti_capacity))); 
        }
    }
}
void graphsp::AddEdge(int from, int to, double weight)
{
    _adj_list[from].push_back(std::make_pair(to, weight));
}
void graphsp::UpdateCapacity(int x1, int y1, int z1, int x2, int y2, int z2, int weight)
{
    int v1 = ToIndex(x1, y1, z1);
    int v2 = ToIndex(x2, y2, z2);
    _edge_info[v1].find(v2)->second.second = weight;
    _edge_info[v2].find(v1)->second.second = weight;
}
void graphsp::UpdateDemand(int idx1, int idx2)
{
    _edge_info[idx1].find(idx2)->second.first += 2;
    _edge_info[idx2].find(idx1)->second.first += 2;
}
void graphsp::Relax(int from, int to, double weight)
{
    if(_distance[to] > _distance[from] + weight) {
        _distance[to] = _distance[from] + weight;
        _predecessor[to] = from;
    }
}
void graphsp::InitializeSingleSource(int start)
{
    for(int i = 0; i < _num_vertex; ++i) {
        _predecessor[i] = -1;
        _distance[i] = max_distance;
    }
    _distance[start] = 0;
}
void graphsp::Dijkstra()
{
    InitializeSingleSource(_source);

    BinaryHeap minQueue(_hori_routing_tile_no * _verti_routing_tile_no);
    minQueue.BuildMinHeap(_idxs);

    while(!minQueue.IsEmpty()) {
        int from = minQueue.ExtractMin();
        if(from == _target) break;
        if(_distance[from] >= max_distance) break;
        for(std::list<std::pair<int, double>>::iterator it = _adj_list[from].begin(); it != _adj_list[from].end(); ++it) {
            int to = it->first;
            double weight = it->second;
            Relax(from, to, weight);
            minQueue.DecreaseKey(to, _distance[to]);
        }
    }
}
void graphsp::PrintDistance()
{
    std::cout << "print distance" << std::endl;
    for(unsigned i = 0; i < _distance.size(); ++i) {
        std::cout << i << " ";
        std::cout << _distance[i] << std::endl;
    }
}
void graphsp::PrintPredecessor()
{
    std::cout << "print predecessor" << std::endl;
    for(unsigned i = 0; i < _distance.size(); ++i) {
        std::cout << i << " ";
        std::cout << _predecessor[i] << std::endl;
    }
}
void graphsp::GetShortestPath()
{
    shortest_path.clear();
    int predecessor = _predecessor[_target];
    if(predecessor == -1) {
        std::cout << "no solution" << std::endl;
       return;
    }
    shortest_path.push_back(ToTile(_target));
    UpdateDemand(_target, predecessor);
    while(predecessor != -1) {
        shortest_path.push_back(ToTile(predecessor));
        int from = predecessor;
        predecessor = _predecessor[predecessor];
        int to = predecessor;
        if(predecessor != -1) UpdateDemand(from, to);
    }
}
void graphsp::Reset()
{
    for(unsigned i = 0; i < _adj_list.size(); ++i) {
        _adj_list[i].clear();
    }
    _idxs.clear();
}
void graphsp::PrintAdjList()
{
    for(unsigned i = 0; i < _adj_list.size(); ++i) {
        std::cout << "index: " << i << " :" << std::endl;
        PrintAdjList(i);
    }
}
void graphsp::PrintAdjList(int idx)
{
    for(std::list<std::pair<int, double>>::iterator it = _adj_list[idx].begin(); it != _adj_list[idx].end(); ++it) {
        std::cout << "adjacent index: " << it->first << ", ";
        std::cout << "weight: " << it->second << std::endl;
    }
}
inline int graphsp::ToIndex(int x, int y, int z)
{
    return y * (_verti_tile_no) + x + z * (_verti_tile_no * _hori_tile_no);
}
inline std::tuple<int, int, int> graphsp::ToTile(int idx)
{
    short x = (idx % (_verti_tile_no * _hori_tile_no)) % _verti_tile_no;
    short y = (idx % (_verti_tile_no * _hori_tile_no)) / _verti_tile_no;
    short z = idx / (_verti_tile_no * _hori_tile_no);
    return std::make_tuple(x, y, z);
}
void graphsp::BuildRoutingGrid()
{
    Reset();

    tuple<int, int, int> source_grid = ToTile(_source);
    tuple<int, int, int> target_grid = ToTile(_target);

    short min_x = get<0>(source_grid) < get<0>(target_grid) ? get<0>(source_grid) : get<0>(target_grid);
    short max_x = get<0>(source_grid) > get<0>(target_grid) ? get<0>(source_grid) : get<0>(target_grid);
    short min_y = get<1>(source_grid) < get<1>(target_grid) ? get<1>(source_grid) : get<1>(target_grid);
    short max_y = get<1>(source_grid) > get<1>(target_grid) ? get<1>(source_grid) : get<1>(target_grid);

    _hori_routing_tile_no = max_y - min_y + 1;
    _verti_routing_tile_no = max_x - min_x + 1;
    _routing_layer_no = _layer_no;

    for(short x = min_x; x <= max_x; ++x) {
        for(short y = min_y; y <= max_y; ++y) {
            int z = 0;
            int from = ToIndex(x, y, z);
            if(from == _source) _idxs.push_back(make_pair(from, 0));
            else _idxs.push_back(make_pair(from, max_distance));
            if(x != max_x) {
                int to = ToIndex(x + 1, y, z);
                map<int, pair<int, int>>::iterator it = _edge_info[from].find(to);
                int demand = it->second.first;
                int capacity = it->second.second;
                double capacity_cost = (capacity == 0) ? max_distance : pow(2, demand / capacity) - 1;
                AddEdge(from, to, capacity_cost);
            }
            if(x != min_x) {
                int to = ToIndex(x - 1, y, z);
                map<int, pair<int, int>>::iterator it = _edge_info[from].find(to);
                int demand = it->second.first;
                int capacity = it->second.second;
                double capacity_cost = (capacity == 0) ? max_distance : pow(2, demand / capacity) - 1;
                AddEdge(from, to, capacity_cost);
            }
            if(y != max_y) {
                int to = ToIndex(x, y + 1, z);
                map<int, pair<int, int>>::iterator it = _edge_info[from].find(to);
                int demand = it->second.first;
                int capacity = it->second.second;
                double capacity_cost = (capacity == 0) ? max_distance : pow(2, demand / capacity) - 1;
                AddEdge(from, to, capacity_cost);
            }
            if(y != min_y) {
                int to = ToIndex(x, y - 1, z);
                map<int, pair<int, int>>::iterator it = _edge_info[from].find(to);
                int demand = it->second.first;
                int capacity = it->second.second;
                double capacity_cost = (capacity == 0) ? max_distance : pow(2, demand / capacity) - 1;
                AddEdge(from, to, capacity_cost);
            }
        }
    }
}
void graphsp::PrinEdgeInfo()
{
    for(unsigned i = 0; i < _edge_info.size(); ++i) {
        cout << "########## node " << i << " ##########" << endl;
        for(map<int, pair<int, int>>::iterator it = _edge_info[i].begin(); it != _edge_info[i].end(); ++it) {
            cout << "to node " << it->first << " demand " << it->second.first << " capacity " << it->second.second << endl;
        } 
    }
}
void graphsp::InitialSourceTarget(int x1, int y1, int z1, int x2, int y2, int z2)
{
    _source = ToIndex(x1, y1, z1);
    _target = ToIndex(x2, y2, z2);
}
void graphsp::PrintEdgeInfo()
{
    for(unsigned i = 0; i < _edge_info.size(); ++i) {
        for(map<int, pair<int, int>>::iterator it = _edge_info[i].begin(); it != _edge_info[i].end(); ++it) {
            cout << "capacity: " << it->second.first << ", demand: " << it->second.second << endl;
        }
    }
}
