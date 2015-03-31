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
            for (int i = 0; i < len; i += 7)
            {
                sum += vecRef[i] * 7;
            }
        };

        auto temp = [sptr]() {
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

    bool filterOutVector() {
        Benchmark add(tr,5000,"filter-remove","Erase elements"
            " from vector.");

        auto sptr = std::make_shared< std::vector<int> >();
        sptr->reserve(10000);

        auto condLambda =
            [](int i) { return i % 7 != 0; };

        auto stlAlg = [condLambda,sptr]() {
            auto& ref = *sptr;
            ref.clear();
            SA::add(ref,SF::seqL(10000));

            auto res = std::remove_if(ref.begin(),ref.end(),condLambda);

            ref.erase(res,ref.end());
            volatile int sum = SM::sum<int>(ref);
            ++sum;
        };

        auto tempAlg = [condLambda,sptr]() {
            auto& ref = *sptr;

            SA::clear(ref);
            SA::add(ref,SF::seqL(10000));

            SA::clear(SF::filter(ref,condLambda));

            volatile int sum = SM::sum<int>(ref);
            ++sum;
        };

        add.addTask("STL_ALGORITHM",
            "Use STL algorithm to erase",stlAlg);
        add.addTask("templatious",
            "Use templatious filter to erase",tempAlg);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    bool distributionBenchmark() {
        Benchmark add(tr,5000000,"distribution",
            "Distribute elements");

        auto sptr = std::make_shared< std::vector<int> >();
        SA::add(*sptr,2,4,57,6,5,3,4,5);

        auto boilAlg = [sptr]() {
            int* dat = sptr->data();

            volatile int sum = 0;

            int a,b,c,d,e;

            a = *dat; // 2
            ++dat;
            b = *dat; // 6
            dat += 2;
            c = *dat; // 12
            dat += 3;
            d = *dat; // 16
            ++dat;
            e = *dat;

            sum = a + b + c + d + e;

            ++sum;
        };

        auto tempAlg = [sptr]() {
            volatile int sum = 0;

            int a,b,c,d,e;

            SM::distribute(*sptr,
                a,
                b,
                SF::dummyVar<1>(),
                c,
                SF::dummyVar<2>(),
                d,
                e);

            sum = a + b + c + d + e;

            ++sum;
        };

        add.addTask("BOILERPLATE",
            "Boilerplate assignment",boilAlg);
        add.addTask("templatious",
            "Templatious assignment",tempAlg);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    static bool didAdd =
        ifSelectCpy()
        && filterOutVector()
        && distributionBenchmark();
}
