/*
 * =====================================================================================
 *
 *       Filename:  BenchRunner.cpp
 *
 *    Description:  Impl
 *
 *        Version:  1.0
 *        Created:  2015.03.24 21:19:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#include <cassert>

#include "BenchRunner.hpp"

TEMPLATIOUS_TRIPLET_STD;

namespace IsItFast {
    Benchmark::Benchmark(
            TimeResolution* ptr, int rep,
            const char* key, const char* fullName) :
        _resStrat(ptr), _repetition(rep), _isRun(false),
        _keyName(key), _fullName(fullName)
    {
        _times.reserve(rep);
    }

    templatious::VCollection< const long > Benchmark::getTimes() const {
        return SF::vcollectionCustom<
            templatious::AP_THROW,
            templatious::CP_THROW,
            templatious::TP_ENABLED,
            templatious::ACP_THROW,
            templatious::SP_ENABLED
        >(this->_times);
    }

    templatious::VCollection< SingleTask* > Benchmark::taskHandle() {
        return SF::vcollectionCustom<
            templatious::AP_ENABLED,
            templatious::CP_THROW,
            templatious::TP_THROW,
            templatious::ACP_THROW,
            templatious::SP_ENABLED
        >(this->_tasks);
    }

    void Benchmark::run() {
        assert(!_isRun);
        _isRun = true;

        TEMPLATIOUS_FOREACH(auto i,_tasks) {
            long start = _resStrat->resolve();
            i->run();
            long end = _resStrat->resolve();
            long diff = end - start;
            SA::add(_times,diff);
        }
    }
}
