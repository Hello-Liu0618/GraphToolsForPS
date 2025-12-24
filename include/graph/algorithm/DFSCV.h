#pragma once

#include "graphAlgorithm.h"
#include "algorithmResult.h"

class DFSCV : public graphAlgorithm {
    public:
        AlgorithmResult run(const Graph& graph ) const override;
};