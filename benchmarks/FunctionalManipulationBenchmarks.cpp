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

            auto& vecRef = *sptr;
            int len = vecRef.size();
            for (int i = 0; i < len; ++i)
            {
                if (i % 7 == 0) {
                    sum += vecRef[i] * 7;
                }
            }
        };

        auto temp = [sptr]() {
            double sum = 0;
            sum += SM::sum<double>(
                SF::select(SF::skip(*sptr,7),
                [](int i) -> double { return i * 7; })
            );
        };

        add.addTask("PADDING","PADDING",boiler);
        add.addTask("BOILERPLATE","Imperative version",
            boiler);
        add.addTask("templattious_select","Templatious version",
            temp);

        BenchCollection::s_inst.addBenchmark(std::move(add));
        return true;
    }

    static bool didAdd = ifSelectCpy();
}
