#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>

class Graph {
public:
    Graph(int n);
    void addEdge(int u, int v);
    void print();
    std::vector<int> bfs(int source, int target);
    std::vector<int> dfs(int source, int target);

private:
    int num_vertices;
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<int>> list;
    std::vector<int> reconstructPath(const std::vector<int>& ant, int source, int target);
};

Graph::Graph(int n) : num_vertices(n) {
    matrix.resize(n, std::vector<int>(n, 0));
    list.resize(n);
}

void Graph::addEdge(int u, int v) {
    matrix[u][v] = 1;
    matrix[v][u] = 1;
    list[u].push_back(v);
    list[v].push_back(u);
}

void Graph::print() {
    std::cout << "Matriz de Adjacência:" << std::endl;
    for (const auto& row : matrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Lista de Adjacência:" << std::endl;
    for (int i = 0; i < num_vertices; ++i) {
        std::cout << "Vértice " << i << ":";
        for (int neighbor : list[i]) {
            std::cout << " " << neighbor;
        }
        std::cout << std::endl;
    }
}

std::vector<int> Graph::bfs(int source, int target) {
    std::vector<int> dist(num_vertices, -1);
    std::vector<int> ant(num_vertices, -1);
    std::vector<bool> isVisited(num_vertices, false);
    std::queue<int> Q;
    Q.push(source);
    isVisited[source] = true;
    dist[source] = 0;

    while (!Q.empty()) {
        int p = Q.front();
        Q.pop();

        for (int v : list[p]) {
            if (!isVisited[v]) {
                dist[v] = dist[p] + 1;
                ant[v] = p;
                Q.push(v);
                isVisited[v] = true;
                if (v == target) {
                    return reconstructPath(ant, source, target);
                }
            }
        }
    }

    return std::vector<int>();
}

std::vector<int> Graph::dfs(int source, int target) {
    std::vector<bool> visited(num_vertices, false);
    std::vector<int> stack;
    stack.push_back(source);
    visited[source] = true;

    while (!stack.empty()) {
        int vertex = stack.back();
        stack.pop_back();

        for (int v : list[vertex]) {
            if (!visited[v]) {
                stack.push_back(v);
                visited[v] = true;

                if (v == target) {
                    return reconstructPath(stack, source, target);
                }
            }
        }
    }

    return std::vector<int>();
}

std::vector<int> Graph::reconstructPath(const std::vector<int>& ant, int source, int target) {
    std::vector<int> path;
    int current = target;

    while (current != -1) {
        path.insert(path.begin(), current);
        current = ant[current];
    }

    if (path[0] == source) {
        return path;
    } else {
        return std::vector<int>();
    }
}

Graph loadGraphFromFile(std::string file_path) {
    std::ifstream file(file_path);
    int num_vertices;
    file >> num_vertices;
    Graph g(num_vertices);

    int u, v;
    while (file >> u >> v) {
        g.addEdge(u, v);
    }

    return g;
}

int main() {
    Graph graph = loadGraphFromFile("grafo.txt");
    graph.print();

    int source = 0;
    int target = 3;

    std::vector<int> bfs_path = graph.bfs(source, target);
    if (!bfs_path.empty()) {
        std::cout << "Caminho BFS de " << source << " para " << target << ": ";
        for (int vertex : bfs_path) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Não há caminho entre " << source << " e " << target << "." << std::endl;
    }

    std::vector<int> dfs_path = graph.dfs(source, target);
    if (!dfs_path.empty()) {
        std::cout << "Caminho DFS de " << source << " para " << target << ": ";
        for (int vertex : dfs_path) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Não há caminho entre " << source << " e " << target << "." << std::endl;
    }

    return 0;
}
