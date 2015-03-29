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

namespace IsItFast {
    bool addBenchmarks() {
        TimeResolution* tr = TimeResolutionFactory::s_curr;
        Benchmark add(tr,50,"SA_add","");

        return true;
    }

    static bool didAdd = addBenchmarks();
}

