#pragma once

#include "graphAlgorithm.h"
#include "AlgorithmResult.h"

class DFSBlk : public graphAlgorithm {
    public:
        AlgorithmResult run(const Graph& graph) const override;
};