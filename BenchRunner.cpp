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
#include <chrono>

#include "BenchRunner.hpp"

TEMPLATIOUS_TRIPLET_STD;

namespace IsItFast {
    Benchmark::Benchmark(
            TimeResolution* ptr, int rep,
            const char* key, const char* fullName) :
        _resStrat(ptr), _repetition(rep), _isRun(false),
        _keyName(key), _fullName(fullName)
    {
        _times.reserve(8);
    }

    Benchmark::Benchmark(Benchmark&& other) :
        _resStrat(other._resStrat),
        _repetition(other._repetition),
        _isRun(other._isRun),
        _tasks(std::move(other._tasks)),
        _keyName(other._keyName),
        _fullName(other._fullName)
    {
    }

    Benchmark::~Benchmark() {}

    templatious::VCollection< const ResNode > Benchmark::getTimes() const {
        return SF::vcollectionCustom<
            templatious::AP_THROW,
            templatious::CP_THROW,
            templatious::TP_ENABLED,
            templatious::ACP_THROW,
            templatious::SP_ENABLED
        >(this->_times);
    }

    void Benchmark::run() {
        assert(!_isRun);
        _isRun = true;

        TEMPLATIOUS_FOREACH(auto i,_tasks) {
            long sum = 0;
            TEMPLATIOUS_REPEAT( _repetition ) {
                long start = _resStrat->resolve();
                i._f();
                long end = _resStrat->resolve();
                long diff = end - start;
                sum += diff;
            }

            ResNode r(sum,_repetition,
                i._keyName.c_str(),i._fullName.c_str());

            SA::add(_times,std::move(r));
        }
    }

    void Benchmark::addTask(const char* key,const char* full,FnType t) {
        SA::add(_tasks,SingleTask(key,full,t));
    }

    std::string Benchmark::keyName() const {
        return _keyName;
    }

    std::string Benchmark::fullName() const {
        return _fullName;
    }

    void BenchCollection::runAll() {
        TEMPLATIOUS_FOREACH(auto& i,_benchmarks) {
            i.run();
        }
    }

    void BenchCollection::addBenchmark(Benchmark&& m) {
        SA::add(_benchmarks,std::move(m));
    }

    templatious::VCollection< const Benchmark > BenchCollection::viewResults() const {
        return SF::vcollectionCustom<
            templatious::AP_THROW,
            templatious::CP_THROW,
            templatious::TP_ENABLED,
            templatious::ACP_THROW,
            templatious::SP_ENABLED
        >(_benchmarks);
    }

    BenchCollection BenchCollection::s_inst = BenchCollection();

    struct ChronoHighResolution : TimeResolution {
        typedef std::chrono::time_point<
            std::chrono::high_resolution_clock
        > TimePoint;
        static const TimePoint s_referencePoint;

        virtual long resolve() override {
            auto now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::microseconds>(
                now - s_referencePoint
            ).count();
        }
    };

    const ChronoHighResolution::TimePoint ChronoHighResolution::s_referencePoint =
        std::chrono::high_resolution_clock::now();

    static ChronoHighResolution s_chl = ChronoHighResolution();

    TimeResolution* TimeResolutionFactory::s_curr =
        TimeResolutionFactory::get(TimeResolutionFactory::CURR_RESOLUTION);

    TimeResolution* TimeResolutionFactory::get(int i) {
        if (i == CHRONO_HIGH_RESOLUTION) {
            return &s_chl;
        }
        return nullptr;
    }
}
