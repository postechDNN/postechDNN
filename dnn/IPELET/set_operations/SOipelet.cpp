// --------------------------------------------------------------------
// Ipelet for two sets operation: Intersection
// --------------------------------------------------------------------
#include <string>
#include "SOipelet.h"
#include "SOutil.h"
#include "geo.h"

using namespace ipe;

bool DrawSetIpe(ipe::IpeletData* data,const std::vector<ipe::CurveSegment> *input);

bool DrawSetIpe2(ipe::IpeletData* data, const std::vector<ipe::CurveSegment>* input);

bool SOIpelet::run(int menu, ipe::IpeletData* data, ipe::IpeletHelper* helper) {

    Page* page = data->iPage;
    std::vector<int> idx;

    if (!findIdx(data, helper, idx)) return false;

    // Check each selected path is connected 
    Curve* set[2] = { new Curve(), new Curve() }; // aligned simple connected set
    std::vector<CurveSegment> extract[2];
    std::vector<CurveSegment> aligned[2];


    for (int i = 0; i < 2; i++) {
        const Path* p = page->object(idx[i])->asPath();
        ipe::Matrix transM = page->object(idx[i])->matrix();
        if (!extractCS(extract[i], p, transM)) {
            helper->message("Invalid input");
            return false;
        }
    }

    if (!writeLog("extract1.txt", &extract[0])) {
        helper->message("Fail to write log");
        return false;
    }
    if (!writeLog("extract2.txt", &extract[1])) {
        helper->message("Fail to write log");
        return false;
    }

    for (int i = 0; i < 2; i++) {
        if (!alignCS(aligned[i], &extract[i])) {
            helper->message("Path is not connected");
            return false;
        }
    }

    if (!writeLog("aligned1.txt", &aligned[0])) {
        helper->message("Fail to write log");
        return false;
    }
    if (!writeLog("aligned2.txt", &aligned[1])) {
        helper->message("Fail to write log");
        return false;
    }

    for (int i = 0; i < 2; i++) {
        if (!isSimple(data,&aligned[i])) {
            helper->message("Set is not simple");
            return false;
        }
    }

    //Test(data, helper, &aligned[0], &aligned[1]);

    
    // Set union
    if (menu == 0) {
        return setUnion(data, helper, &aligned[0], &aligned[1]);
    }
    // Set intersection
    else if (menu == 1) {
        return setIntersection(data, helper, &aligned[0], &aligned[1]);
    }
    // Set minus
    else if (menu == 2) {
        return setMinus(data, helper, &aligned[0], &aligned[1]);
    }
    
    return true;
}

