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
        Benchmark add(tr,5000,"distribution",
            "Distribute elements");

        auto sptr = std::make_shared< std::vector<int> >();
        SA::add(*sptr,2,4,57,6,5,3,4,5);

        auto boilAlg = [sptr]() {
            volatile int sum = 0;

            int a,b,c,d,e;
            SM::set(0,a,b,c,d,e);

            int* datOrig = sptr->data();
            TEMPLATIOUS_REPEAT( 10000 ) {
                int* dat = datOrig;

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
            }

            ++sum;
        };

        auto vectAlg = [sptr]() {
            volatile int sum = 0;

            int a,b,c,d,e;
            SM::set(0,a,b,c,d,e);

            auto& ref = *sptr;
            TEMPLATIOUS_REPEAT( 10000 ) {
                a = ref[0]; // 2
                b = ref[1]; // 6
                c = ref[3]; // 12
                d = ref[6]; // 16
                e = ref[7]; // 21

                sum = a + b + c + d + e;
            }

            ++sum;
        };


        auto tempAlg = [sptr]() {
            volatile int sum = 0;

            int a,b,c,d,e;
            SM::set(0,a,b,c,d,e);

            auto& ref = *sptr;
            TEMPLATIOUS_REPEAT( 10000 ) {
                SM::distribute(ref,
                    a,
                    b,
                    SF::dummyVar<1>(),
                    c,
                    SF::dummyVar<2>(),
                    d,
                    e);

                sum = a + b + c + d + e;
            }

            ++sum;
        };

        add.addTask("DIRECT",
            "Direct pointer manipulation",boilAlg);
        add.addTask("VECTOR",
            "Vector assigment",vectAlg);
        add.addTask("templatious",
            "Templatious assignment",tempAlg);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    bool loopingNormalBenchmark() {
        Benchmark add(tr,50,"looping_simple",
            "Simple loop 0 until 1000000"
            " and sum all elements.");

        const int ROUNDS = 1000000;
        auto loopBoiler = [=]() {
            volatile int sum = 0;

            for (int i = 0; i < ROUNDS; ++i) {
                sum += i;
            }
        };

        auto tempLoop1 = [=]() {
            volatile int sum = 0;

            // kinda unfair because SM::sum doesn't
            // have to write to volatile variable but
            // hey, LET'S BOAST!
            sum += SM::sum<int>(SF::seqL(ROUNDS));
        };

        auto tempLoop2 = [=]() {
            volatile int sum = 0;

            auto seq = SF::seqL(ROUNDS);
            TEMPLATIOUS_FOREACH(int i,seq) {
                sum += i;
            }
        };

        auto tempLoop3 = [=]() {
            volatile int sum = 0;

            auto seq = SF::seqL(ROUNDS);
            SM::forEach([&](int i) { sum += i; },seq);
        };

        add.addTask("LOOP_BOILER",
            "Simple boilerplate looping",loopBoiler);
        add.addTask("templatious",
            "Templatious sum v1 (spec func)",tempLoop1);
        add.addTask("templatious",
            "Templatious sum v2 (FOREACH macro)",tempLoop2);
        add.addTask("templatious",
            "Templatious sum v3 (forEach function)",tempLoop3);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    bool nestedLoopingBenchmark() {
        Benchmark add(tr,50,"looping_nested",
            "3 nested loops 0 until 100.");

        const int ROUNDS = 100;
        auto loopBoiler = [=]() {
            volatile int sum = 0;

            for (int i = 0; i < ROUNDS; ++i) {
                for (int j = 0; j < ROUNDS; ++j) {
                    for (int k = 0; k < ROUNDS; ++k) {
                        sum += i + j + k;
                    }
                }
            }
        };

        auto tempLoop1 = [=]() {
            volatile int sum = 0;

            auto s = SF::seqL(ROUNDS);

            TEMPLATIOUS_FOREACH(int i,s) {
                TEMPLATIOUS_FOREACH(int j,s) {
                    TEMPLATIOUS_FOREACH(int k,s) {
                        sum += i + j + k;
                    }
                }
            }
        };

        auto tempLoop2 = [=]() {
            volatile int sum = 0;

            auto seq = SF::seqL(ROUNDS);
            SM::quadro([&](int i,int j,int k) {
                sum += i + j + k;
            },seq,seq,seq);
        };

        add.addTask("LOOP_BOILER",
            "Simple boilerplate looping",loopBoiler);
        add.addTask("templatious",
            "Templatious loop v1 (FOREACH macro)",tempLoop1);
        add.addTask("templatious",
            "Templatious loop v2 (quadro function)",tempLoop2);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    // this should probably go to a different
    // file but I'm too lazy to make one
    bool staticVectorVsStdVectorBenchmark() {
        Benchmark add(tr,500,"static_vs_std",
            "Static vector vs std::vector benchmarks."
            " Add 1024 integers to a vector 100 times.");

        const int ROUNDS = 100;
        const int ADD_SEQ = 1024;

        auto flappingStdVector = [=]() {
            TEMPLATIOUS_REPEAT(ROUNDS) {
                std::vector<int> v;
                v.reserve(ADD_SEQ);

                SA::add(v,SF::seqL(ADD_SEQ));
            }
        };

        auto shared = std::make_shared< std::vector<int> >();
        shared->reserve(ADD_SEQ);

        auto preallocatedStdVector = [=]() {
            TEMPLATIOUS_REPEAT(ROUNDS) {
                shared->clear();

                SA::add(*shared,SF::seqL(ADD_SEQ));
            }
        };

        auto templatiousStaticVector = [=]() {
            TEMPLATIOUS_REPEAT(ROUNDS) {
                templatious::StaticBuffer<int,ADD_SEQ> buf;

                auto v = buf.getStaticVector();
                SA::add(v,SF::seqL(ADD_SEQ));
            }
        };

        add.addTask("FLAPPING",
            "std::vector allocated each time"
            " on a stack.",flappingStdVector);
        add.addTask("PREALLOCATED",
            "std::vector preallocated and"
            " cleared each iteration.",preallocatedStdVector);
        add.addTask("templatious",
            "Static vector initiated on stack"
            " each iteration.",templatiousStaticVector);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    struct WrapContainer {
        WrapContainer(templatious::VCollection<int>&& v) : _t(std::move(v)) {}

        WrapContainer(const WrapContainer& cnst) :
            _t(std::move(const_cast< WrapContainer& >(cnst)._t)) {}

        templatious::VCollection<int> _t;
    };

    bool stdVectorVirtualWrapBenchmark() {
        Benchmark add(tr,50,"virtual_wrap_vector",
            "Performance of std::vector raw and wrapped"
            " inside VirtualCollection");

        auto ptr = std::make_shared< std::vector<int> >();
        auto ww = SF::vcollection(*ptr);
        WrapContainer cont(std::move(ww));

        const int ELEMENTS = 1000000;

        auto clAddRaw = [=]() {
            SA::clear(*ptr);
            SA::add(*ptr,SF::seqL(ELEMENTS));
        };

        auto clAddWrapper = [=]() {
            typedef templatious::VCollection<int>& CastAway;
            CastAway ncref = const_cast< CastAway >(cont._t);
            SA::clear(ncref);
            SA::add(ncref,SF::seqL(ELEMENTS));
        };

        add.addTask("templatious_no_wrap",
            "Unwrapped std::vector addition.",clAddRaw);
        add.addTask("templatious_wrap",
            "Virtual wrap around std::vector addition.",clAddWrapper);

        BenchCollection::s_inst.addBenchmark(std::move(add));

        return true;
    }

    static bool didAdd =
        ifSelectCpy()
        && filterOutVector()
        && distributionBenchmark()
        && loopingNormalBenchmark()
        && nestedLoopingBenchmark()
        && staticVectorVsStdVectorBenchmark()
        && stdVectorVirtualWrapBenchmark();
}
