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
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include "../BenchRunner.hpp"

TEMPLATIOUS_TRIPLET_STD;

namespace {
    IsItFast::TimeResolution* tr = IsItFast::TimeResolutionFactory::s_curr;
}

namespace IsItFast {

    bool warBetweenVirtualPacks() {
        Benchmark add(tr,50,"virtual-pack","Different virtual pack"
            " processing speed. Process 100000 messages,"
            " in match functor where first 128 don't match,"
            " the last one does.");

        auto boiler = []() {
            volatile double sum = 0;

            auto& vecRef = *sptr;
            int len = vecRef.size();
            for (int i = 0; i < len; i += 7)
            {
                sum += vecRef[i] * 7;
            }
        };

        auto temp = []() {
            volatile double sum = 0;
            auto sel =
                SF::select(
                    SF::skip(*sptr,7),
                    [](int i) -> double { return i * 7; }
                );

            TEMPLATIOUS_FOREACH(auto i,sel) {
                sum += i;
            }
        };

        add.addTask("PADDING","PADDING",boiler);
        add.addTask("BOILERPLATE","Imperative version",
            boiler);
        add.addTask("templatious_select","Templatious version",
            temp);

        BenchCollection::s_inst.addBenchmark(std::move(add));
        return true;
    }

    static bool didAdd =
        ifSelectCpy();
}
