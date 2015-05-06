//
//  main.cpp
//  DBSCAN
//
//  Created by Foresea on 31/3/15.
//  Copyright (c) 2015 foresea. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class AdjacencyMatrix
{
private:
    int n;
    int **adj;
    bool *visited;
    
public:
    // Constructor
    AdjacencyMatrix(int n)
    {
        this->n = n;
        visited = new bool [n];
        // Mark all the vertices as not visited
        for (int i=0; i<n; i++) {
            visited[i] = false;
        }
        
        // initialize matrix
        adj = new int* [n];
        for (int i = 0; i < n; i++)
        {
            adj[i] = new int [n];
            for(int j = 0; j < n; j++)
            {
                adj[i][j] = 0;
            }
        }
    }
    
    /*
     * Adding Edge to Graph
     */
    void add_edge(int origin, int destin)
    {
        if( origin > n || destin > n || origin < 0 || destin < 0)
        {
            std::cout<<"Invalid edge!\n";
        }
        else
        {
            adj[origin][destin] = 1;
        }
    }
    
    /*
     * Print the graph
     */
    void display()
    {
        int i,j;
        for(i = 0;i < n;i++)
        {
            for(j = 0; j < n; j++)
                std::cout<<adj[i][j]<<"  ";
                std::cout<<std::endl;
        }
    }
    
    /*
     * Breadth First Search
     */
    std::vector<int> BFS(int s)
    {
        std::vector<int> cluster;
        if (!visited[s])
        {
            // Create a queue for BFS
            std::list<int> queue;
            
            // Mark the current node as visited and enqueue it
            visited[s] = true;
            queue.push_back(s);
            
            while (!queue.empty())
            {
                // Dequeue a vertex from queue and print it
                s = queue.front();
                cluster.push_back(s);
                queue.pop_front();
                
                for (int i=0; i<n; i++)
                {
                    // Connect to other vertex and that vertex "i" is not visited
                    if (adj[s][i] != 0 && !visited[i])
                    {
                        visited[i] = true;
                        queue.push_back(i);
                    }
                }
            }
        }
        return cluster;
    }
    
    /*
     * Print the outliers
     */
    std::vector<int> printOutliers()
    {
        std::vector<int> outLiers;
        for(int i=0; i<n; i++){
            if(!visited[i])
                outLiers.push_back(i);
        }
        return outLiers;
    }
};

int main(int argc, char** argv) {
    // Read input parameters from console
    if (argc != 4){
        std::cout<<"Please follow below instruction input the parameters:"<<std::endl;
        std::cout<<"     ./exc.out [dataset path] [<float> radius] [<int> threshold]"<<std::endl;
        std::cout<<"E.g. ./exc.out /Users/Destop/DBSCAN/dataset.txt 2.0 4"<<std::endl<<std::endl;
        std::cout<<"REMINDER: The clusters and outliers output will be in the folder of input dataset."<<std::endl;
    }
    
    std::string infileUrl = argv[1];
    std::ifstream infile(infileUrl);
    float Radius = strtol(argv[2], NULL, 10);
    int Threshold = (int)strtol(argv[3], NULL, 10);
    
    // Parse path of file
    unsigned found = (unsigned)infileUrl.find_last_of("/");
    std::string baseUrl = infileUrl.substr(0, found);
    
    // Read points from dataset file
    std::vector< std::pair<double, double> > pts;
    std::string line;
    double a, b;
    while (!infile.eof())
    {
        infile >> a >> b;
        std::pair<double, double> tmp = std::make_pair(a, b);
        pts.push_back(tmp);
    }
    
    /*
     * Find Core Points
     */
    int mat_size = (int)pts.size();
    std::vector<int> corePts;
    
    for (int i=0; i<mat_size; i++)
    {
        int neighborPtCount = 0;
        
        // Find neighbor points
        for (int j=0; j<mat_size; j++)
        {
            // Check sqrt( (x1-x2)^2 + (y1-y2)^2 ) <= r
            bool isInside = sqrt(pow(pts[i].first-pts[j].first,2)+pow(pts[i].second-pts[j].second,2)) <= Radius;
            if (isInside)
                neighborPtCount++;
        }
        
        // Check if i is core point
        if (neighborPtCount >= Threshold)
        {
            corePts.push_back(i);
        }
    }
    
    /*
     * Generate directed graph
     */
    AdjacencyMatrix dbscan_graph(mat_size);
    for (int i=0; i<corePts.size(); i++)
    {
        int corePt = corePts[i];
        for (int j=0; j<mat_size; j++)
        {
            if (i == j)
                continue;
            bool isInside = sqrt(pow(pts[corePt].first-pts[j].first,2)+pow(pts[corePt].second-pts[j].second,2)) <= Radius;
            if (isInside)
                dbscan_graph.add_edge(corePt, j);
        }
    }
    
    /*
     * Form Clusters (BFS)
     */
    std::vector< std::vector<int> > clusters;   // Store the clusters
    for (int i=0; i<corePts.size(); i++)
    {
        int corePt = corePts[i];
        std::vector<int> cluster = dbscan_graph.BFS(corePt);
        if (!cluster.empty())
            clusters.push_back(cluster);
    }
    
    /*
     * Find outliers
     */
    std::vector<int> outLiers = dbscan_graph.printOutliers();
    
    /*
     * Print clusters and outliers
     */
    int clusterNum = (int)clusters.size();
    // Initial output files
    std::ofstream *outFile = new std::ofstream[clusterNum];
    for (int i=0; i<clusterNum; i++)
    {
        std::ostringstream s;
        s<<baseUrl<<"/Cluster_"<<i+1<<".txt";
        std::string url(s.str());
        outFile[i].open(url);
    }
    
    // Write Clusters to files and console
    for (int i = 0; i<clusterNum; i++)
    {
        std::cout<<"Cluster "<<i+1<<": "<<std::endl;
        for (int j=0; j<clusters[i].size(); j++)
        {
            int point = clusters[i][j];
            std::cout << point << " ";
            outFile[i] << pts[point].first << " " << pts[point].second << std::endl;
        }
        outFile[i].close();
        std::cout<<std::endl<<std::endl;
    }
    
    // Write Outliers to files and console
    std::ostringstream os;
    os<<baseUrl<<"/Outliers.txt";
    std::string olUrl(os.str());
    std::ofstream outLierUrl(olUrl);
    if (outLiers.empty())
    {
        std::cout<<"No outliers"<<std::endl;
        outLierUrl<<"No outliers"<<std::endl;
        outLierUrl.close();
    }
    else
    {
        std::cout<<"Outliers are:"<<std::endl;
        for (int i=0; i<outLiers.size(); i++) {
            int olPoint = outLiers[i];
            std::cout<<olPoint<<" ";
            outLierUrl<<pts[olPoint].first<<" "<<pts[olPoint].second<<std::endl;
        }
        outLierUrl.close();
        std::cout<<std::endl;
    }
    
    return 0;
}
