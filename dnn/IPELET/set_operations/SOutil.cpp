
#include <fstream>
#include "SOutil.h"
#include "helperOpen.h"
#include "ipepage.h"

using namespace ipe;

void extractOneCS(std::vector<ipe::CurveSegment>& res, const ipe::CurveSegment* cs);

bool findIdx(ipe::IpeletData* data, ipe::IpeletHelper* helper, std::vector<int>& idx) {
    Page* page = data->iPage;

    // Check the number of selected objects
    for (int i = 0; i < page->count(); i++) {
        if (page->select(i) != 0) {
            idx.push_back(i);
            if (idx.size() > 2) {
                helper->message("You must select two simple connected sets");
                return false;
            }
        }
    }

    if (idx.size() != 2) {
        helper->message("You must select two simple connected sets");
        return false;
    }

    // Check the type of selected objects
    if (page->object(idx[0])->type() != 1 || page->object(idx[1])->type() != 1) {
        helper->message("You must select two simple connected sets");
        return false;
    }
}

bool extractCS(std::vector<ipe::CurveSegment>& res, const ipe::Path* p) {
    const Shape s = p->shape();
    if (s.countSubPaths() > 1) {
        for (int j = 0; j < s.countSubPaths(); j++) {
            if (s.subPath(j)->type() != 0) return false;
        }
    }
    for (int j = 0; j < s.countSubPaths(); j++) {
        int type = s.subPath(j)->type();
        if (type == 0) { // Path
            const Curve* curve = s.subPath(j)->asCurve();
            // Curvesegment
            for (int k = 0; k < curve->countSegments(); k++) {
                CurveSegment cs = curve->segment(k);
                extractOneCS(res, &cs);
            }
            // Closing segment
            if (curve->countSegmentsClosing() - curve->countSegments()) {
                CurveSegment cs = curve->closingSegment();
                extractOneCS(res, &cs);
            }         
        }
        else if (type == 1) { // Eclipse
            //------------------ Not implemented now ------------------//
            return false;
        }
        else if (type == 2) { // Closed spline
            //------------------ Not implemented now ------------------//
            return false;
        } 
    }
    return true;
}

void extractOneCS(std::vector<ipe::CurveSegment>& res, const ipe::CurveSegment *cs) {
    if (cs->type() == 0) { // Arc
        ipe::Arc arc = cs->arc();
        if(arc.beginp() != arc.endp())
            res.push_back(*cs);
    }
    else if (cs->type() == 1) { // Segment
        if (cs->cp(0) != cs->cp(1))
            res.push_back(*cs);
    }
    else if (cs->type() == 2) { // Spline
        std::vector<Bezier> beziers;
        cs->beziers(beziers);
        for (int b = 0; b < beziers.size(); b++) {
            Curve* bc = new Curve();
            std::vector<Vector> v = { beziers[b].iV[0],beziers[b].iV[1],beziers[b].iV[2],beziers[b].iV[3] };
            bool isPoint = true;
            for (int i = 1; i < v.size(); i++) {
                if (v[0] != v[i]) {
                    isPoint = false;
                    break;
                }
            }
            if (!isPoint) {
                bc->appendSpline(v);
                res.push_back(bc->segment(0));
            }
            //delete bc;
        }
    }
}

bool writeLog(std::string fileName,const std::vector<ipe::CurveSegment>* input) {
    std::ofstream log(fileName);
    if (!log.is_open()) return false;
    for (int i = 0; i < input->size(); i++) {
        int type = (*input)[i].type();
        log << i << "-th CurveSegment type: " << type << "\n";
        if (type == 0) { // Arc
            ipe::Arc arc = (*input)[i].arc();
            log << "start point: " << arc.beginp().x << ", " << arc.beginp().y << "\n";
            log << "end point: " << arc.endp().x << ", " << arc.endp().y << "\n";
        }
        else if (type == 1) { // Segment
            log << "start point: " << (*input)[i].cp(0).x << ", " << (*input)[i].cp(0).y << "\n";
            log << "end point: " << (*input)[i].cp(1).x << ", " << (*input)[i].cp(1).y << "\n";
        }
        else if (type == 2) { // Spline
            log << "start point: " << (*input)[i].cp(0).x << ", " << (*input)[i].cp(0).y << "\n";
            log << "end point: " << (*input)[i].cp((*input)[i].countCP()-1).x << ", " << (*input)[i].cp((*input)[i].countCP() - 1).y << "\n";
        }
    }
}

bool writeLog(const std::string fileName, const std::vector<std::vector<ipe::Vector>>* input) {
    std::ofstream log(fileName);
    if (!log.is_open()) return false;
    for (int i = 0; i < input->size(); i++) {
        log << i << "-th start point: " << (*input)[i][0].x << " , " << (*input)[i][0].y << "\n";
        log << i << "-th end point: " << (*input)[i][1].x << " , " << (*input)[i][1].y << "\n";
    }
}