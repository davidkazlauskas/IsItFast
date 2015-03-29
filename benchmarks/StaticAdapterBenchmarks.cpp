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

        auto defAdd =
            []() {
                std::vector<int> v;
                for (int i = 0; i < 100000; ++i) {
                    v.push_back(i);
                }
            };

        auto tempAdd =
            []() {
                std::vector<int> v;
                SA::add(v,SF::seqL(100000));
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

