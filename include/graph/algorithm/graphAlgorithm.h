#pragma once

#include "algorithmResult.h"
#include "../graph.h"

class graphAlgorithm  {
    public:
        ~graphAlgorithm() = default;
        virtual AlgorithmResult run(const Graph &graph) const = 0;
};