    #ifndef _MYGRAPH_H_
    #define _MYGRAPH_H_

    #include <iostream>
    #include <fstream>

    #include "MyVector_w125t659.h"
    #include "MyLinkedList_w125t659.h"
    #include "MyQueue_w125t659.h"
    #include "MyStack_w125t659.h"
    #include "MyHashTable_w125t659.h"


    // the graph is supposed to be a undirected graph
    // no multi-edge is allowed (assumed to be ensured from the input)

    typedef long long VertexIDType;
    typedef long long EdgeIDType;

    template <typename VertexDataType, typename EdgeDataType>
    class MyGraph
    {
    public:

        class Vertex
        {
        public: 
            // default constructor
            Vertex()
            {
                return;
            }

            // vertex copy constructor
            explicit Vertex(const VertexDataType& vertex_data) :
                data(vertex_data)
            {
                return;
            }

            // vertex move constructor
            explicit Vertex(VertexDataType && vertex_data) :
                data(std::move(vertex_data))
            {
                return;
            }

            // copy assignment
            Vertex& operator=(const Vertex& rhs)
            {
                id = rhs.id;
                tag = rhs.tag;
                data = rhs.data;
                return *this;
            }

            // move assignment
            Vertex& operator=(Vertex && rhs)
            {
                id = rhs.id;
                tag = rhs.tag;
                data = std::move(rhs.data);
                return *this;
            }

            VertexIDType id;        // the unique ID of the vertex
            int tag;                // a reserved tag variable
            VertexDataType data;    // the property associated with the vertex

            friend class MyGraph;

        };

        class Edge
        {
        public:
            // default constructor
            Edge()
            {
                return;
            }

            // copy constructor
            // sid: source vertex ID; vid: target vertex ID
            explicit Edge(const VertexIDType sid, const VertexIDType tid, const EdgeDataType& edge_data) :
                src(sid),
                tgt(tid),
                data(edge_data)
            {
                return;
            }

            // move constructor
            // sid: source vertex ID; vid: target vertex ID
            explicit Edge(const VertexIDType sid, const VertexIDType tid, EdgeDataType && edge_data) :
                src(sid),
                tgt(tid),
                data(std::move(edge_data))
            {
                return;
            }

            // copy assignment
            Edge& operator=(const Edge& rhs)
            {
                id = rhs.id;
                src = rhs.src;
                tgt = rhs.tgt;
                tag = rhs.tag;
                data = rhs.data;
                return *this;
            }

            // move assignment
            Edge& operator=(Edge && rhs)
            {
                id = rhs.id;
                src = rhs.src;
                tgt = rhs.tgt;
                tag = rhs.tag;
                data = std::move(rhs.data);
                return *this;
            }
        
            EdgeIDType id;          // the unique ID of the edge
            VertexIDType src, tgt;  // the IDs for the source vertex and the target vertex of the edge
            int tag;                // the reserved tag varaible
            EdgeDataType data;      // the property associated with the edge

            friend class MyGraph;

        };

        // default constructor of graph
        // initialize the graph with a size of 0
        MyGraph() :
            current_vertex_ID(0),
            current_edge_ID(0),
            num_vertices(0),
            num_edges(0),
            vertex_set{0},
            edge_set{0},
            vertex_map{},
            edge_map{},
            adj_list{}
        {   
            return;
        }

        // destructor
        ~MyGraph()
        {
            for(size_t i = 0; i < vertex_set.size(); ++ i)
            {
                delete vertex_set[i];
            }
            for(size_t i = 0; i < edge_set.size(); ++ i)
            {
                delete edge_set[i];
            }
            // std::cout << "delete adj_list" << std::endl;
            // for(size_t i = 0; i < adj_list.size(); ++ i)
            // {
            //     if(adj_list[i]){
            //         adj_list[i]->clear();
            //         delete adj_list[i];
            //     }else{
            //         std::cout << "delete broken at " << i << std::endl; 
            //         // break;
            //     }
            // }
            return;
        }

        // adds an vertex v to the graph (copy)
        // assign ID according the to oder of vertex insertion
        // for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex
        // returns the assigned vertex ID for v
        VertexIDType addVertex(const VertexDataType& v_data)
        {
            Vertex* new_vertex = new Vertex(v_data);
            new_vertex->id = current_vertex_ID;
            adj_list.push_back(new MyLinkedList<EdgeIDType>);

            // store data
            vertex_set.push_back(new_vertex);
            HashedObj<VertexIDType, size_t> vertex_entry(current_vertex_ID, vertex_set.size() - 1);
            vertex_map.insert(std::move(vertex_entry));

            // update counters
            ++num_vertices;
            return current_vertex_ID++;
        }

        // adds an vertex v to the graph (move)
        // assign ID according the to oder of vertex insertion
        // for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex
        // returns the assigned vertex ID for v
        VertexIDType addVertex(VertexDataType && v_data)
        {
            Vertex* new_vertex = new Vertex(std::move(v_data));
            new_vertex->id = current_vertex_ID;
            adj_list.push_back(new MyLinkedList<EdgeIDType>);

            // store data
            vertex_set.push_back(new_vertex);
            HashedObj<VertexIDType, size_t> vertex_entry(current_vertex_ID, vertex_set.size() - 1);
            vertex_map.insert(std::move(vertex_entry));

            // update counters
            ++num_vertices;
            return current_vertex_ID++;
        }

        // retrieves the information of the vertex specified by the vid
        // returns the vertex
        Vertex* getVertex(const VertexIDType vid)
        {
            // vertex_map contains key
            HashedObj<VertexIDType, size_t> mapping;
            if(!vertex_map.retrieve(vid, mapping)){
                return nullptr;
            }

            // check bounds
            if(mapping.value >= vertex_set.size() || vertex_set[mapping.value] == nullptr){
                return nullptr;
            }

            return vertex_set[mapping.value];
        }

        // adds an edge e between the source node src and the target node tgt (copy)
        // adds to both of the src's adjacency list and tgt's adjacency list (as undirected graph)
        // assign ID according the to oder of edge insertion
        // for example, assign ID 1 to the first edge, 2 to the second edge, ..., and n to the nth edge
        // returns the assigned edge ID for e
        EdgeIDType addEdge(const VertexIDType sid, const VertexIDType tid, const EdgeDataType& e_data)
        {
            // find vertices
            Vertex* v1 = getVertex(sid);
            Vertex* v2 = getVertex(tid);

            // check if either vertex doesn't exist or is the same vertex
            if(!v1 || !v2 || sid == tid){
                return -1;
            }

            // create edge
            Edge* new_edge = new Edge(e_data);
            new_edge->id = current_edge_ID;
            new_edge->src = sid;
            new_edge->tgt = tid;

            // store data
            edge_set.push_back(new_edge);
            HashedObj<EdgeIDType, size_t> edge_entry(current_edge_ID, edge_set.size() - 1);
            edge_map.insert(std::move(edge_entry));

            // get indices of vertex
            HashedObj<VertexIDType, size_t> mapping1, mapping2;
            if (!vertex_map.retrieve(sid, mapping1) || !vertex_map.retrieve(tid, mapping2)) {
                return -1;
            }

            // connect source and target vertex via new edge
            adj_list[mapping1.value]->push_back(new_edge->id);
            adj_list[mapping2.value]->push_back(new_edge->id);

            // update counters
            ++num_edges;
            return current_edge_ID++;
        }

        // adds an edge e between the source node src and the target node tgt (copy)
        // adds to both of the src's adjacency list and tgt's adjacency list (as undirected graph)
        // assign ID according the to oder of vertex insertion
        // for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex
        // returns the assigned edge ID for e
        EdgeIDType addEdge(const VertexIDType sid, const VertexIDType tid, EdgeDataType && e_data)
        {
            Vertex* v1 = getVertex(sid);
            Vertex* v2 = getVertex(tid);

            // check if either vertex doesn't exist or is the same vertex
            if(!v1 || !v2 || sid == tid){
                return -1;
            }

            // create edge
            Edge* new_edge = new Edge(sid, tid, e_data);
            new_edge->id = current_edge_ID;
            new_edge->src = sid;
            new_edge->tgt = tid;

            // store data
            edge_set.push_back(new_edge);
            HashedObj<EdgeIDType, size_t> edge_entry(current_edge_ID, edge_set.size() - 1);
            edge_map.insert(std::move(edge_entry));

            // get indices of vertex
            HashedObj<VertexIDType, size_t> mapping1, mapping2;
            if (!vertex_map.retrieve(sid, mapping1) || !vertex_map.retrieve(tid, mapping2)) {
                return -1;
            }

            // connect source and target vertex via new edge
            adj_list[mapping1.value]->push_back(new_edge->id);
            adj_list[mapping2.value]->push_back(new_edge->id);

            // update counters
            ++num_edges;
            return current_edge_ID++;
        }

        // retrieves the information of the edge specified by the eid
        // returns the edge
        Edge* getEdge(const EdgeIDType eid)
        {
            // edge_map contains key
            HashedObj<EdgeIDType, size_t> mapping;
            if(!edge_map.retrieve(eid, mapping)){
                return nullptr;
            }

            // check bounds
            if(mapping.value >= edge_set.size() || edge_set[mapping.value] == nullptr){
                return nullptr;
            }

            return edge_set[mapping.value];
        }

        // checks whether an exists between src and tgt
        // if yes, return TRUE and record the corresponding edge ID in eid
        // if no, return FALSE
        bool probeEdge(const VertexIDType sid, const VertexIDType tid, EdgeIDType& eid)
        {
            // find sid and tid in maps
            HashedObj<VertexIDType, size_t> mapping1, mapping2;
            if (!vertex_map.retrieve(sid, mapping1) || !vertex_map.retrieve(tid, mapping2)) {
                return false;
            }

            // find each list
            MyLinkedList<EdgeIDType>* list1 = adj_list[mapping1.value];
            MyLinkedList<EdgeIDType>* list2 = adj_list[mapping2.value];

            // get shorter of two lists
            MyLinkedList<EdgeIDType>* shorter = (list1->size() <= list2->size()) ? list1 : list2;

            // iterate through shorter list
            for (auto it = shorter->begin(); it != shorter->end(); ++it){
                Edge* current_edge = getEdge(*it);
                if(current_edge && ((current_edge->src == sid && current_edge->tgt == tid) || (current_edge->src == tid && current_edge->tgt == sid))){
                    eid = *it;
                    return true;
                }
            }

            // edge does not exist between sid and tid
            return false;
        }

        // returns the degree of the vertex specified by vid
        size_t degree(const VertexIDType vid)
        {
            HashedObj<VertexIDType, size_t> mapping;
            if (!vertex_map.retrieve(vid, mapping)) {
                std::cout << "Vertex not found" << std::endl;
                return 0;
            }

            if (!adj_list[mapping.value]) {
                std::cout << "Vertex exists in map but has no adjacency list" << std::endl;
                return 0;
            }

            return adj_list[mapping.value]->size();
        }

        // deletes the vertex specified by vid from the graph
        // also delete all edges associated with it
        // expected time complexity: O(d^2), where d is the average degree of the graph
        void deleteVertex(const VertexIDType vid) {
            // Find vertex in map
            HashedObj<VertexIDType, size_t> mapping;
            if (!vertex_map.retrieve(vid, mapping)) {
                std::cout << "Vertex not found" << std::endl;
                return;
            }
        
            size_t v_index = mapping.value;
            MyLinkedList<EdgeIDType>* list = adj_list[v_index];
        
            // Make a copy of the edge list since we will modify it while iterating
            MyVector<EdgeIDType> edges_to_remove;
            for (auto it = list->begin(); it != list->end(); ++it) {
                edges_to_remove.push_back(*it);
            }
        
            for (size_t i = 0; i < edges_to_remove.size(); ++i) {
                deleteEdge(edges_to_remove[i]);  // reuse deleteEdge logic
            }
        
            // Delete vertex and cleanup
            delete list;
            adj_list[v_index] = nullptr;
        
            delete vertex_set[v_index];
            vertex_set[v_index] = nullptr;
        
            vertex_map.remove(vid);
            --num_vertices;
        }

        // deletes the edge specified by eid from the graph
        // expected time complexity: O(d), where d is the average degree of the graph
        void deleteEdge(const EdgeIDType eid) {
            Edge* edge = getEdge(eid);
            if (!edge) {
                std::cout << "Edge not found" << std::endl;
                return;
            }
        
            // Lookup source and target indices
            HashedObj<VertexIDType, size_t> src_mapping, tgt_mapping;
            if (!vertex_map.retrieve(edge->src, src_mapping) || 
                !vertex_map.retrieve(edge->tgt, tgt_mapping)) {
                std::cout << "One or both vertices not found" << std::endl;
                return;
            }
        
            MyLinkedList<EdgeIDType>* src_list = adj_list[src_mapping.value];
            MyLinkedList<EdgeIDType>* tgt_list = adj_list[tgt_mapping.value];
        
            // Erase edge from src list
            for (auto it = src_list->begin(); it != src_list->end(); ) {
                if (*it == eid) {
                    it = src_list->erase(it);  // erase returns next valid iterator
                } else {
                    ++it;
                }
            }
        
            // Erase edge from tgt list
            for (auto it = tgt_list->begin(); it != tgt_list->end(); ) {
                if (*it == eid) {
                    it = tgt_list->erase(it);
                } else {
                    ++it;
                }
            }
        
            delete edge;
            edge_set[eid] = nullptr;
            edge_map.remove(eid);
            --num_edges;
        }

        // performs breath-first-search on the graph, starting from v_src
        // record the path in path
        // outgoing edge visit order should be determined based on their ID (smaller ID visited earlier)
        // the source vertex should also be included in the traversal path
        void breadthFirstSearch(const VertexIDType v_src, MyVector<VertexIDType>& path)
        {
            Vertex* vertex = getVertex(v_src);
            if(!vertex){
                std::cout << "Vertex not found" << std::endl;
                return;
            }

            MyHashTable<VertexIDType, bool> visited;
            MyQueue<VertexIDType> queue;

            visited.insert({v_src, true});
            queue.enqueue(v_src);
            path.push_back(v_src);

            while(!queue.empty()){
                VertexIDType current = queue.front();
                queue.dequeue();

                // obtain adj_list
                HashedObj<VertexIDType, size_t> mapping;
                if (!vertex_map.retrieve(current, mapping)) {
                    continue;
                }

                MyLinkedList<EdgeIDType>* edges = adj_list[mapping.value];

                // sort linked list into vector
                MyVector<EdgeIDType> edge_ids;
                for (auto it = edges->begin(); it != edges->end(); ++it) {
                    edge_ids.push_back(*it);
                }

                // sort edges by ID (ascending)
                std::sort(edge_ids.begin(), edge_ids.end());

                // search through every edge adjacent to current
                for(size_t i = 0; i < edge_ids.size(); ++i){
                    Edge* edge = getEdge(edge_ids[i]);
                    if(!edge){
                        continue;
                    }
                
                    // determine other vertex
                    VertexIDType neighbor = (edge->src == current) ? edge->tgt : edge->src;

                    // add vertex if not visited
                    if (!visited.contains(neighbor)) {
                        visited.insert({neighbor, true});
                        queue.enqueue(neighbor);
                        path.push_back(neighbor);
                    }
                }
            }
        }

        // performs depth-first-search on the graph, starting from v_src
        // record the path in path
        // outgoing edge visit order should be determined based on their ID (larger ID visited earlier)
        // the source vertex should also be included in the traversal path
        void depthFirstSearch(const VertexIDType v_src, MyVector<VertexIDType>& path)
        {
            Vertex* vertex = getVertex(v_src);
            if(!vertex){
                std::cout << "Vertex not found" << std::endl;
                return;
            }

            MyHashTable<VertexIDType, bool> visited;
            MyStack<VertexIDType> stack;

            visited.insert({v_src, true});
            stack.push(v_src);
            path.push_back(v_src);

            while(!stack.empty()){
                VertexIDType current = stack.top();

                // obtain adj_list
                HashedObj<VertexIDType, size_t> mapping;
                if (!vertex_map.retrieve(current, mapping)) {
                    stack.pop();
                    continue;
                }

                MyLinkedList<EdgeIDType>* edges = adj_list[mapping.value];

                // sort linked list into vector
                MyVector<EdgeIDType> edge_ids;
                for (auto it = edges->begin(); it != edges->end(); ++it) {
                    edge_ids.push_back(*it);
                }

                // sort edges by ID (ascending)
                std::sort(edge_ids.begin(), edge_ids.end(), std::greater<EdgeIDType>());

                bool pushed = false;
                for(size_t i = 0; i < edge_ids.size(); ++i){
                    Edge* edge = getEdge(edge_ids[i]);
                    if(!edge){
                        continue;
                    }

                    // determine other vertex
                    VertexIDType neighbor = (edge->src == current) ? edge->tgt : edge->src;

                    // if not visited, mark and push to stack
                    if (!visited.contains(neighbor)) {
                        visited.insert({neighbor, true});
                        stack.push(neighbor);
                        path.push_back(neighbor);
                        pushed = true;
                        break;
                    }
                }

                // if no unvisited neighbors left, pop the stack
                if (!pushed) {
                    stack.pop();
                }
            }
        }

        // determines whether the two nodes vid1 and vid2 are connected
        // if yes, return TRUE
        // if no, return FASLE
        bool isConnected(const VertexIDType vid1, const VertexIDType vid2)
        {
            if (vid1 == vid2) {
                return true;
            }

            MyVector<VertexIDType> path;
            breadthFirstSearch(vid1, path);

            for(size_t i = 0; i < path.size(); ++i){
                // std::cout << path[i] << " = " << vid2 << " ";
                if (path[i] == vid2) {
                    // std::cout << std::endl;
                    return true;
                }
            }
            // std::cout << std::endl;
            return false;
        }

        // returns the number of vertices in the graph
        size_t numVertices(void) const
        {
            return num_vertices;
        }

        // returns the number of edges in the graph
        size_t numEdges(void) const
        {
            return num_edges;
        }

        // checks whether the graph is empty (no vertex)
        bool empty(void) const
        {
            return (num_vertices == 0);
        }

        // prints the content in vertex_set
        void printInfoVertexSet(void) 
        {
            for(size_t i = 0; i < num_vertices; ++ i)
            {
                std::cout << "Array Position: " << i << "\t";
                std::cout << "Vertex ID: " << vertex_set[i]->id << "\t";
                std::cout << "Vertex data: " << vertex_set[i]->data << std::endl;
            }
            return;
        }

        // prints the content in edge_set
        void printInfoEdgeSet(void) 
        {
            for(size_t i = 0; i < num_edges; ++ i)
            {
                std::cout << "Array Position: " << i << "\t";
                std::cout << "Edge ID: " << edge_set[i]->id << "\t";
                std::cout << "Edge ends: " << edge_set[i]->src << "\t" << edge_set[i]->tgt << "\t";
                std::cout << "Edge data: " << edge_set[i]->data << std::endl;
            }
            return;
        }

        // prints the content in vertex_map
        void printInfoVertexMap(void) 
        {
            MyVector<VertexIDType> v_keys;
            vertex_map.keys(v_keys);
            for(size_t i = 0; i < v_keys.size(); ++ i)
            {
                std::cout << "VertexID-Position:\t" << v_keys[i] << "\t" << vertexID2SetPos(v_keys[i]) << std::endl;
            }
            return;
        }

        // prints the content in edge_map
        void printInfoEdgeMap(void) 
        {
            MyVector<EdgeIDType> e_keys;
            edge_map.keys(e_keys);
            for(size_t i = 0; i < e_keys.size(); ++ i)
            {
                std::cout << "EdgeID-Position:\t" << e_keys[i] << "\t" << edgeID2SetPos(e_keys[i]) << std::endl;
            }
            return;
        }

        // prints the content in the adjacency list
        void printAdjList(void) 
        {
            // std::cout << vertex_set[0] << std::endl;
            // if(vertex_set[0] == nullptr){
            //     std::cout << "nullptr" << std::endl;
            // }
            // std::cout << vertex_set[0]->id << std::endl;
            // std::cout << "printing adj list" << std::endl;
            for(size_t i = 0; i < adj_list.size(); ++ i)
            {
                if(vertex_set[i] == nullptr || adj_list[i] == nullptr){
                    // std::cout << "nullptr" << std::endl;
                    continue;
                }
                std::cout << "Vertex: " << vertex_set[i]->id << " degree: " << degree(vertex_set[i]->id) << "\tIt is connected with: ";
                for(auto itr = adj_list[i]->begin(); itr != adj_list[i]->end(); ++ itr)
                {
                    if(vertex_set[i]->id == getEdge(*itr)->src)
                        std::cout << getEdge(*itr)->tgt << "\t";
                    else if(vertex_set[i]->id == getEdge(*itr)->tgt)
                        std::cout << getEdge(*itr)->src << "\t";
                    else
                        throw std::domain_error("MyGraph::printAdjList: edge information may have been corrupted.");
                }
                std::cout << std::endl;
            }
        }

    private:
        VertexIDType current_vertex_ID;                 // the ID to be assigned to the next vertex (ID are assigned based on the order of insertion)
        EdgeIDType current_edge_ID;                     // the ID to be assigned to the next edge (IDs are assigned based on the order of insertion)
        size_t num_vertices;                            // the number of vertices
        size_t num_edges;                               // the number of edges
        MyVector<Vertex*> vertex_set;                   // the set of vertices
        MyVector<Edge*> edge_set;                       // the set of edges
        MyHashTable<VertexIDType, size_t> vertex_map;   // the mapping between a vertex ID and its index in vertex_set
        MyHashTable<EdgeIDType, size_t> edge_map;       // the mapping between an edge ID and its index in vertex_set
        MyVector<MyLinkedList<EdgeIDType>* > adj_list;  // the adjacency list (stores the IDs for the corresponding edges)

        // maps the vertex ID to its position in the vertex_set array
        size_t vertexID2SetPos(const VertexIDType vid)
        {
            HashedObj<VertexIDType, size_t> mapping;
            if(vertex_map.retrieve(vid, mapping) && mapping.value < vertex_set.size()) {
                return mapping.value;
            }

            return -1;
        }

        // maps the edge ID to its position in the edge_set array
        size_t edgeID2SetPos(const EdgeIDType eid)
        {
            HashedObj<EdgeIDType, size_t> mapping;
            if(edge_map.retrieve(eid, mapping) && mapping.value < edge_set.size()){
                return mapping.value;
            }

            return 0;
        }

        // gets the vertex ID for one in a given position in the vertex_set array
        VertexIDType vertexSetPos2ID(const size_t vpos)
        {
            if (vpos >= vertex_set.size()){
                return -1;
            }
            return vertex_set[vpos]->id;
        }

        // gets the edge ID for one in a given position in the edge_set array
        EdgeIDType edgeSetPos2ID(const size_t epos)
        {
            if (epos >= edge_set.size()){
                return -1;
            }
            return edge_set[epos]->id;
        }
    
    };

    #endif  // _MYGRAPH_H_