/*
 * =====================================================================================
 *
 *       Filename:  StaticAdapterBenchmarks.cpp
 *
 *    Description:  Benchmark for common operations
 *
 *        Version:  1.0
 *        Created:  2015.03.26 17:50:22
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

namespace IsItFast {
    bool addBenchmarks() {
        TimeResolution* tr = TimeResolutionFactory::s_curr;
        Benchmark add(tr,50,"SA_add","Addition to vector with"
                "templatious and with default method.");

        BENCH_TASK_GEN( AdditionDefault,
                "addition_default",
                "Add to vector 1000000 elements",
            ad, {
            std::vector<int> v;
            for (int i = 0; i < 1000000; ++i) {
                v.push_back(i);
            }
        });


        // ad
        auto h = add.taskHandle();
        SA::add(h,ad);

        return true;
    }

    static bool didAdd = addBenchmarks();
}