bool setUnion(ipe::IpeletData* data, ipe::IpeletHelper* helper, std::vector<ipe::CurveSegment>* set1, std::vector<ipe::CurveSegment>* set2) {
    std::vector<std::vector<CurveSegment>> result;

    std::vector<int> idx1, idx2;
    std::vector<ipe::CurveSegment> cutset1, cutset2;

    cutCS(set1, set2, cutset1, idx1);
    cutCS(set2, set1, cutset2, idx2);

    std::vector<CurveSegment> rawRes;
    bool flag;

    if (verticalRay((*set1)[0].cp(0), set2) % 2 == 1) flag = true;
    else flag = false;
    if (flag) {
        for (int i = 0; i < idx1.size(); i += 2) {
            for (int j = idx1[i]; j < idx1[i+1]; j++)
                rawRes.push_back(cutset1[j]);
        }
    }
    else {
        for (int j = 0; j < idx1[0]; j++)
            rawRes.push_back(cutset1[j]);
        for (int i = 2; i < idx1.size(); i += 2) {
            for (int j = idx1[i - 1]; j < idx1[i]; j++)
                rawRes.push_back(cutset1[j]);
        }
        for (int j = idx1[idx1.size()-1]; j < cutset1.size(); j++)
            rawRes.push_back(cutset1[j]);
    }

    if (verticalRay((*set2)[0].cp(0), set1) % 2 == 1) flag = true;
    else flag = false;
    if (flag) {
        for (int i = 0; i < idx2.size(); i += 2) {
            for (int j = idx2[i]; j < idx2[i + 1]; j++)
                rawRes.push_back(cutset2[j]);
        }
    }
    else {
        for (int j = 0; j < idx2[0]; j++)
            rawRes.push_back(cutset2[j]);
        for (int i = 2; i < idx2.size(); i += 2) {
            for (int j = idx2[i - 1]; j < idx2[i]; j++)
                rawRes.push_back(cutset2[j]);
        }
        for (int j = idx2[idx2.size() - 1]; j < cutset2.size(); j++)
            rawRes.push_back(cutset2[j]);
    }
    
    //DrawSetIpe2(data, &cutset1);
    //DrawSetIpe2(data, &cutset2);
    //DrawSetIpe2(data,&rawRes);

    
    divideRaw(&rawRes, result);  
    
    for (auto& set : result) {
        std::vector<ipe::CurveSegment> aligned;
        alignCS(aligned, &set);
        if (!DrawSetIpe(data, &aligned)) {
            helper->message("Fail to draw result");
            return false;
        }
    }
    
    return true;
}
bool setIntersection(ipe::IpeletData* data, ipe::IpeletHelper* helper, std::vector<ipe::CurveSegment>* set1, std::vector<ipe::CurveSegment>* set2) {
    std::vector<std::vector<CurveSegment>> result;

    std::vector<int> idx1, idx2;
    std::vector<ipe::CurveSegment> cutset1, cutset2;

    cutCS(set1, set2, cutset1, idx1);
    cutCS(set2, set1, cutset2, idx2);

    std::vector<CurveSegment> rawRes;
    bool flag;

    if (verticalRay((*set1)[0].cp(0), set2) % 2 == 1) flag = true;
    else flag = false;
    if (flag) {
        for (int j = 0; j < idx1[0]; j++)
            rawRes.push_back(cutset1[j]);
        for (int i = 2; i < idx1.size(); i += 2) {
            for (int j = idx1[i - 1]; j < idx1[i]; j++)
                rawRes.push_back(cutset1[j]);
        }
        for (int j = idx1[idx1.size() - 1]; j < cutset1.size(); j++)
            rawRes.push_back(cutset1[j]);
    }
    else {
        for (int i = 0; i < idx1.size(); i += 2) {
            for (int j = idx1[i]; j < idx1[i + 1]; j++)
                rawRes.push_back(cutset1[j]);
        }
    }


    if (verticalRay((*set2)[0].cp(0), set1) % 2 == 1) flag = true;
    else flag = false;
    if (flag) {
        for (int j = 0; j < idx2[0]; j++)
            rawRes.push_back(cutset2[j]);
        for (int i = 2; i < idx2.size(); i += 2) {
            for (int j = idx2[i - 1]; j < idx2[i]; j++)
                rawRes.push_back(cutset2[j]);
        }
        for (int j = idx2[idx2.size() - 1]; j < cutset2.size(); j++)
            rawRes.push_back(cutset2[j]);
    }
    else {
        for (int i = 0; i < idx2.size(); i += 2) {
            for (int j = idx2[i]; j < idx2[i + 1]; j++)
                rawRes.push_back(cutset2[j]);
        }
    }

    //DrawSetIpe2(data, &rawRes);

    
    divideRaw(&rawRes, result);

    
    for (auto& set : result) {
        std::vector<ipe::CurveSegment> aligned;
        alignCS(aligned, &set);
        if (!DrawSetIpe(data, &aligned)) {
            helper->message("Fail to draw result");
            return false;
        }
    }
    
    return true;
}
bool setMinus(ipe::IpeletData* data, ipe::IpeletHelper* helper, std::vector<ipe::CurveSegment>* set1, std::vector<ipe::CurveSegment>* set2) {
    std::vector<std::vector<CurveSegment>> result;

    std::vector<int> idx1, idx2;
    std::vector<ipe::CurveSegment> cutset1, cutset2;

    cutCS(set1, set2, cutset1, idx1);
    cutCS(set2, set1, cutset2, idx2);

    std::vector<CurveSegment> rawRes;
    bool flag;

    if (verticalRay((*set1)[0].cp(0), set2) % 2 == 1) flag = true;
    else flag = false;
    if (flag) {
        for (int i = 0; i < idx1.size(); i += 2) {
            for (int j = idx1[i]; j < idx1[i + 1]; j++)
                rawRes.push_back(cutset1[j]);
        }
    }
    else {
        for (int j = 0; j < idx1[0]; j++)
            rawRes.push_back(cutset1[j]);
        for (int i = 2; i < idx1.size(); i += 2) {
            for (int j = idx1[i - 1]; j < idx1[i]; j++)
                rawRes.push_back(cutset1[j]);
        }
        for (int j = idx1[idx1.size() - 1]; j < cutset1.size(); j++)
            rawRes.push_back(cutset1[j]);
    }

    if (verticalRay((*set2)[0].cp(0), set1) % 2 == 1) flag = true;
    else flag = false;
    if (flag) {
        for (int j = 0; j < idx2[0]; j++)
            rawRes.push_back(cutset2[j]);
        for (int i = 2; i < idx2.size(); i += 2) {
            for (int j = idx2[i - 1]; j < idx2[i]; j++)
                rawRes.push_back(cutset2[j]);
        }
        for (int j = idx2[idx2.size() - 1]; j < cutset2.size(); j++)
            rawRes.push_back(cutset2[j]);
    }
    else {
        for (int i = 0; i < idx2.size(); i += 2) {
            for (int j = idx2[i]; j < idx2[i + 1]; j++)
                rawRes.push_back(cutset2[j]);
        }
    }

    divideRaw(&rawRes, result);

    for (auto& set : result) {
        std::vector<ipe::CurveSegment> aligned;
        alignCS(aligned, &set);
        if (!DrawSetIpe(data, &aligned)) {
            helper->message("Fail to draw result");
            return false;
        }
    }

    return true;
}

