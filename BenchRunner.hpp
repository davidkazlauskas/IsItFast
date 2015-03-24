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

class Benchmark {
    templatious::VCollection< const long > getTimes() const;
    templatious::VCollection< SingleTask* > taskHandle();

    Benchmark(TimeResolution* ptr,int rep,const char* key,const char* fullName);

    void run();
private:
    TimeResolution* _resStrat;
    int _repetition;
    bool _isRun;
    std::vector< SingleTask* > _tasks;
    std::vector< long > _times;
    std::string _keyName;
    std::string _fullName;
};

}
