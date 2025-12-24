#pragma once

#include "graphAlgorithm.h"
#include "../graph.h"
#include "algorithmResult.h"

class EdgeSetMetrics : public graphAlgorithm {
    private:
        Graph* other;
    public:
        EdgeSetMetrics(Graph* other) {
            this->other = other;
        }
        AlgorithmResult run(const Graph& graph) const override;
};