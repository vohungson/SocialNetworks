//
//  main.cpp
//  SocialNetworks
//
//  Created by Vo Hung Son on 06/06/2019.
//  Copyright © 2019 Vo Hung Son. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cstdio>
#include <ctime>
using namespace std;

class Graph {
private:
    int V;//The number of users (vertices)
    int numberOfCommunities = 0;
    int number = 0;
    bool *existVertex;

public:
    void addEdge(vector<int> adj[], int u, int v){
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void printGraph(vector<int> adj[], int V){
        for (int v = 0; v < V; ++v){
            cout << "\n Adjacency list of vertex " << v << " : ";
            for (auto x : adj[v]) cout << " -> " << x;
        }
    }

    bool existEdge(vector<int> adj[], int u, int v){//the edge exist ?
        for (auto x : adj[u]){
            if (x == v) return true;
        }
        return false;
    }

    void countEvenDegree(vector<int> adj[]) {
        int countVertices = 0;
        for (int u = 0; u < V; u ++){
            if(adj[u].size() %2 ==0) countVertices++;
        }
        cout<<"\n   Number of even vertices: "<<countVertices;
    }

    //check exist community by checking the connection of all edges
    bool existCommunity (vector<int> adj[]) {
        bool community = true;
        for (int u = 0; u < V - 1; u ++){
            while(!existVertex[u]) {u++;}
            for (int v = u + 1; v < V; v++){
                while(!existVertex[v]) {v++;}
                if(v < V){
                    if(!existEdge(adj, u, v)) community = false;
                }
            }
        }
        return community;
    }
    
    int numberOfVertices(){//cout the number of Vertices.
        int numberVertices = 0;
        for(int i = 0; i < V; i ++){
            if(existVertex[i]) numberVertices++;
        }
        return numberVertices;
    }
    
    void minusOne (){
        if(existVertex[V-1]) {
            existVertex[V-1] = false;
        } else {
            int i = V - 1;
            while (!existVertex[i]){
                existVertex[i] = true;
                i--;
            }
            existVertex[i] = false;
        }
    }
    
    void shiftToLeft(){//example: 10101->10011, 10011->01111
        //the bit zero shift to left, other digits on the right is 1
        //this algorithm we use when we found out the community and
        //we want to cancel the sub graph inside the community
        int j = 0;
        for (int i = V - 1; i > 0; i --){
            if(!existVertex[i] && existVertex[i-1]){
                j = i;
                existVertex[i-1] = false;
                break;
            }
        }
        if(j != 0){
            for (int i = j; i < V; i ++){
                existVertex[i] = true;
            }
        } else {//set all digits are false when we can not shift to left
            for (int i = 0; i < V; i ++){
                existVertex[i] = false;
            }
        }
    }
    
    void findCommunities(vector<int> adj[]){
        while (numberOfVertices() >= 3){
            //step 1: if graph is a community -> numberOfCommunities ++
            bool community = existCommunity(adj);
            //step 2: else use recursive to find communities in sub graph.
            if(!community){//if false ->find community in sub graph
                if(numberOfVertices() > 3){
                    minusOne();
                } else {
                    shiftToLeft();
                }
            } else { //community = true -> find out the community here
                numberOfCommunities ++;
                shiftToLeft();//cancel the sub graph inside the community
            }
        }
    }

    void randomNetwork(int nUsers, int nFriends){
        cout<<"1. RandomNetwork (nUsers = "<<nUsers<<", nFriends = " <<nFriends<<")";
        clock_t start, end;
        start = clock();
        V = nUsers;
        nFriends = min(nUsers, nFriends);
        vector<int> adj[V];
        for (int u = 0; u < V; u++){//V = nUsers
            for(int i = 0; i < nFriends; i++){
                int v = rand() % (nUsers - 1);// get the vertex v
                while (v == u){
                    v = rand() % (nUsers - 1);
                }
                if(!existEdge(adj, u, v)) addEdge(adj, u, v);
            }
        }
        end = clock();
        double time_taken1 = double(end-start)/double(CLOCKS_PER_SEC);
        cout<<"\n-> Time for random network: "<<fixed<< time_taken1<<"\n";
        //printGraph(adj, V);
    }

    void improvedNetwork(int nCommunities, int nMembers){
        cout<<"2. ImprovedNetwork (nCommunities = "<<nCommunities<<", nFriends = "<<nMembers<<")";
        clock_t start, end;
        start = clock();
        V = nCommunities*nMembers;//numbers of Users
        vector<int> adj[V];
        for (int i = 0; i < nCommunities; i++){
            for(int u = 0; u < nMembers - 1; u++){
                for (int v = u + 1; v < nMembers; v++){
                        addEdge(adj,i*nMembers + u,i*nMembers + v);
                }
            }
            if(i != 0){ //add link between these communities
                addEdge(adj,(i-1)*nMembers,i*nMembers);
            }
        }
        //printGraph(adj, V);
        end = clock();
        double time_taken1 = double(end-start)/double(CLOCKS_PER_SEC);
        cout<<"\n-> Time for improved network: "<<fixed<<time_taken1<<"\n";

        //3. Count the number of vertices whose degree are even
        start = clock();
        cout<<"3. Count the number of vertices whose degree are even";
        countEvenDegree(adj);
        end = clock();
        double time_taken2 = double(end-start)/double(CLOCKS_PER_SEC);
        cout<<"\n-> Time to find even degree: "<<fixed<<time_taken2;

        //4. Develop an algorithm to find all of communities
        start = clock();
        existVertex = new bool[V];
        for(int i = 0; i < V; i ++){
            existVertex[i] = true;
        }
        findCommunities(adj);
        cout<<"\n4. Number of communities: "<<numberOfCommunities;
        end = clock();
        double time_taken3 = double(end-start)/double(CLOCKS_PER_SEC);
        cout<<"\n-> Time to find communities: "<<fixed<<time_taken3<<"\n";
    }
};

int main(int argc, const char * argv[]) {
    // insert code here...
    Graph graph;
    //1. Generate a random social network, represented as a graph.
    graph.randomNetwork(10, 2);
    //2. Generate a network with N communities, each has M members
    graph.improvedNetwork(7, 3); //maximum is 10k users
    return 0;
}
