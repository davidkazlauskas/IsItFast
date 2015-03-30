/*
 * =====================================================================================
 *
 *       Filename:  FunctionalManipulationBenchmarks.cpp
 *
 *    Description:  Func manip benchmarks
 *
 *        Version:  1.0
 *        Created:  2015.03.30 19:45:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#include "../BenchRunner.hpp"

TEMPLATIOUS_TRIPLET_STD;

namespace {
    IsItFast::TimeResolution* tr = IsItFast::TimeResolutionFactory::s_curr;
}

namespace IsItFast {

    bool ifSelectCpy() {
        Benchmark add(tr,50,"filter-select","Traverse select some"
            " integers.");

        auto sptr = std::make_shared< std::vector<int> >();
        SA::add(*sptr,SF::seqL(10000));

        auto boiler = [sptr]() {
            volatile double sum = 0;

            int len = sptr->size();
            for (int i = 0; i < len; ++i)
            {
                if (i % 7 == 0) {
                    sum += sptr->at(i) * 7;
                }
            }
        };

        add.addTask("BOILERPLATE","Imperative version",
            boiler);

        BenchCollection::s_inst.addBenchmark(std::move(add));
        return true;
    }

    static bool didAdd = ifSelectCpy();
}
