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

#include "BenchRunner.hpp"

TEMPLATIOUS_TRIPLET_STD;

namespace IsItFast {
    templatious::VCollection< const long > Benchmark::getTimes() const {
        return SF::vcollectionCustom<
            templatious::AP_THROW,
            templatious::CP_THROW,
            templatious::TP_ENABLED,
            templatious::ACP_THROW,
            templatious::SP_ENABLED
        >(this->_times);
    }
}
