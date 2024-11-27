#pragma once

#include "ipeIO.h"
#include "SVD_Function.h"

class L2SVDIpelet : public ipe::Ipelet {
	virtual int ipelibVersion() const { return ipe::IPELIB_VERSION; }
	virtual bool run(int, ipe::IpeletData* data, ipe::IpeletHelper* helper);
};

IPELET_DECLARE ipe::Ipelet* newIpelet();