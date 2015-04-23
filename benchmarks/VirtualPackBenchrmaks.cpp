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

#define MFUNCT_DUMMY_2 \
    SF::virtualMatch<int,char>(\
        [](int,char) { assert(false); }\
    ),\
    SF::virtualMatch<char,int>(\
        [](char,int) { assert(false); }\
    )

#define MFUNCT_DUMMY_4 MFUNCT_DUMMY_2,MFUNCT_DUMMY_2
#define MFUNCT_DUMMY_8 MFUNCT_DUMMY_4,MFUNCT_DUMMY_4
#define MFUNCT_DUMMY_16 MFUNCT_DUMMY_8,MFUNCT_DUMMY_8
#define MFUNCT_DUMMY_32 MFUNCT_DUMMY_16,MFUNCT_DUMMY_16
#define MFUNCT_DUMMY_64 MFUNCT_DUMMY_32,MFUNCT_DUMMY_32
#define MFUNCT_DUMMY_128 MFUNCT_DUMMY_64,MFUNCT_DUMMY_64

namespace IsItFast {

    bool warBetweenVirtualPacks() {
        Benchmark add(tr,50,"virtual-pack","Different virtual pack"
            " processing speed. Process 100000 messages,"
            " in match functor where first 128 don't match,"
            " the last one does.");

        auto theTarget = SF::vpackPtr<int,int>(1,2);
        auto scapeGoat = std::make_shared<long>(0);

        auto inlineMatcher =
            SF::virtualMatchFunctorPtr(
                MFUNCT_DUMMY_128,
                SF::virtualMatch<int,int>(
                    [=](int a,int b) {
                        ++(*scapeGoat);
                    }
                )
            );

        auto dynamicMatcher = std::make_shared<
            templatious::DynamicVMatchFunctor
        >();

        TEMPLATIOUS_REPEAT( 128 ) {
            dynamicMatcher->attach(
                SF::virtualMatchFunctorPtr(
                    MFUNCT_DUMMY_2
                )
            );
        }
        dynamicMatcher->attach(
            SF::virtualMatch<int,int>(
                [=](int a,int b) {
                    ++(*scapeGoat);
                }
            )
        );

        auto dynFunct = [=]() {
            dynamicMatcher->tryMatch(*theTarget);
        }

        auto inlineFunctPtr = [=]() {
            inlineMatcher->tryMatch(*theTarget);
        };

        auto inlineFunct = [=]() {
            auto innerMatcher =
                SF::virtualMatchFunctor(
                    MFUNCT_DUMMY_128,
                    SF::virtualMatch<int,int>(
                        [=](int a,int b) {
                            ++(*scapeGoat);
                        }
                    )
                );

            innerMatcher.tryMatch(*theTarget);
        }

        add.addTask("templatious_dynamic_vpack","Dynamic templatious virtual match functor.",
            dynFunct);
        add.addTask("templatious_inline_ptr","Heap allocated templatious inline virtual match functor.",
            inlineFunctPtr);
        add.addTask("templatious_inline_stack","Stack allocated templatious inline virtual match functor.",
            inlineFunct);

        BenchCollection::s_inst.addBenchmark(std::move(add));
        return true;
    }

    static bool didAdd =
        ifSelectCpy();
}
