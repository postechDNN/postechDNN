
#include <fstream>
#include "SOutil.h"
#include "helperOpen.h"
#include "ipepage.h"

using namespace ipe;

ipe::Vector applyTransformations(ipe::Vector p, ipe::Matrix transM);
ipe::Matrix applyTransformations(ipe::Matrix ellM, ipe::Matrix transM);
void extractOneCS(std::vector<ipe::CurveSegment>& res, const ipe::CurveSegment* cs, const ipe::Matrix transM);

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

bool extractCS(std::vector<ipe::CurveSegment>& res, const ipe::Path* p, const ipe::Matrix transM) {
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
                extractOneCS(res, &cs, transM);
            }
            // Closing segment
            if (curve->countSegmentsClosing() - curve->countSegments()) {
                CurveSegment cs = curve->closingSegment();
                extractOneCS(res, &cs, transM);
            }         
        }
        else if (type == 1) { // Ellipse
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

void extractOneCS(std::vector<ipe::CurveSegment>& res, const ipe::CurveSegment *cs, const ipe::Matrix transM) {
    if (cs->type() == 0) { // Arc
        ipe::Arc arc = cs->arc();
        if (arc.beginp() != arc.endp()) {
            Curve* cv = new Curve();
            ipe::Vector cp[2];
            ipe::Matrix arcM = applyTransformations(arc.iM, transM);
            cp[0] = applyTransformations(arc.beginp(), transM);
            cp[1] = applyTransformations(arc.endp(), transM);
            cv->appendArc(arcM, cp[0], cp[1]);
            res.push_back(cv->segment(0));
        }
    }
    else if (cs->type() == 1) { // Segment
        if (cs->cp(0) != cs->cp(1)) {
            Curve* cv = new Curve();
            ipe::Vector cp[2];
            cp[0] = applyTransformations(cs->cp(0), transM);
            cp[1] = applyTransformations(cs->cp(1), transM);
            cv->appendSegment(cp[0], cp[1]);
            res.push_back(cv->segment(0));
        }
            
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
                std::vector<Vector> tv;
                for (int i = 0; i < v.size(); i++) {
                    tv.push_back(applyTransformations(v[i], transM));
                }
                bc->appendSpline(tv);
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

ipe::Vector applyTransformations(ipe::Vector p, ipe::Matrix transM) {
    double dx, dy;
    dx = transM.a[0] * p.x + transM.a[2] * p.y + transM.a[4];
    dy = transM.a[1] * p.x + transM.a[3] * p.y + transM.a[5];
    return ipe::Vector(dx, dy);
}

ipe::Matrix applyTransformations(ipe::Matrix ellM, ipe::Matrix transM) {
    ipe::Matrix ret;
    for (int i = 0; i < 4; i += 2) {
        ret.a[i] = transM.a[0] * ellM.a[i] + transM.a[2] * ellM.a[i + 1];
        ret.a[i + 1] = transM.a[1] * ellM.a[i] + transM.a[3] * ellM.a[i + 1];
    }
    ret.a[4] = transM.a[0] * ellM.a[4] + transM.a[2] * ellM.a[5] + transM.a[4];
    ret.a[5] = transM.a[1] * ellM.a[4] + transM.a[3] * ellM.a[5] + transM.a[5];
    return ret;
}