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

        static const int N_NUM = 100000;

        auto shared = std::make_shared< std::vector<int> >();
        shared->reserve(N_NUM);

        auto defAdd =
            [shared]() {
                auto& vec = *shared;
                vec.clear();
                for (int i = 0; i < N_NUM; ++i) {
                    vec.push_back(i);
                }
            };

        auto tempAdd =
            [shared]() {
                auto& vec = *shared;
                vec.clear();
                SA::add(vec,SF::seqL(N_NUM));
            };

        add.addTask("PADDING_DOESN_COUNT",
                "Padding (first is slowest)",defAdd);

        add.addTask("addition_default",
                "Default addition",defAdd);

        add.addTask("addition_templatious",
                "Default addition",tempAdd);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    static bool didAdd = addBenchmarks();
}

