#ifndef router_h
#define router_h
#include "routingdb.h"
#include "lib/graphsp.h"

extern RoutingDB db;

class tile
{
    public:
        tile() {;}
        tile(int gx, int gy, int layer): _gx(gx), _gy(gy), _layer(layer) {;}
        ~tile() {;}

        friend std::ostream &operator<<(std::ostream &output, const tile &t) {
            output<< "(" << t._gx << ", " << t._gy << ", " << t._layer << ")";
            return output;
        }
        bool operator== (tile const &rhs) {
            return this->_gx == rhs._gx && this->_gy == rhs._gy && this->_layer == rhs._layer;
        }

        int GetGx() { return _gx; }
        int GetGy() { return _gy; }
        int GetLayer() { return _layer; }
        int GetCx() { return db.GetLowerLeftX() + _gx * db.GetTileWidth() + db.GetTileWidth() / 2; }
        int GetCy() { return db.GetLowerLeftY() + _gy * db.GetTileHeight() + db.GetTileHeight() / 2; }

    private:
        int _gx;
        int _gy;
        int _layer;
};

class router
{
    public:
        router();
        ~router();

        void Route();
        void AdjustCapacity();
        void Output(ofstream& outputFile);

    private:
        vector<vector<vector<tile>>> _routing_result;
        graphsp _sp;
};

#endif
