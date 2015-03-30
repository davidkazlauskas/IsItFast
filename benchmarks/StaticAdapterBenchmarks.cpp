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

namespace {
    IsItFast::TimeResolution* tr = IsItFast::TimeResolutionFactory::s_curr;
}

namespace IsItFast {

    bool staticAdapterAdd() {
        Benchmark add(tr,50,"SA_add","Addition to vector with"
                " templatious and with default method.");

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

        add.addTask("PADDING_DOESNT_COUNT",
                "Padding (first is slowest)",defAdd);

        add.addTask("BOILERPLATE",
                "Default addition",defAdd);

        add.addTask("templatious",
                "Default addition",tempAdd);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    bool foreachBenchmark() {
        Benchmark add(tr,50,"foreach","Traverse and sum 100000"
            " integers.");

        static const int N_NUM = 100000;
        auto boiler =
            []() {
                // volatile - don't optimize out
                volatile long sum = 0;
                for (int i = 0; i < N_NUM; ++i) {
                    sum += i;
                }
            };

        auto tempA =
            []() {
                // volatile - don't optimize out
                volatile long sum = 0;
                TEMPLATIOUS_FOREACH(auto i,SF::seqL(N_NUM)) {
                    sum += i;
                }
            };

        auto tempB =
            []() {
                // volatile - don't optimize out
                volatile long sum = 0;
                SM::forEach([&](int i) { sum += i; },
                    SF::seqL(N_NUM));
            };

        auto tempC =
            []() {
                volatile long sum =
                    SM::sum<int>(SF::seqL(N_NUM));
                // no warning about unused var
                return sum;
            };

        add.addTask("BOILERPLATE","Default summing",
            boiler);
        add.addTask("templatious_a","TEMPLATIOUS_FOREACH macro",
            tempA);
        add.addTask("templatious_b","SM::forEach",
            tempB);
        add.addTask("templatious_c","SM::sum",
            tempC);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    static bool didAdd =
        staticAdapterAdd()
        && foreachBenchmark();
}

