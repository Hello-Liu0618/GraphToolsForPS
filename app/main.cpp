/*
---------------------------------------------------------------------------------------------
!!!!!
此部分主要由AI生成，但是AI生成的样例中有许多错误和与项目不匹配的地方，因此在AI基础上进行了修正
!!!!!
---------------------------------------------------------------------------------------------
*/
#include <iostream>
#include <vector>
#include <string>
#include <any>
#include <algorithm>
#include "../include/graph/vertex.h"
#include "../include/graph/edge.h"
#include "../include/graph/directedGraph.h"
#include "../include/graph/dynamicDirectedGraph.h"
#include "../include/graph/dynamicUndirectedGraph.h"
#include "../include/graph/algorithm/BinaryGraphCheck.h"
#include "../include/graph/algorithm/DFSCV.h"
#include "../include/graph/algorithm/DFSBlk.h"

#include <windows.h>

// 辅助函数：打印分隔线
void printSeparator(const std::string& title) {
    std::cout << "\n\033[1;34m===== " << title << " =====\033[0m" << std::endl;
}

// 1. 展示Vertex类功能
void demonstrateVertex() {
    printSeparator("Vertex类功能展示");
    Vertex v(1);
    std::cout << "创建顶点ID: " << v.getId() << std::endl;

    // 属性操作
    v.setAttribute("name", std::string("核心节点"));
    v.setAttribute("weight", 5.8);
    v.setAttribute("active", true);
    std::cout << "设置属性后:" << std::endl;
    std::cout << "  name: " << std::any_cast<std::string>(v.getAttribute("name")) << std::endl;
    std::cout << "  weight: " << std::any_cast<double>(v.getAttribute("weight")) << std::endl;
    std::cout << "  active: " << (std::any_cast<bool>(v.getAttribute("active")) ? "是" : "否") << std::endl;

    // 拷贝操作
    Vertex v2(v);
    std::cout << "拷贝顶点ID: " << v2.getId() << ", 拷贝的name属性: " 
              << std::any_cast<std::string>(v2.getAttribute("name")) << std::endl;
}

// 2. 展示Edge类功能
void demonstrateEdge() {
    printSeparator("Edge类功能展示");
    std::pair<int, int> endpoints(1, 2);
    Edge e(100, endpoints, 3.5);  // ID=100, 权重=3.5
    std::cout << "创建边: " << e.getU() << "->" << e.getV() 
              << ", ID: " << e.getId() << ", 权重: " << e.getWeight() << std::endl;

    // 属性操作
    e.setAttribute("capacity", 100);
    std::cout << "边容量属性: " << std::any_cast<int>(e.getAttribute("capacity")) << std::endl;

    // 比较操作
    Edge e2(endpoints, 3.5);  // 同端点同权重不同ID
    std::cout << "与同端点同权重边是否相同: " << (e.isSame(e2) ? "是" : "否") << std::endl;
}

// 3. 展示DirectedGraph类功能
void demonstrateDirectedGraph() {
    printSeparator("DirectedGraph类功能展示");
    DirectedGraph g;

    // 添加顶点
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addVertex(4);
    std::cout << "添加4个顶点后, 顶点数: " << g.numVertices() << std::endl;

    // 添加边
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 1);
    g.addEdge(3, 4, 3);
    std::cout << "添加4条边后, 边数: " << g.numEdges() << std::endl;

    // 邻居查询
    std::vector<int> neighbors = g.neighbors(1);
    std::cout << "顶点1的出邻居: ";
    for (int v : neighbors) std::cout << v << " ";
    std::cout << std::endl;

    // 子图生成
    std::vector<int> subVertices = {1, 2, 3};
    Graph* subGraph = g.subgraph_by_vertices(subVertices);
    std::cout << "由顶点{1,2,3}生成的子图边数: " << subGraph->numEdges() << std::endl;
    delete subGraph;
}

// 4. 展示dynamicDirectedGraph功能
void demonstrateDynamicDirectedGraph() {
    printSeparator("dynamicDirectedGraph动态功能展示");
    dynamicDirectedGraph ddg;

    // 初始化图
    for (int i = 1; i <= 5; ++i) ddg.addVertex(i);
    ddg.addEdge(1, 2);
    ddg.addEdge(2, 3);
    ddg.addEdge(3, 4);
    ddg.addEdge(4, 5);
    ddg.addEdge(5, 1);  // 形成环
    std::cout << "初始图边数: " << ddg.numEdges() << std::endl;

    // 删除顶点
    ddg.removeVertex(3);
    std::cout << "删除顶点3后, 顶点数: " << ddg.numVertices() 
              << ", 边数: " << ddg.numEdges() << std::endl;

    // 补图生成
    dynamicDirectedGraph complement = ddg.getComplement();
    std::cout << "补图边数: " << complement.numEdges() << std::endl;
}

