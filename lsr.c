#include <stdio.h>

#define MAX 20        // Maximum number of nodes
#define INF 9999      // Represents infinity (no direct link)

int main() {
    int n, i, j, source;
    int cost[MAX][MAX];     // Cost adjacency matrix
    int distance[MAX];      // Shortest distance from source
    int visited[MAX];       // Visited status of nodes
    // Read number of nodes
    printf("Enter number of nodes: ");
    scanf("%d", &n);
    // Read cost adjacency matrix
    printf("Enter the cost matrix (9999 for no link):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
        }
    }
    // Read source node
    printf("Enter source node: ");
    scanf("%d", &source);
    // Initialization
    for (i = 0; i < n; i++) {
        distance[i] = cost[source][i];  // Initial distances
        visited[i] = 0;           // Mark all nodes as unvisited
    }
    distance[source] = 0;  // Distance from source to itself is 0
    visited[source] = 1;    // Mark source as visited

    // Dijkstra’s algorithm
    for (i = 1; i < n; i++) {
        int min = INF;
        int u = -1;
        // Find the unvisited node with minimum distance
        for (j = 0; j < n; j++) {
            if (!visited[j] && distance[j] < min) {
                min = distance[j];
                u = j;
            }
        }
        // Mark the selected node as visited
        visited[u] = 1;
        // Update distance of neighboring nodes
        for (j = 0; j < n; j++) {
            if (!visited[j] && (distance[u] + cost[u][j] < distance[j])) {
                distance[j] = distance[u] + cost[u][j];
            }
        }
    }
    // Display shortest distances
    printf("\nShortest distances from source node %d: ", source);
    for (i = 0; i < n; i++) {
        printf("To node %d = %d\n", i, distance[i]);
    }
    return 0;
}
