#pragma once

#include <vector>
#include "ipelet.h"
#include "ipepath.h"
#include "ipepage.h"
#include "ipeshape.h"

class SOIpelet : public ipe::Ipelet {
public:
    virtual int ipelibVersion() const { return ipe::IPELIB_VERSION; }
    virtual bool run(int menu, ipe::IpeletData* data, ipe::IpeletHelper* helper);
};

IPELET_DECLARE ipe::Ipelet* newIpelet()
{
    return new SOIpelet;
}

bool setUnion(ipe::IpeletData* data, ipe::IpeletHelper* helper, std::vector<ipe::CurveSegment>* set1, std::vector<ipe::CurveSegment>* set2);
bool setIntersection(ipe::IpeletData* data, ipe::IpeletHelper* helper, std::vector<ipe::CurveSegment>* set1, std::vector<ipe::CurveSegment>* set2);
bool setMinus(ipe::IpeletData* data, ipe::IpeletHelper* helper, std::vector<ipe::CurveSegment>* set1, std::vector<ipe::CurveSegment>* set2);