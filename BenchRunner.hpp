/*
 * =====================================================================================
 *
 *       Filename:  BenchRunner.hpp
 *
 *    Description:  Benchmark main interface
 *
 *        Version:  1.0
 *        Created:  2015.03.24 21:09:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#include <vector>

#include <templatious/FullPack.hpp>

namespace IsItFast {

struct SingleTask {
    virtual void run() = 0;
    virtual const char* keyName() const = 0;
    virtual const char* fullName() const = 0;
};

struct TimeResolution {
    virtual long resolve() = 0;
};

struct ResNode {
    ResNode(long f,long c,const char* k,const char* fname) :
        _full(f), _count(c),
        _avg(static_cast<double>(_full)/_count),
        _key(k), _fullName(fname)
    {}

    long _full;
    long _count;
    double _avg;
    std::string _key;
    std::string _fullName;
};

class Benchmark {
    templatious::VCollection< const ResNode > getTimes() const;
    templatious::VCollection< SingleTask* > taskHandle();

    Benchmark(TimeResolution* ptr,int rep,const char* key,const char* fullName);

    void run();
private:
    TimeResolution* _resStrat;
    int _repetition;
    bool _isRun;
    std::vector< SingleTask* > _tasks;
    std::vector< ResNode > _times;
    std::string _keyName;
    std::string _fullName;
};

class BenchCollection {
public:
    void runAll();
    void addBenchmark(Benchmark&& m);

    templatious::VCollection< const Benchmark > viewResults() const;
private:
    std::vector< Benchmark > _benchmarks;
};

}
