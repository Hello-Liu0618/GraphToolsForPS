#pragma once

#include "AlgorithmResult.h"
#include "graphAlgorithm.h"

class BinaryGraphCheck : public graphAlgorithm {
    public:
        AlgorithmResult run(const Graph& graph) const override;
};
