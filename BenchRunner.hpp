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

#ifndef BENCHRUNNER_ER0XCDF2
#define BENCHRUNNER_ER0XCDF2

#include <vector>

#include <templatious/FullPack.hpp>

namespace IsItFast {

struct SingleTask {
    virtual void run() = 0;
    virtual const char* keyName() const = 0;
    virtual const char* fullName() const = 0;
    virtual ~SingleTask() {}
};

#define BENCH_TASK_GEN(name,fullNameL,clNameL,valName,code) \
    struct name : public SingleTask {\
        name(const char* sn,const char* fn) :\
            _shortName(sn), _fullName(fn) {}\
\
        virtual const char* keyName() const {\
            return _shortName.c_str();\
        }\
\
        virtual const char* fullName() const {\
            return _fullName.c_str();\
        }\
\
        virtual void run() {\
            code\
        }\
    private:\
        std::string _shortName;\
        std::string _fullName;\
    };\
\
    name* valName = new name(fullNameL,clNameL);

struct TimeResolution {
    virtual long resolve() = 0;
};

struct TimeResolutionFactory {
    enum Variant {
        CHRONO_HIGH_RESOLUTION
    };

    static TimeResolution* get(int i);

    static const int CURR_RESOLUTION = CHRONO_HIGH_RESOLUTION;
    static TimeResolution* s_curr;
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
public:
    templatious::VCollection< const ResNode > getTimes() const;
    templatious::VCollection< SingleTask* > taskHandle();

    Benchmark(TimeResolution* ptr,int rep,const char* key,const char* fullName);
    ~Benchmark();

    void run();

    Benchmark(Benchmark&& other);
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

    static BenchCollection s_inst;
private:
    BenchCollection() {}

    std::vector< Benchmark > _benchmarks;
};

}

#endif /* end of include guard: BENCHRUNNER_ER0XCDF2 */
