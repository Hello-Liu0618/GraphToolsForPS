#GraphTool--Project for Problemg Solving 2025

##名称与简介

- **名称：**GraphTool
该项目为南京大学问题求解2025年秋季学期课程作业，旨在用面向对象编程思想设计图论工具包
- **简介： **GraphTool是一个基于c++的图论工具库，目前阶段支持对于简单图的一些操作，适用图的范围包括有向动态图、无向动态图、有向静态图、无向静态图的创建与操作，内置以下算法：对动态图的边加入与删除切换、获得两个同类图的边并集与交集大小、求补图、用Tarjan算法求割点集、BFS求两点间无权最短距离、二分图判定、求点双连通分量
- **由于时间关系，许多算法没能实现，另外目前暂时没有编写 GraphIO 以及 GraphManager 功能**

## 核心功能

- 支持静态/动态图（有向图、无向图）的顶点与边的增删改查
- 顶点和边可附加任意类型属性（基于 std::any 以及 template）
- 内置经典图算法：二分图检测、割点查找、双连通分量分解、BFS求两点间无权最短距离等
- 支持子图生成、补图计算、边集交并集分析等拓展操作
- 较为完善的错误处理以及边界检测，较为明确的报错信息
- test.cpp 进行了一些测试，main.cpp 进行了对功能的一些展示
**本项目中test.cpp与main.cpp作为测试与展示内容主要由AI生成，对其进行了调整以确保适用与正确，对于项目主要内容的搭建由本人自主搭建，部分重复性的内容以及少量代码注释使用了copilot代码补全功能**

##项目结构

graphTools_for_PS_2025fall/
├── LICENSE
├── makefile
├── readme.md
├── .vscode/
│   ├── launch.json
│   ├── settings.json
│   └── tasks.json
├── app/
│   └── main.cpp
├── bin/
├── include/
│   └── graph/
│       ├── directedGraph.h
│       ├── dynamicDirectedGraph.h
│       ├── dynamicUndirectedGraph.h
│       ├── edge.h
│       ├── graph.h
│       ├── undirectedGraph.h
│       ├── vertex.h
│       └── algorithm/
│           ├── AlgorithmResult.h
│           ├── BFS_dist.h
│           ├── BinaryGraphCheck.h
│           ├── DFSBlk.h
│           ├── DFSCV.h
│           ├── EdgeSetMetrics.h
│           ├── EdgesSwitch.h
│           ├── GetComplement.h
│           └── graphAlgorithm.h
├── obj/
├── src/
│   ├── BFS_dist.cpp
│   ├── BinaryGraphCheck.cpp
│   ├── DFSBlk.cpp
│   ├── DFSCV.cpp
│   ├── directedGraph.cpp
│   ├── dynamicDirectedGraph.cpp
│   ├── dynamicUndirectedGraph.cpp
│   ├── edge.cpp
│   ├── EdgeSetMetrics.cpp
│   ├── undirectedGraph.cpp
│   └── vertex.cpp
└── test/
    └── test.cpp


- 以Vertex和Edge类作为基础搭建Graph类
- Graph作为抽象基类，DirectedGraph和UndirectedGraph作为继承子类，又各自派生出DynamicDirectedGraph和DynamicUndirectedGraph
- GraphAlgorithm作为算法抽象基类，具体算法继承Algortihm并重写run函数
- AlgorithmResult作为储存算法结果的类
##安装与编译
- 环境要求
c++17及以上编译器(GCC 8+/MSVC2019+)
CMake 3.10+（test.cpp与main.cpp使用了windows.h头文件用于控制输出汉字为CP_UTF8编码，跨平台使用前视情况调整）
- 编译步骤
    
	
    git clone https://github.com/Hello-Liu0618/GraphToolsForPS.gitb
    cd graphTools_for_PS_2025fall
    
    make
    
    bin/main.exe
    bin/test.exe
	
	make clean//清除
##快速入门
    #include <graph.h>
    #include <BinaryGraphCheck.h>
    #include <iostream>
    
    int main() {
    	//创建无向图
    	UndirectedGraph graph;
    	graph.addVertex(1);//添加顶点1
    	graph.addVertex(2;)//添加顶点2
    	graph.addEdge(1,2);//添加边(1,2)
    	
    	//检查二分图
    	BinaryGraphCheck checker;
    	AlgorithmResult result = check.run(&graph);
    	if ( result.getData<bool>("isBinary") ) {
    		std::cout<< "The graph is binaryGraph";
    	}
    	return 0;
    }



