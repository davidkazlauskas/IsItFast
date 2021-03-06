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
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include <cstring>
#include <iostream>
#include <fstream>

#include "BenchRunner.hpp"

TEMPLATIOUS_TRIPLET_STD;

static auto toJsonPrimitives = SF::matchFunctor(
    SF::matchLoose<std::ostream,std::string,const char*>(
        [](std::ostream& o,const std::string& s,const char* key) {
            auto sf = SF::streamOutFunctor(o);
            sf('"',key,'"',':','"',s,'"');
        }
    ),
    SF::matchLoose<std::ostream,double,const char*>(
        [](std::ostream& o,double d,const char* key) {
            auto sf = SF::streamOutFunctor(o);
            sf('"',key,'"',':',' ',d);
        }
    )
);

static auto toJsonTimeNode = SF::matchFunctor(
    SF::matchLoose<std::ostream,const IsItFast::ResNode,const char*>(
        [](std::ostream& o,const IsItFast::ResNode& rn,const char* key) {
            auto sf = SF::streamOutFunctor(o);
            if (strlen(key) != 0) {
                sf('"',key,"\":");
            }
            sf("{");
            toJsonPrimitives(o,rn._key,"name");
            sf(",");
            toJsonPrimitives(o,rn._fullName,"full_name");
            sf(",");
            toJsonPrimitives(o,rn._avg,"time");
            sf("}");
        }
    )
);

static auto toJson = SF::matchFunctor(
    toJsonPrimitives,
    toJsonTimeNode,
    SF::matchLoose<std::ostream,const IsItFast::Benchmark,const char*>(
        [](std::ostream& o,const IsItFast::Benchmark& b,const char* key) {
            auto sf = SF::streamOutFunctor(o);
            sf("{");
            toJsonPrimitives(o,b.keyName(),"name");
            sf(",");
            toJsonPrimitives(o,b.fullName(),"full_name");
            sf(",\"times\": [");

            auto times = b.getTimes();
            auto beg = SA::begin(times);
            auto end = SA::end(times);
            toJsonTimeNode(o,*beg,"");
            ++beg;
            while (beg != end) {
                sf(",");
                toJsonTimeNode(o,*beg,"");
                ++beg;
            }

            sf("]}");
        }
    )
);

void jsonResults(IsItFast::BenchCollection& r,const char* filename) {
    std::ofstream ostr(filename);

    std::string shortName;
    std::string longName;

    auto sf = SF::streamOutFunctor(ostr);
    auto res = r.viewResults();
    auto beg = SA::begin(res);
    auto end = SA::end(res);
    sf("{ \"benchmarks\":[");
    toJson(ostr,*beg,"benchmark");
    ++beg;
    while (beg != end) {
        sf(",");
        toJson(ostr,*beg,"benchmark");
        ++beg;
    }
    sf("]}");
}

void printOutResults(IsItFast::BenchCollection& r) {
    auto sf = SF::streamOutFunctor(std::cout);

    std::string full;
    std::string sName;
    double avgTime;
    auto p = SF::pack("\tShort Name: ",sName,"; Full Name: ",full,"; Time: ",avgTime," micro seconds\n");

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
    jsonResults(r,"results.json");
}
