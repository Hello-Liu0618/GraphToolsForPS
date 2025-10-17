#pragma once

#include "graphAlgorithm.h"
#include "AlgorithmResult.h"

class BFS_dist : public graphAlgorithm {
    private:
        int start;
        int v;
    public:
        BFS_dist(int start, int v) {
            this->start = start;
            this->v = v;
        }
        AlgorithmResult run(const Graph& graph) const override;
};