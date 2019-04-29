#ifndef graphsp_h
#define graphsp_h

#include <climits>
#include <vector>
#include <list>
#include <map>

const int max_distance = 1000000;

class graphsp
{
    public:
        graphsp();
        ~graphsp();

        void Initialization(short hori_tile, short verti_tile, short layer);
        void InitialGraph(short hori_capacity, short verti_capacity);
        void InitialSourceTarget(int x1, int y1, int z1, int x2, int y2, int z2);
        void AddEdge(int from, int to, double weight);
        void UpdateWeight(int from, int to, int weight);
        void UpdateCapacity(int x1, int y1, int z1, int x2, int y2, int z2, int weight);
        void UpdateDemand(int idx1, int idx2);
        void Relax(int from, int to, double weight);
        void InitializeSingleSource(int start);
        void Dijkstra();
        void PrintDistance();
        void PrintPredecessor();
        void PrintAdjList();
        void PrintAdjList(int idx);
        void PrintEdgeInfo();
        void PrinEdgeInfo();
        void GetShortestPath();
        void BuildRoutingGrid();

        std::vector<std::tuple<int, int, int>> shortest_path;

    private:
        int _source, _target; 
        int _num_vertex;
        short _hori_tile_no;
        short _verti_tile_no;
        short _layer_no;

        int _begin_idx, _end_idx;
        short _hori_routing_tile_no; 
        short _verti_routing_tile_no; 
        short _routing_layer_no; 

        std::vector<std::list<std::pair<int, double>>> _adj_list; // next vertex and weight
        std::vector<std::map<int, std::pair<int, int>>> _edge_info; // map next vertex to the pair of demand and capacity
        std::vector<int> _predecessor;
        std::vector<double> _distance;
        std::vector<std::pair<int, double>> _idxs; 

        inline int ToIndex(int x, int y, int z);
        inline std::tuple<int, int, int> ToTile(int idx);
        void Reset();
};

#endif