bool DrawSetIpe(IpeletData* data, const std::vector<ipe::CurveSegment> *input) {
    Curve* set = new Curve();
    for (const CurveSegment &cs : *input) {
        int type = cs.type();
        if (type == 0) { // Arc
            Arc arc = cs.arc();
            set->appendArc(arc.iM, arc.beginp(), arc.endp());
        }
        else if (type == 1) { // Segment
            set->appendSegment(cs.cp(0), cs.cp(1));
        }
        else if (type == 2) { // Spline
            std::vector<Vector> v;
            for (int j = 0; j < cs.countCP(); j++) {
                v.push_back(cs.cp(j));
            }
            set->appendSpline(v);
        }
    }

    Shape* shape = new Shape();
    shape->appendSubPath(set);
    Path* path = new Path(data->iAttributes, *shape);

    // Set style
    path->setPathMode(EStrokedAndFilled);
    path->setOpacity(Attribute(true, "10%"));
    path->setFill(Attribute(Color(1, 1, 1)));

    data->iPage->append(ESecondarySelected, data->iLayer, path);

    return true;
}


bool DrawSetIpe2(IpeletData* data, const std::vector<ipe::CurveSegment>* input) {
    
    for (const CurveSegment& cs : *input) {
        Curve* set = new Curve();
        int type = cs.type();
        if (type == 1) {
            set->appendSegment(cs.cp(0), cs.cp(1));
        }
        else if (type == 2) {
            std::vector<Vector> v;
            for (int j = 0; j < cs.countCP(); j++) {
                v.push_back(cs.cp(j));
            }
            set->appendSpline(v);
        }
        Shape* shape = new Shape();
        shape->appendSubPath(set);
        Path* path = new Path(data->iAttributes, *shape);
        data->iPage->append(ESecondarySelected, data->iLayer, path);
    }
    return true;
}

bool Test(ipe::IpeletData* data, ipe::IpeletHelper* helper, std::vector<ipe::CurveSegment>* set1, std::vector<ipe::CurveSegment>* set2) {


    if (!DrawSetIpe(data, set1)) {
        helper->message("Fail to draw result");
        return false;
    }
    if (!DrawSetIpe(data, set2)) {
        helper->message("Fail to draw result");
        return false;
    }
    return true;
}