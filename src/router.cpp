#include "router.h"
#include "routingdb.h"

router::router() {;}
router::~router() {;}
void router::Route()
{
    _sp.Initialization(db.GetVertiGlobalTileNo(), db.GetHoriGlobalTileNo(), db.GetLayerNo());
    _sp.InitialGraph(db.GetLayerHoriCapacity(0), db.GetLayerVertiCapacity(1));

    AdjustCapacity();
    _routing_result.resize(db.GetNetNo());

    cout << "Number of net: " << db.GetNetNo() << endl;
    for(int i = 0; i < db.GetNetNo(); ++i) {
        Net& n = db.GetNetByPosition(i);
        _routing_result[i].resize(n.GetSubNetNo());
        for(int j = 0; j < n.GetSubNetNo(); ++j) {
            SubNet& sn = n.GetSubNet(j);
            // solve the shortest path
            _sp.InitialSourceTarget(sn.GetSourcePinGx(), sn.GetSourcePinGy(), sn.GetSourcePinLayer() - 1, 
                                    sn.GetTargetPinGx(), sn.GetTargetPinGy(), sn.GetTargetPinLayer() - 1);
            _sp.BuildRoutingGrid();
            _sp.Dijkstra();
            _sp.GetShortestPath();

            // store path result
            for(unsigned k = 0; k < _sp.shortest_path.size() - 1; ++k) {
                tuple<int, int, int> from = _sp.shortest_path[k];
                int from_x = get<0>(from);
                int from_y = get<1>(from);
                tuple<int, int, int> to = _sp.shortest_path[k + 1];
                int to_x = get<0>(to);
                int to_y = get<1>(to);

                if(k == 0) { 
                    _routing_result[i][j].push_back(tile(from_x, from_y, 1));
                    if(from_x == to_x) _routing_result[i][j].push_back(tile(from_x, from_y, 2));
                }
                if(from_x == to_x) _routing_result[i][j].push_back(tile(to_x, to_y, 2));
                else _routing_result[i][j].push_back(tile(to_x, to_y, 1));

                if(k != _sp.shortest_path.size() - 2) {
                    tuple<int, int, int> next = _sp.shortest_path[k + 2];
                    int next_x = get<0>(next);
                    int next_y = get<1>(next);
                    // vertical direction
                    if(from_x == to_x) {
                        if(to_x != next_x) _routing_result[i][j].push_back(tile(to_x, to_y, 1));
                    }
                    // horizontal direction
                    else {
                        if(to_y != next_y) _routing_result[i][j].push_back(tile(to_x, to_y, 2));
                    }
                }

                if(k == _sp.shortest_path.size() - 2) {
                    if(from_x == to_x) _routing_result[i][j].push_back(tile(to_x, to_y, 1));
                }
            }
        }
    }

    // _sp.PrintEdgeInfo();
}
void router::AdjustCapacity()
{
    for(int i = 0; i < db.GetCapacityAdjustNo(); ++i) {
        CapacityAdjust &adjust_capacity = db.GetCapacityAdjust(i);
        _sp.UpdateCapacity(adjust_capacity.GetGx1(), adjust_capacity.GetGy1(), 0, 
                           adjust_capacity.GetGx2(), adjust_capacity.GetGy2(), 0, adjust_capacity.GetReduceCapacity());
    }
}
void router::Output(ofstream& outputFile)
{
    for(unsigned i = 0; i < _routing_result.size(); ++i) {
        Net& n = db.GetNetByPosition(i);
        int routes_no = 0;
        for(unsigned j = 0; j < _routing_result[i].size(); ++j) {
            routes_no += _routing_result[i][j].size() - 1;
        }
        outputFile << n.GetName() << " " << n.GetUid() << " " << routes_no << endl;
        for(unsigned j = 0; j < _routing_result[i].size(); ++j) {
            for(unsigned k = 0; k < _routing_result[i][j].size() - 1; ++k) {
                tile from = _routing_result[i][j][k];
                tile to = _routing_result[i][j][k + 1];
                outputFile << "(" << from.GetCx() << "," << from.GetCy() << "," << from.GetLayer() << ")-"; 
                outputFile << "(" << to.GetCx() << "," << to.GetCy() << "," << to.GetLayer() << ")" << endl; 
            }
        }
        outputFile << "!" << endl;
    }
}