// 5. 展示dynamicUndirectedGraph功能
void demonstrateDynamicUndirectedGraph() {
    printSeparator("dynamicUndirectedGraph动态功能展示");
    dynamicUndirectedGraph dug;

    // 构建完全图
    for (int i = 1; i <= 4; ++i) dug.addVertex(i);
    for (int i = 1; i <= 4; ++i)
        for (int j = i + 1; j <= 4; ++j)
            dug.addEdge(i, j);
    std::cout << "4顶点完全图边数: " << dug.numEdges() << std::endl;

    // 边切换操作
    std::vector<Edge*> edgesToSwitch;
    edgesToSwitch.push_back(dug.findEdge(1, 2));
    edgesToSwitch.push_back(dug.findEdge(3, 4));
    dynamicUndirectedGraph switched = dug.edgesSwitch(edgesToSwitch);
    std::cout << "切换边(1-2)和(3-4)后, 新图边数: " << switched.numEdges() << std::endl;
    std::cout << "切换后是否存在边(1-2): " << (switched.findEdge(1, 2) ? "是" : "否") << std::endl;
}

// 6. 展示图算法功能
void demonstrateAlgorithms() {
    printSeparator("图算法功能展示");

    // 二分图检查
    printSeparator("二分图检查");
    dynamicUndirectedGraph bipartiteGraph;
    for (int i = 1; i <= 6; ++i) bipartiteGraph.addVertex(i);
    // 构建二分图 (1-2-3 与 4-5-6 之间连边)
    for (int i = 1; i <= 3; ++i)
        for (int j = 4; j <= 6; ++j)
            bipartiteGraph.addEdge(i, j);
    BinaryGraphCheck bipartChecker;
    AlgorithmResult bipartResult = bipartChecker.run(bipartiteGraph);
    std::cout << "是否为二分图: " << (std::any_cast<bool>(bipartResult.getData<bool>("isBinary")) ? "是" : "否") << std::endl;

    // 割点查找
    printSeparator("割点查找");
    dynamicUndirectedGraph cutVertexGraph;
    for (int i = 1; i <= 5; ++i) cutVertexGraph.addVertex(i);
    cutVertexGraph.addEdge(1, 2);
    cutVertexGraph.addEdge(2, 3);
    cutVertexGraph.addEdge(3, 4);
    cutVertexGraph.addEdge(4, 2);
    cutVertexGraph.addEdge(2, 5);
    DFSCV cutFinder;
    AlgorithmResult cutResult = cutFinder.run(cutVertexGraph);
    std::vector<int> cutVertices = std::any_cast<std::vector<int>>(cutResult.getData<std::vector<int>>("cutVertices"));
    std::cout << "割点集合: ";
    for (int v : cutVertices) std::cout << v << " ";
    std::cout << std::endl;

    // 双连通分量查找
    printSeparator("双连通分量查找");
    DFSBlk bccFinder;
    AlgorithmResult bccResult = bccFinder.run(cutVertexGraph);
    std::vector<std::vector<int>> bccs = std::any_cast<std::vector<std::vector<int>>>(bccResult.getData<std::vector<std::vector<int>>>("biconnectedComponents"));
    std::cout << "双连通分量数量: " << bccs.size() << std::endl;
    for (size_t i = 0; i < bccs.size(); ++i) {
        std::cout << "  分量" << i + 1 << ": ";
        for (int v : bccs[i]) std::cout << v << " ";
        std::cout << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "\033[1;32m===== 图论工具包功能展示程序 =====\033[0m" << std::endl;

    // 依次展示各模块功能
    demonstrateVertex();
    demonstrateEdge();
    demonstrateDirectedGraph();
    demonstrateDynamicDirectedGraph();
    demonstrateDynamicUndirectedGraph();
    demonstrateAlgorithms();

    std::cout << "\n\033[1;32m===== 所有功能展示完成 =====\033[0m" << std::endl;
    std::cout << "按任意键继续..." << std::endl;
    std::cin.get();
    return 0;
}