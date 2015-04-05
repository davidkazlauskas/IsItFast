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

void printOutResults(IsItFast::BenchCollection& r) {
    auto sf = SF::streamOutFunctor(std::cout);

    std::string full;
    std::string sName;
    double avgTime;
    auto p = SF::pack("\tShort Name: ",sName,"; Full Name: ",full,"; Time: ",avgTime,"ms\n");

    std::string blockName;
    std::string fullBlockName;
    auto pb = SF::pack("Bench block: ",blockName," (",fullBlockName,")\n");
    auto tail = SF::pack(SF::packRepeat<27>('-'),"\n");
    TEMPLATIOUS_FOREACH(auto& i,r.viewResults()) {
        blockName = i.keyName();
        fullBlockName = i.fullName();
        SM::callEach(sf,pb);
        TEMPLATIOUS_FOREACH(auto& j,i.getTimes()) {
            sName = j._key;
            full = j._fullName;
            avgTime = j._avg;

            SM::callEach(sf,p);
        }
        SM::callEach(sf,tail);
    }
}

int main(int argc,char* argv[]) {
    auto& r = IsItFast::BenchCollection::s_inst;
    r.runAll();

    printOutResults(r);
}
