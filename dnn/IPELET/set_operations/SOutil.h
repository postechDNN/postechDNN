#pragma once

#include <string>
#include "ipelet.h"
#include "ipepath.h"
#include "ipegeo.h"
#include "ipeshape.h"

bool findIdx(ipe::IpeletData* data, ipe::IpeletHelper* helper, std::vector<int> &idx);

bool extractCS(std::vector<ipe::CurveSegment> &res, const ipe::Path *p, const ipe::Matrix transM);

bool writeLog(const std::string fileName, const std::vector<ipe::CurveSegment>* input);

bool writeLog(const std::string fileName, const std::vector<std::vector<ipe::Vector>>* input);