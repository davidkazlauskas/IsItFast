/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Get ready to roll
 *
 *        Version:  1.0
 *        Created:  08/30/2014 09:19:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#include <iostream>

#include "BenchRunner.hpp"

TEMPLATIOUS_TRIPLET_STD;

int main(int argc,char* argv[]) {
    auto& r = IsItFast::BenchCollection::s_inst;
    r.runAll();

    std::string full;
    double avgTime;

    auto p = SF::pack("Name: ",full,"; Time: ",avgTime,"\n");
    auto sf = SF::streamOutFunctor(std::cout);

    TEMPLATIOUS_FOREACH(auto& i,r.viewResults()) {
        TEMPLATIOUS_FOREACH(auto& j,i.getTimes()) {
            full = j._fullName;
            avgTime = j._avg;

            SM::callEach(sf,p);
        }
    }
}
