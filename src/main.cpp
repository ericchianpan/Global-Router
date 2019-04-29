#include "parser.h"
#include "routingdb.h"
#include "tree.h"
#include "router.h"
#include <string>
#include <cstdlib>

using namespace std;

ifstream inputFile;
ofstream outputFile;

RoutingDB db;


void ShowSyntax()
{
    cout << "syntax: " << endl;
    cout << "./gr [input_file] [output_file] " << endl;
    cout << endl;

    cout << "example: " << endl; 
    cout << "./gr input/adaptec1.capo70.2d.35.50.90.gr a1.out" << endl;

    cout << endl;
}

void HandleArgument(const int argc, char** argv)
{
    if (argc < 3)
    { ShowSyntax(); exit(1); }

    int arg_no = 1;

    /* input file */
    inputFile.open(argv[arg_no], ios::in);
    if (!inputFile)
    {
	cerr << "Could not open input file: " << argv[arg_no] << endl;
	exit(1);
    }

    arg_no++;

    /* output file */
    outputFile.open(argv[arg_no], ios::out);
    if (!outputFile)
    {
	cerr << "Could not open output file: " << argv[arg_no] << endl;
	exit(1);
    }

    arg_no++;
}


int main(int argc, char** argv) 
{
    HandleArgument(argc, argv); 


    {
	/* Parser */
	cout << "[Parser]" << endl;
	Parser parser;
	parser.ReadISPD(inputFile);
    }
    cout << endl;

    /* =================================== */
    /* Show input information after parser */
    // cout << "RoutingDB Info: " << endl;

    // cout << "..# of horizontal global tiles: " 
	// << db.GetHoriGlobalTileNo() << endl;
    // cout << "..# of vertical global tiles: " 
	// << db.GetVertiGlobalTileNo() << endl;
    // cout << "..# of layers: " << db.GetLayerNo() << endl;

    // cout << "..vertical default capacity: "; 
    // for (int i = 0; i < db.GetLayerNo(); i++)
    // { cout << db.GetLayerVertiCapacity(i) << " "; }
    // cout << endl;

    // cout << "..horizontal default capacity: "; 
    // for (int i = 0; i < db.GetLayerNo(); i++)
    // { cout << db.GetLayerHoriCapacity(i) << " "; }
    // cout << endl;

    // cout << "..minimum width: "; 
    // for (int i = 0; i < db.GetLayerNo(); i++)
    // { cout << db.GetLayerMinWidth(i) << " "; }
    // cout << endl;

    // cout << "..minimum spacing: "; 
    // for (int i = 0; i < db.GetLayerNo(); i++)
    // { cout << db.GetLayerMinSpacing(i) << " "; }
    // cout << endl;

    // cout << "..via spacing: "; 
    // for (int i = 0; i < db.GetLayerNo(); i++)
    // { cout << db.GetLayerViaSpacing(i) << " "; }
    // cout << endl;

    // cout << "..chip lower left x: " << db.GetLowerLeftX() << endl; 
    // cout << "..chip lower left y: " << db.GetLowerLeftY() << endl; 
    // cout << "..global tile width: " << db.GetTileWidth() << endl; 
    // cout << "..global tile height: " << db.GetTileHeight() << endl; 

    // cout << "..# of capacity adjustment: " << db.GetCapacityAdjustNo() << endl;
    // for (int i = 0; i < db.GetCapacityAdjustNo(); i++)
    // {
	// CapacityAdjust& ca = db.GetCapacityAdjust(i);
	// cout << "...." << ca.GetGx1() << " " << ca.GetGy1() << " " << ca.GetLayer1() 
		// << " " << ca.GetGx2() << " " << ca.GetGy2() << " " << ca.GetLayer2() 
		// << " " << ca.GetReduceCapacity() << endl;
    // }

    // cout << endl;

    // cout << "..# of net: " << db.GetNetNo() << endl;

    // for (int i = 0; i < db.GetNetNo(); i++)
    // {
	// Net& n = db.GetNetByPosition(i);

	// cout << "..net[" << n.GetUid() << "] " << n.GetName() << endl;
	// cout << "...# of pin: " << n.GetPinNo() << endl;
	// for (int i = 0; i < n.GetPinNo(); i++)
	// {
		// Pin& p = n.GetPin(i);
		// cout << "...."; 
		// // p.ShowInfo();
	// }

	// cout << endl;

    // }

    // cout << endl;
    /* =================================== */



    {
	cout << "[Tree Construction (Net Decomposition)]" << endl;

	RoutingTree tree;
	tree.MinimumSpanningTreeConstruction();

	// tree.ShowInfo();
	cout << endl;
    }
	



    /* =================================== */
    /* Show net(subnet) information after net decomposition */

    // cout << "Net/SubNet Info: " << endl;

    // for (int i = 0; i < db.GetNetNo(); i++)
    // {
    // Net& n = db.GetNetByPosition(i);

    // cout << "..net[" << n.GetUid() << "] " << n.GetName() << endl;

    // cout << "...# of subnets: " << n.GetSubNetNo() << endl;
    // for (int i = 0; i < n.GetSubNetNo(); i++)
    // {
        // SubNet& e = n.GetSubNet(i);
        // cout << "...."; 
        // e.ShowInfo();
    // }
    // cout << endl;
    // }

    // cout << endl;
    /* =================================== */

    {
    cout << "[Route]" << endl;
    router gr;
    gr.Route();
    gr.Output(outputFile);
    }

    {
    cout << "[Verify]" << endl;
    char cmd[100];

    //sprintf(cmd, "./eval2008.pl %s %s", argv[1], argv[2]);
    sprintf(cmd, "./eval.pl %s %s", argv[1], argv[2]);
    cout << cmd << endl;
    system(cmd);
    }

    cout << endl;
    cout << endl;

    return 0;
}




