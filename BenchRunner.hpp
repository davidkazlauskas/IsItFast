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
};

struct TimeResolution {
    virtual long resolve() = 0;
};

class Benchmark {


private:
    std::vector< SingleTask > _tasks;
    std::vector< long > _times;
    int _repetition;
};

}
