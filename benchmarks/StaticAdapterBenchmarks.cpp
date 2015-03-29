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
                "Add to vector 100000 elements",
            ad_d, {
            std::vector<int> v;
            for (int i = 0; i < 100000; ++i) {
                v.push_back(i);
            }
        });

        BENCH_TASK_GEN( AdditionTemplatious,
                "addition_templatious",
                "Add to vector 100000 elements",
            ad_t, {
            std::vector<int> v;
            SA::add(v,SF::seqL(100000));
        });


        // ad
        auto h = add.taskHandle();
        SA::add(h,ad_d,ad_t);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    static bool didAdd = addBenchmarks();
}

