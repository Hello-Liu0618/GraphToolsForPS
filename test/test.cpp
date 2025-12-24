/*
---------------------------------------------------------------------------------------------
!!!!!
此部分主要由AI生成，但是AI生成的样例中有许多错误和与项目不匹配的地方，因此在AI基础上进行了修正
!!!!!
---------------------------------------------------------------------------------------------
*/
#include "../include/graph/vertex.h"
#include "../include/graph/edge.h"
#include "../include/graph/directedGraph.h"
#include "../include/graph/dynamicDirectedGraph.h"
#include "../include/graph/dynamicUndirectedGraph.h"
#include "../include/graph/algorithm/BinaryGraphCheck.h"
#include "../include/graph/algorithm/DFSCV.h"
#include "../include/graph/algorithm/DFSBlk.h"
#include "../include/graph/algorithm/EdgeSetMetrics.h"
#include "../include/graph/algorithm/GetComplement.h"
#include "../include/graph/algorithm/BFS_dist.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <unordered_set>
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <windows.h>

using namespace std;

// 测试工具函数
void printTestResult(const string& testName, bool success) {
    cout << "[" << (success ? "\033[1;32mPASS\033[0m" : "\033[1;31mFAIL\033[0m") << "] " << testName << endl;
}

// 生成随机整数的辅助函数
int randomInt(int min, int max) {
    static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// 1. Vertex类增强测试
bool testVertex() {
    try {
        // 基础功能测试
        Vertex v1(1);
        assert(v1.getId() == 1);
        
        // 拷贝构造与赋值
        Vertex v2(v1);
        assert(v2.getId() == 1);
        Vertex v3 = v2;
        assert(v3.getId() == 1);
        
        // ID边界测试
        Vertex v4;
        assert(v4.setId(0) == false);   // 无效ID
        assert(v4.setId(-5) == false);  // 负ID
        assert(v4.setId(1000000) == true);  // 大ID
        
        // 复杂属性操作
        v1.setAttribute("name", string("vertex1"));
        v1.setAttribute("value", 3.1415926);
        v1.setAttribute("active", true);
        v1.setAttribute("count", (long long)1234567890);
        
        // 属性类型验证
        assert(any_cast<string>(v1.getAttribute("name")) == "vertex1");
        assert(fabs(any_cast<double>(v1.getAttribute("value")) - 3.1415926) < 1e-6);
        assert(any_cast<bool>(v1.getAttribute("active")) == true);
        assert(any_cast<long long>(v1.getAttribute("count")) == 1234567890);
        
        // 属性覆盖与删除
        v1.setAttribute("value", 2.718);
        assert(fabs(any_cast<double>(v1.getAttribute("value")) - 2.718) < 1e-3);
        assert(v1.deleteAttribute("active") == true);
        assert(v1.deleteAttribute("nonexistent") == false);
        
        // 批量属性操作
        unordered_map<string, any> attrs;
        attrs["a"] = 10;
        attrs["b"] = string("test");
        v1.setAttributesMap(attrs);
        assert(any_cast<int>(v1.getAttribute("a")) == 10);
        assert(any_cast<string>(v1.getAttribute("b")) == "test");
        assert(v1.clearAttributes() == true);
        assert(v1.getAttributesMap().empty() == true);

        return true;
    } catch (const exception& e) {
        cerr << "Vertex测试异常: " << e.what() << endl;
        return false;
    }
}

// 2. Edge类增强测试
bool testEdge() {
    try {
        pair<int, int> uv(1, 2);
        pair<int, int> vu(2, 1);
        pair<int, int> self(5, 5);
        
        // 多种构造方式测试
        Edge e1(10, uv, 3);
        assert(e1.getId() == 10);
        assert(e1.getU() == 1);
        assert(e1.getV() == 2);
        
        assert(fabs(e1.getWeight() - 3) < 1e-6);
        
        Edge e2(20, vu);
        assert(e2.getWeight() == 1);  // 默认权重
        assert(e2.isSame(e1) == false);  // 方向不同
        
        Edge e3(self, 0.5);
        assert(e3.getId() == -1);  // 默认ID
        assert(e3.getU() == e3.getV());  // 自环边
        
        // 拷贝与比较
        Edge e4(e1);
        assert(e4.isSame(e1) == true);
        e4.setWeight(4.5);
        assert(e4.isSame(e1) == false);  // 权重不同视为不同边
        
        // 属性操作
        e1.setAttribute("capacity", 100);
        e1.setAttribute("label", string("main"));
        assert(any_cast<int>(e1.getAttribute("capacity")) == 100);
        
        // 自环边特殊测试
        Edge selfEdge(30, self);
        assert(selfEdge.getU() == selfEdge.getV());

        return true;
    } catch (const exception& e) {
        cerr << "Edge测试异常: " << e.what() << endl;
        return false;
    }
}

// 3. DirectedGraph类增强测试
bool testDirectedGraph() {
    try {
        DirectedGraph g;
        
        // 顶点批量操作
        assert(g.addVertex(1) == true);
        assert(g.addVertex(1) == false);  // 重复添加
        assert(g.addVertex(2) == true);
        assert(g.addVertex(3) == true);
        assert(g.addVertex(0) == false);  // 无效ID
        assert(g.numVertices() == 3);
        
        // 边操作与验证
        assert(g.addEdge(1, 2, 3) == true);
        assert(g.addEdge(1, 3, 2) == true);
        assert(g.addEdge(2, 3, 1) == true);
        assert(g.addEdge(3, 1) == true);  // 形成环
        try{
            assert(g.addEdge(1, 4) == false);  // 目标顶点不存在
        }catch(const invalid_argument& e){
            assert(std::string(e.what()) == "vertices not exist");
        }
        assert(g.numEdges() == 4);
        
        // 查找与邻居测试
        assert(g.findEdge(1, 2)->getWeight() == 3);
        vector<int> neighbors1 = g.neighbors(1);
        sort(neighbors1.begin(), neighbors1.end());
        assert(neighbors1 == vector<int>({2, 3}));
        
        // 度测试（入度+出度）
        assert(g.degree(1) == 2);  // 出度:2
        
        assert(g.degree(1) == 2);
        
        // 复杂子图测试
        vector<int> subVerts = {1, 2, 3};
        Graph* subGraph = g.subgraph_by_vertices(subVerts);
        assert(subGraph->numVertices() == 3);
        assert(subGraph->numEdges() == 4);  // 所有边都在子图中
        
        // 空图测试
        DirectedGraph emptyG;
        assert(emptyG.numVertices() == 0);
        assert(emptyG.numEdges() == 0);
        assert(emptyG.subgraph_by_vertices({}) != nullptr);
        
        delete subGraph;
        return true;
    } catch (const exception& e) {
        cerr << "DirectedGraph测试异常: " << e.what() << endl;
        return false;
    }
}

// 4. 动态图增强测试（有向+无向）
bool testDynamicGraphs() {
    try {
        // 动态有向图复杂测试
        dynamicDirectedGraph ddg;
        // 添加10个顶点
        for (int i = 1; i <= 10; ++i) {
            assert(ddg.addVertex(i) == true);
        }
        // 添加随机边
        vector<pair<int, int>> edges;
        for (int i = 0; i < 15; ++i) {
            int u = randomInt(1, 10);
            int v = randomInt(1, 10);
            if (u != v && ddg.findEdge(u, v) == nullptr) {
                ddg.addEdge(u, v);
                edges.emplace_back(u, v);
            }
        }
        assert(ddg.numEdges() == edges.size());
        
        // 随机删除顶点测试
        int delV = randomInt(1, 10);
        int prevVertCount = ddg.numVertices();
        int prevEdgeCount = ddg.numEdges();
        assert(ddg.removeVertex(delV) == true);
        assert(ddg.numVertices() == prevVertCount - 1);
        // 验证所有关联边都被删除
        for (auto& e : edges) {
            if (e.first == delV || e.second == delV) {
                assert(ddg.findEdge(e.first, e.second) == nullptr);
            }
        }
        
        // 动态无向图测试
        dynamicUndirectedGraph dug;
        // 构建完全图
        for (int i = 1; i <= 5; ++i) dug.addVertex(i);
        for (int i = 1; i <= 5; ++i) {
            for (int j = i+1; j <= 5; ++j) {
                dug.addEdge(i, j);
            }
        }
        assert(dug.numEdges() == 10);  // 5*4/2=10
        
        // 补图测试（完全图的补图是空图）
        dynamicUndirectedGraph comp = dug.getComplement();
        assert(comp.numEdges() == 0);
        
        // 边切换测试
        vector<Edge*> edgesToSwitch;
        edgesToSwitch.push_back(dug.findEdge(1, 2));
        edgesToSwitch.push_back(dug.findEdge(3, 4));
        dynamicUndirectedGraph switched = dug.edgesSwitch(edgesToSwitch);
        assert(switched.findEdge(1, 2) == nullptr);  // 已删除
        assert(switched.findEdge(3, 4) == nullptr);  // 已删除
        assert(switched.findEdge(1, 3) != nullptr);  // 保留的边
        assert(switched.numEdges() == dug.numEdges() - 2);
        
        // 边界测试：删除不存在的顶点/边
        assert(ddg.removeVertex(99) == false);
        assert(dug.removeEdge(new Edge({1,99})) == false);  // 无效边

        return true;
    } catch (const exception& e) {
        cerr << "动态图测试异常: " << e.what() << endl;
        return false;
    }
}

// 5. 二分图检查增强测试
bool testBinaryGraphCheck() {
    try {
        BinaryGraphCheck checker;
        
        // 大型二分图测试（ bipartite graph: 20个顶点的完全二分图）
        dynamicUndirectedGraph bigBipartite;
        for (int i = 1; i <= 20; ++i) bigBipartite.addVertex(i);
        // 左部:1-10，右部:11-20，只在两部之间连边
        for (int i = 1; i <= 10; ++i) {
            for (int j = 11; j <= 20; ++j) {
                bigBipartite.addEdge(i, j);
            }
        }
        AlgorithmResult res1 = checker.run(bigBipartite);
        assert(any_cast<bool>(res1.getData<bool>("isBinary")) == true);
        
        // 非二分图测试（含多个奇数环）
        dynamicUndirectedGraph nonBipartite;
        for (int i = 1; i <= 7; ++i) nonBipartite.addVertex(i);
        // 3-环（奇数环）
        nonBipartite.addEdge(1, 2);
        nonBipartite.addEdge(2, 3);
        nonBipartite.addEdge(3, 1);
        // 5-环（奇数环）
        nonBipartite.addEdge(4, 5);
        nonBipartite.addEdge(5, 6);
        nonBipartite.addEdge(6, 7);
        nonBipartite.addEdge(7, 4);
        nonBipartite.addEdge(4, 6);
        AlgorithmResult res2 = checker.run(nonBipartite);
        assert(any_cast<bool>(res2.getData<bool>("isBinary")) == false);
        
        // 边界测试：单顶点图
        dynamicUndirectedGraph singleVertex;
        singleVertex.addVertex(1);
        AlgorithmResult res3 = checker.run(singleVertex);
        assert(any_cast<bool>(res3.getData<bool>("isBinary")) == true);
        
        // 边界测试：空图
        dynamicUndirectedGraph emptyGraph;
        AlgorithmResult res4 = checker.run(emptyGraph);
        assert(any_cast<bool>(res4.getData<bool>("isBinary")) == true);

        return true;
    } catch (const exception& e) {
        cerr << "二分图检查测试异常: " << e.what() << endl;
        return false;
    }
}

// 6. 割点算法增强测试
bool testDFSCV() {
    try {
        DFSCV cutter;
        
        // 复杂图割点测试（多个割点）
        dynamicUndirectedGraph complexGraph;
        for (int i = 1; i <= 8; ++i) complexGraph.addVertex(i);
        // 构建包含3个割点的图
        complexGraph.addEdge(1, 2);
        complexGraph.addEdge(2, 3);  // 割点:2
        complexGraph.addEdge(3, 4);  // 割点：3
        complexGraph.addEdge(3, 5);
        complexGraph.addEdge(5, 6);  // 割点:5
        complexGraph.addEdge(6, 7);
        complexGraph.addEdge(6, 8);  // 割点:6
        complexGraph.addEdge(7, 8);
        AlgorithmResult res = cutter.run(complexGraph);
        vector<int> cuts = any_cast<vector<int>>(res.getData<vector<int>>("cutVertices"));
        sort(cuts.begin(), cuts.end());
        assert(cuts == vector<int>({2, 3, 5, 6}));
        
        // 树结构测试（所有非叶节点都是割点）
        dynamicUndirectedGraph tree;
        for (int i = 1; i <= 5; ++i) tree.addVertex(i);
        tree.addEdge(1, 2);
        tree.addEdge(2, 3);
        tree.addEdge(2, 4);
        tree.addEdge(4, 5);
        AlgorithmResult res2 = cutter.run(tree);
        vector<int> treeCuts = any_cast<vector<int>>(res2.getData<vector<int>>("cutVertices"));
        sort(treeCuts.begin(), treeCuts.end());
        assert(treeCuts == vector<int>({2, 4}));
        
        // 双连通图测试（无割点）
        dynamicUndirectedGraph biconnected;
        for (int i = 1; i <= 4; ++i) biconnected.addVertex(i);
        biconnected.addEdge(1, 2);
        biconnected.addEdge(2, 3);
        biconnected.addEdge(3, 4);
        biconnected.addEdge(4, 1);
        biconnected.addEdge(1, 3);
        AlgorithmResult res3 = cutter.run(biconnected);
        vector<int> noCuts = any_cast<vector<int>>(res3.getData<vector<int>>("cutVertices"));
        assert(noCuts[0] == -1);

        return true;
    } catch (const exception& e) {
        cerr << "割点算法测试异常: " << e.what() << endl;
        return false;
    }
}

// 7. 双连通分量增强测试（修正版）
bool testDFSBlk() {
    try {
        DFSBlk blkFinder;
        
        // 复杂图的双连通分量测试（修正为3个分量）
        dynamicUndirectedGraph graph;
        for (int i = 1; i <= 6; ++i) graph.addVertex(i);
        graph.addEdge(1, 2);
        graph.addEdge(2, 3);
        graph.addEdge(3, 1);  // 第一个分量：{1,2,3}
        graph.addEdge(3, 4);  // 第二个分量：{3,4}（桥）
        graph.addEdge(4, 5);
        graph.addEdge(5, 6);
        graph.addEdge(6, 4);  // 第三个分量：{4,5,6}
        AlgorithmResult res = blkFinder.run(graph);
        vector<vector<int>> components = any_cast<vector<vector<int>>>(
            res.getData<vector<vector<int>>>("biconnectedComponents")
        );
        
        // 验证分量数量（修正为3个）
        assert(components.size() == 3);
        
        // 对每个分量排序，便于比较（分量内部顶点顺序可能不同）
        for (auto& comp : components) {
            sort(comp.begin(), comp.end());
        }
        
        // 验证3个分量的内容（顺序可能不同，需逐一匹配）
        bool hasComponent1 = false, hasComponent2 = false, hasComponent3 = false;
        for (const auto& comp : components) {
            if (comp == vector<int>{1, 2, 3}) {
                hasComponent1 = true;
            } else if (comp == vector<int>{3, 4}) {
                hasComponent2 = true;
            } else if (comp == vector<int>{4, 5, 6}) {
                hasComponent3 = true;
            }
        }
        // 确保三个分量都存在
        assert(hasComponent1 && hasComponent2 && hasComponent3);
        
        // 单环测试（验证1个分量）
        dynamicUndirectedGraph cycle;
        for (int i = 1; i <= 5; ++i) cycle.addVertex(i);
        for (int i = 1; i <= 4; ++i) cycle.addEdge(i, i+1);
        cycle.addEdge(5, 1);  // 5-环
        AlgorithmResult res2 = blkFinder.run(cycle);
        vector<vector<int>> cycleComps = any_cast<vector<vector<int>>>(
            res2.getData<vector<vector<int>>>("biconnectedComponents")
        );
        assert(cycleComps.size() == 1);  // 整个环是一个双连通分量

        return true;
    } catch (const exception& e) {
        cerr << "双连通分量测试异常: " << e.what() << endl;
        return false;
    }
}
    

// 8. 边集度量增强测试
bool testEdgeSetMetrics() {
    try {
        // 构建两个有重叠的图
        DirectedGraph g1, g2;
        for (int i = 1; i <= 5; ++i) {
            g1.addVertex(i);
            g2.addVertex(i);
        }
        // 共同边
        g1.addEdge(1, 2);
        g1.addEdge(2, 3);
        g1.addEdge(3, 4);
        // g1特有边
        g1.addEdge(4, 5);
        g1.addEdge(5, 1);
        // g2特有边
        g2.addEdge(1, 2);
        g2.addEdge(2, 3);
        g2.addEdge(5, 4);
        g2.addEdge(1, 3);
        
        EdgeSetMetrics metrics(&g2);
        AlgorithmResult res = metrics.run(g1);
        
        // 验证度量结果
        assert(any_cast<int>(res.getData<int>("intersection")) == 2);  // 共同边:1-2,2-3
        assert(any_cast<int>(res.getData<int>("union_size")) == 7);    // 总边数:2+4=6
        
        
        // 空图与非空图的度量
        DirectedGraph emptyG;
        EdgeSetMetrics emptyMetrics(&emptyG);
        AlgorithmResult res2 = emptyMetrics.run(g1);
        assert(any_cast<int>(res2.getData<int>("intersection")) == 0);
        assert(any_cast<int>(res2.getData<int>("union_size")) == g1.numEdges());

        return true;
    } catch (const exception& e) {
        cerr << "边集度量测试异常: " << e.what() << endl;
        return false;
    }
}

// 9. BFS距离算法测试（规范版）
bool testBFS_Dist() {
    try {
        // 连通图测试
        dynamicUndirectedGraph connected;
        for (int i = 1; i <= 6; ++i) connected.addVertex(i);
        connected.addEdge(1, 2);
        connected.addEdge(1, 3);
        connected.addEdge(2, 4);
        connected.addEdge(3, 5);
        connected.addEdge(4, 6);
        connected.addEdge(5, 6);

        BFS_dist bfs(1, 6); // 从顶点1到顶点6
        AlgorithmResult res = bfs.run(connected);
        int dist = res.getData<int>("distance");
        assert(dist == 3); // 1-2-4-6 或 1-3-5-6

        // 非连通图测试
        dynamicUndirectedGraph disconnected;
        for (int i = 1; i <= 5; ++i) disconnected.addVertex(i);
        disconnected.addEdge(1, 2);
        disconnected.addEdge(2, 3);
        disconnected.addEdge(4, 5); // 独立分量
        BFS_dist bfs2(1, 5);
        AlgorithmResult res2 = bfs2.run(disconnected);
        int dist2 = res2.getData<int>("distance");
        assert(dist2 == -1); // 不可达

        // 单顶点测试
        dynamicUndirectedGraph single;
        single.addVertex(1);
        BFS_dist bfs3(1, 1);
        AlgorithmResult res3 = bfs3.run(single);
        int dist3 = res3.getData<int>("distance");
        assert(dist3 == 0);

        return true;
    } catch (const std::exception& e) {
        std::cerr << "BFS距离测试异常: " << e.what() << std::endl;
        return false;
    }
}

// 10. 补图算法增强测试（新增）
bool testGetComplement() {
    try {
        // 有向图补图测试
        dynamicDirectedGraph ddg;
        for (int i = 1; i <= 3; ++i) ddg.addVertex(i);
        ddg.addEdge(1, 2);
        ddg.addEdge(2, 3);
        dynamicDirectedGraph ddgComp = ddg.getComplement();
        // 补图应包含所有不存在的边（不含自环）
        assert(ddgComp.numEdges() == 3*2 - 2);  // 总可能边数(3*2) - 原图边数(2)
        assert(ddgComp.findEdge(1, 3) != nullptr);
        assert(ddgComp.findEdge(2, 1) != nullptr);
        assert(ddgComp.findEdge(3, 1) != nullptr);
        assert(ddgComp.findEdge(3, 2) != nullptr);
        assert(ddgComp.findEdge(1, 1) == nullptr);  // 无自环
        
        // 无向图补图测试
        dynamicUndirectedGraph dug;
        for (int i = 1; i <= 4; ++i) dug.addVertex(i);
        dug.addEdge(1, 2);
        dug.addEdge(2, 3);
        dynamicUndirectedGraph dugComp = dug.getComplement();
        // 完全图应有6条边，减去原图2条，补图应有4条
        assert(dugComp.numEdges() == 4);
        assert(dugComp.findEdge(1, 3) != nullptr);
        assert(dugComp.findEdge(1, 4) != nullptr);
        assert(dugComp.findEdge(2, 4) != nullptr);
        assert(dugComp.findEdge(3, 4) != nullptr);

        return true;
    } catch (const exception& e) {
        cerr << "补图算法测试异常: " << e.what() << endl;
        return false;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    cout << "=== 开始增强版图结构全面测试 ===" << endl;
    cout << "测试环境: 包含边界条件、复杂结构和异常场景验证" << endl << endl;
    
    printTestResult("Vertex类增强测试", testVertex());
    printTestResult("Edge类增强测试", testEdge());
    printTestResult("DirectedGraph类增强测试", testDirectedGraph());
    printTestResult("动态图(有向+无向)增强测试", testDynamicGraphs());
    printTestResult("二分图检查增强测试", testBinaryGraphCheck());
    printTestResult("割点算法增强测试", testDFSCV());
    printTestResult("双连通分量增强测试", testDFSBlk());
    printTestResult("边集度量增强测试", testEdgeSetMetrics());
    printTestResult("BFS距离算法测试", testBFS_Dist());
    printTestResult("补图算法增强测试", testGetComplement());
    
    cout << endl << "=== 测试完成 ===" << endl;
    cout << "按任意键继续..." << endl;
    cin.get();
    return 0;
}