##详细使用方法
###图类型
Graph为抽象基类

    //Graph
    bool addVertex(int v);
    bool addEdge(int u, int v);
    bool addEdge(int u, int v, int weight);//添加赋权边，否则默认为1;weight 只支持int型
    
    int setEdgeId(Edge* e, int id);//根据边的指针设置其ID
    
    int degree(int v);//获取顶点度数（对于有向图为出度）
    
    Graph* subgraph_by_vertices(const std::vector<int>& vertices);点导出子图
    Graph* subgraph_by_edges(const std::vector<Edge*>& edges);边导出子图
    /*输入边指针集中元素不需要为原指针，函数根据输入边的两个端点在原图中搜索边，而非根据指针，封装为Edge型是为了保证封装性与Edge设计更加符合*/
    
    int numVertices();//顶点数量
    int numEdges();边的数量
    bool isDirected();是否为有向图
    
    std::vector<int> neighbors(int v);获得v的邻点
    std::vector<Edges*> edges();输出边集
    std::vector<int> vertices();输出顶点集
    
    Vertex* findVertex(int v);//找到图中点的指针
    Edge* findEdge(int u, int v);//根据端点找到图中边的指针
    Edge* findEdge(int Id);//根据Id找到图中边的指针
    
    std::any getVertexAttr(int v, std::string key);//获取点的key属性
    std::any getEdgeAttr(int u, int v, std::string key);//获取边(u, v)的属性
    std::any getEdgeAttr(int id, std::string key)//获取边id的属性
    
    bool setVertexAttr(int v, std::string key, std::any value) = 0;//设置点的属性
    bool setEdgeAttr(int u, int v, std::string key, std::any value) = 0;//设置边(u, v)的属性
    bool setEdgeAttr(int id, std::string key, std::any value) = 0;//设置边id的属性
    bool removeVertexAttr(int v, std::string key) = 0; // 删除顶点v的属性
    bool removeEdgeAttr(int u, int v, std::string key) = 0; // 删除边(u,v)的属性
    bool removeEdgeAttr(int id, std::string key) = 0; // 删除边id的属性
    bool clearVertexAttrs(int v) = 0; // 清除顶点v的属性
    bool clearEdgeAttrs(int u, int v) = 0; // 清除边(u,v)的属性
    bool clearEdgeAttrs(int id) = 0; // 清除边id的属性
    
    Graph* clone() const = 0;//获得图的深拷贝

对于动态图，增加了一些动态操作

    //对于dynamicUndirectedGraph
    bool removeVertex( int v );//移除点
    bool removeEdge( Edge* e );//移除边
    dynamicUndirectedGraph edgesSwitch(const std::vector<Edge*> &edges);//对边集进行添加/删除交换
    dynamicUndirectedGraph getComplement();//求补图
    
    //对于dynamicDirectedGraph
    bool removeVertex( int v );
    bool removeEdge( Edge* e );
    dynamicDirectedGraph edgesSwitch(const std::vector<Edge*> &edges);
    dynamicDirectedGraph getComplement();

###算法类
graphAlgorithm作为抽象基类，每个具体的算法重新实现run函数
用AlgorithmResult同一储存算法输出结果，AlgorithmResult中键值表储存算法结果，对于每个AlgorithmResult，通过一定的键值取出结果
####BFS_dist
该算法需要在初始化算法类的时候输入起点与终点，用key="distance"取出键值

	Graph graph;
	//...
	BFS_dist get(int start, int end);//初始化时输入起点与终点
    AlgorithmResult result = get.run(graph);//执行run函数
    int dist = result.getData<int>("distance");//运行结果
	
###BinaryGraphCheck
默认初始化，用key="isBinary"取出键值

	Graph graph;
	//...
    BinaryGraphCheck get;
    AlgorithmResult result = get.run(graph);
    bool isBinary = result.getData<bool>("isBinary");
	
###DFSBlk
默认初始化，用key="biconnectedComponents"和
key="biconnectedComponentsCount"取出键值

	Graph graph;
	//...
    DFSBlk get;
    AlgorithmResult result = get.run(graph);
    //点双连通分量数量
    int size = result.getData<int>("biconnectedComponentsCount");
    //各个点双连通分量
    std::vector<std::vector<int>> biconnectedComponents = result.getData<std::vector<std::vector<int>>>("biconnectedComponents");

###DFSCV
默认初始化，用key="cutVertices"取出键值

    Graph graph;
	//...
	DFSCV get;
    AlgorithmResult result = get.run(graph);
    std::vector<int> cutVertices = result.getData<std::vector<int>>("cutVertices");

###EdgeSetMetrics
初始化时需要输入另一个图的指针，用key="intersection"和key="union_size"取出边交并集大小

	Graph graph;
	Graph* other = ...;
	//...
    EdgeSetMetrics get(other);
    AlgorithmResult result = get.run(graph);
    int intersection = result.getData<int>("intersetion");
    int union_size = result.getData<int>("union_size");

###EdgesSwitch
初始化时需传入边集合，用key="switchedGraph"取出新图

    Graph graph;
    std::vector<Edge*> edges;
    //...
    EdgesSwitch get(edges);
    AlgorithmResult result = get.run(graph);
    Graph = result.getData<Graph>("switchedGraph");

###GetComplement
默认初始化，用key="complementGraph"取出补图

    Graph graph;
    //...
    GetComplement get;
    AlgorithmResult result = get.run(graph);
    Graph ans = result.getData<Graph>("complementGraph");

##更多实现细节请详细阅读代码及其注释
##LICENSE
> 本项目基于 MIT 许可证开源，详情见 LICENSE 文件

##联系方式
作者：Hello-Liu0618
邮箱：2463141541@qq.com
项目地址：https://github.com/Hello-Liu0618/GraphToolsForPS

##更新日期
2025.10.17
