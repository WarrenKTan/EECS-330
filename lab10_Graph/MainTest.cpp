#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "MyGraph_w125t659.h"
#include "MyVector_w125t659.h"
#include "MyHashTable_w125t659.h"

using namespace std;

typedef string CityNameType;
typedef unsigned int DistanceType;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "Please provide the graph vertex input file and the graph edge input file (two files)." << endl;
        exit(1);
    }

    MyGraph<CityNameType, DistanceType> graph;
    MyHashTable<CityNameType, VertexIDType> city_hashmap;
    MyVector<CityNameType> cities;

    // testing adding vertices
    ifstream inFile1;
    inFile1.open(argv[1]);
    if(inFile1.is_open())
    {
        string line;
        while(getline(inFile1, line))
        {
            cout << "Adding vertex:\t" << line << endl;
            VertexIDType vid = graph.addVertex(line);   
            cout << "Vertex added with assigned ID:\t" << vid << endl;
            MyGraph<CityNameType, DistanceType>::Vertex* vx = graph.getVertex(vid);
            cout << "Retrieved vertex:\t" << vx->data << endl;

            // adding the vertex to hash table
            HashedObj<CityNameType, VertexIDType> v_record(line, vid);
            city_hashmap.insert(std::move(v_record));
            cities.push_back(line);
        }
        inFile1.close();
    }
    else
    {
        cout << "The graph vertex input file cannot be opened." << endl;
    }

    // testing adding edges
    ifstream inFile2;
    inFile2.open(argv[2]);
    if(inFile2.is_open())
    {
        string line;
        CityNameType city1, city2;
        DistanceType dist;
        while(getline(inFile2, line))
        {
            std::stringstream ss;
            ss.str(line);
            ss >> city1 >> city2 >> dist;

            HashedObj<CityNameType, VertexIDType> v_record_1, v_record_2;
            if(city_hashmap.retrieve(city1, v_record_1) && city_hashmap.retrieve(city2, v_record_2))
            {
                // add the edge
                cout << "Adding edge:\t" << city1 << "\t" << city2 << "\t" << dist << endl;  
                EdgeIDType eid = graph.addEdge(v_record_1.value, v_record_2.value, std::move(dist));
                cout << "Edge added with assigned ID:\t" << eid << endl; 
                MyGraph<CityNameType, DistanceType>::Edge* eg = graph.getEdge(eid);
                cout << "Retrieved edge:\t" << eg->src << "\t" << eg->tgt << "\t" << eg->data << endl;
            }
            else
                throw std::domain_error("Main(MyGraph): adding edge between nodes that do not exist.");

        }
        inFile2.close();
    }
    else
    {
        cout << "The graph edge input file cannot be opened." << endl;
    }

    graph.printAdjList();

    // testing vertex degree
    for(size_t i = 0; i < cities.size(); ++ i)
    {
        HashedObj<CityNameType, VertexIDType> v_record;
        if(city_hashmap.retrieve(cities[i], v_record))
        {
            size_t dg = graph.degree(v_record.value);
            cout << "The degree of vertex " << cities[i] << " is:\t" << dg << endl;
        }
    }

    // testing edge probing and connevtivity
    for(size_t i = 0; i < 1000; ++ i)
    {
        VertexIDType v1, v2;
        v1 = static_cast<VertexIDType>((int) rand() % cities.size());
        v2 = static_cast<VertexIDType>((int) rand() % cities.size());
        if(v1 == v2)    continue;

        HashedObj<CityNameType, VertexIDType> v_record_1, v_record_2;
        if(city_hashmap.retrieve(cities[v1], v_record_1) && city_hashmap.retrieve(cities[v2], v_record_2))
        {                
            EdgeIDType eid;
            if(graph.probeEdge(v_record_1.value, v_record_2.value, eid))
                cout << "The distance between the cities " << cities[v1] << " and " << cities[v2] << " is " << graph.getEdge(eid)->data << endl; 
                
            else
                cout << "No edge exists between the cities " << cities[v1] << " and " << cities[v2] << endl;                 
        }    
        else
            throw std::domain_error("Main(MyGraph): trying to find edges between nodes that do not exist.");

        if(graph.isConnected(v_record_1.value, v_record_2.value))
            cout << "The cities " << cities[v1] << " and " << cities[v2] << " are connected." << endl;
        else
            cout << "The cities " << cities[v1] << " and " << cities[v2] << " are not connected." << endl;

    }

    // testing graph traversal
    MyVector<VertexIDType> path_bfs, path_dfs;
    graph.breadthFirstSearch(0, path_bfs);
    cout << "BFS path: " << endl;
    for(size_t i = 0; i < path_bfs.size(); ++ i)
    {
        cout << path_bfs[i] << "\t";
    }
    cout << endl;
    graph.depthFirstSearch(0, path_dfs);
    cout << "DFS path: " << endl;
    for(size_t i = 0; i < path_dfs.size(); ++ i)
    {
        cout << path_dfs[i] << "\t";
    }
    cout << endl;

    // testing vertex deletion
    MyVector<bool> city_deleted(cities.size());
    for(size_t i = 0; i < city_deleted.size(); ++ i)
        city_deleted[i] = false;

    for(size_t i = 0; i < 1000; ++ i)
    {
        VertexIDType vd = static_cast<VertexIDType>((int) rand() % cities.size());
        if(city_deleted[vd])
            continue;

        HashedObj<CityNameType, VertexIDType> v_record;
        if(city_hashmap.retrieve(cities[vd], v_record))
        {
            graph.deleteVertex(v_record.value);
        }
        city_deleted[vd] = true;
    }
    graph.printInfoVertexMap();
    graph.printInfoEdgeMap();
    graph.printAdjList();
    for(size_t i = 0; i < city_deleted.size(); ++ i)
    {
        if(city_deleted[i])
            continue;

        HashedObj<CityNameType, VertexIDType> v_record;
        if(city_hashmap.retrieve(cities[i], v_record))
        {
            graph.deleteVertex(v_record.value);
        }
        city_deleted[i] = true;
    }
    graph.printInfoVertexMap();
    graph.printInfoEdgeMap();
    graph.printAdjList();
    

    return 0;
}