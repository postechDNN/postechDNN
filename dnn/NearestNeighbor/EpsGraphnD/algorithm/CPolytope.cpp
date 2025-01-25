#include "CPolytope.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include "linalg.h"
#include "solvers.h"

HalfPlane::HalfPlane(int dim, bool is_boundary, double coeffs[]) {
	this->dim = dim;
    this->coeffs.resize(dim + 1);
	this->is_boundary = is_boundary;
    std::copy(coeffs, coeffs + dim + 1, this->coeffs.begin()); 
}


void HalfPlane::translate(Point vec) {
    assert(vec.getsize() == this->dim);

    double cum = 0.;
    for (int i = 0; i < this->dim; i++)
        cum += this->coeffs[i] * -vec.getx(i);
    this->coeffs[this->dim] += cum;
}

HalfPlane::HalfPlane(int dim, std::vector<Point> pts, Point anc) {
    this->dim = dim;
    this->coeffs.resize(dim + 1);
    //this->coeffs = new double[dim + 1]; // Allocate space for coefficients

    // Ensure enough points to define the HalfPlane
    assert(pts.size() >= dim); // At least dim points are required

    

    // Create augmented matrix [A | 1]
    alglib::real_2d_array AugmentedA;
    int n = pts.size(); // Number of points
    AugmentedA.setlength(n, dim + 1);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < dim; ++j) {
            AugmentedA[i][j] = pts[i].getx(j); // Fill A with point coordinates
        }
        AugmentedA[i][dim] = 1.0; // Add 1 for the constant term column
    }

    // Perform SVD on augmented matrix
    alglib::real_2d_array u, vt;
    alglib::real_1d_array s;
    alglib::ae_int_t ae_dim = dim + 1;
    try {
        alglib::rmatrixsvd(AugmentedA, n, ae_dim, 2, 2, 2, s, u, vt);

        // Null space corresponds to the column in V^T with the smallest singular value
        int min_index = 0;
        double min_value = s[0];
        for (int i = 1; i < dim + 1; ++i) {
            if (s[i] < min_value) {
                min_value = s[i];
                min_index = i;
            }
        }

        // Extract null space vector
        for (int i = 0; i < dim + 1; ++i) {
            this->coeffs[i] = vt[min_index][i];
        }

        // Normalize to make b = 1 (optional)
        if (std::abs( this->coeffs[dim]) > ERR_BOUND) {
            for (int i = 0; i < dim + 1; ++i) {
                this->coeffs[i] /= this->coeffs[dim];
            }
        }

        //else {
        //    throw std::runtime_error("b is zero; HalfPlane cannot be uniquely determined.");
        //}

    }
    catch (const alglib::ap_error& e) {
        std::cerr << "ALGLIB error during SVD: " << e.msg << std::endl;
        exit(EXIT_FAILURE);
    }
    // Adjust coefficients to ensure anc is within the HalfPlane
    double value = 0.0;
    for (int i = 0; i < dim; ++i) {
        value += this->coeffs[i] * anc.getx(i);
    }
    value += this->coeffs[dim]; // Add the constant term

    if (value > ERR_BOUND) {
        // Flip the coefficients to make anc inside the HalfPlane
        this->is_boundary = false;
        for (int i = 0; i < dim + 1; ++i) {
            this->coeffs[i] *= -1;
        }
    }
    else if (std::abs(value) <= ERR_BOUND) {
        // Anc is on the boundary
        this->is_boundary = true;
    }
    else {
        // Anc is already inside the HalfPlane
        this->is_boundary = false;
    }
}

double HalfPlane::getCoeff(int index)  {
    if (index < 0 || index > dim) {
        throw std::out_of_range("Index out of range in getCoeff.");
    }
    return this->coeffs[index];
}

int HalfPlane::is_in(Point& p)  {
    assert(p.getsize() == this->dim);

    double ret = 0.0;
    for (int i = 0; i < this->dim; i++)
        ret += this->coeffs[i] * p.getx(i);
    ret += this->coeffs[this->dim];

    if (ret < -ERR_BOUND)
        return 1; // Interior
    else if (ret > ERR_BOUND)
        return -1; // Exterior
    else
        return 0; // Boundary
}
CPolytope::CPolytope(int dim, std::vector<Point> vertices, std::vector< std::vector<int>> facets) {
    this->dim = dim;

    // Copy vertices
    this->vertices = vertices;
    this->v_n = vertices.size();
    // Copy facets
    this->facets = facets;
    this->f_n = facets.size();

    Point anc = vertices[0];
    for (int i = 1; i < v_n; i++)
        anc += vertices[i];
    anc /= v_n;

    // Generate half-planes from facets
    for (const auto& facet : facets) {
        // Collect points for the current facet
        std::vector<Point> facet_points;
        for (int index : facet) {
            if (index >= 0 && index < static_cast<int>(vertices.size())) {
                facet_points.push_back(vertices[index]);
            }
            else {
                throw std::invalid_argument("Invalid vertex index in facet.");
            }
        }

        // Create a half-plane for the facet
        if (facet_points.size() == dim) { // Exactly dim points are required
            HalfPlane hp(dim, facet_points, anc);
            half_planes.push_back(hp);
        }
        else if (facet_points.size() > dim) { // More than dim points, select first dim
            std::vector<Point> plane_points(facet_points.begin(), facet_points.begin() + dim);
            HalfPlane hp(dim, plane_points, anc);
            half_planes.push_back(hp);
        }
        else {
            throw std::invalid_argument("Facet does not define a proper half-plane.");
        }
    }

}

bool CPolytope::is_intersect(Point p, Point q) {
    assert(p.getsize() == this->dim && q.getsize() == this->dim);

    // Initialize t_min and t_max to represent the entire real line
    double t_min = -std::numeric_limits<double>::infinity();
    double t_max = std::numeric_limits<double>::infinity();

    // Compute the direction vector (q - p)
    std::vector<double> direction(this->dim);
    for (int i = 0; i < this->dim; ++i) {
        direction[i] = q.getx(i) - p.getx(i);
    }

    // Iterate over each half-plane to update t_min and t_max
    for (auto& hp : this->half_planes) {
        // Compute numerator and denominator for the inequality
        double numerator = 0.0;
        double denominator = 0.0;
        for (int i = 0; i < this->dim; ++i) {
            numerator += hp.getCoeff(i) * p.getx(i);
            denominator += hp.getCoeff(i) * direction[i];
        }
        numerator += hp.getCoeff(this->dim); // Add the constant term

        // The inequality is: denominator * t + numerator <= 0
        if (std::abs(denominator) < ERR_BOUND) {
            // If denominator is approximately zero
            if (numerator > ERR_BOUND) {
                // No solution exists for this half-plane
                return false;
            }
            // Else, the line is parallel and inside the half-plane; no update needed
            continue;
        }

        double t_critical = -numerator / denominator;

        if (denominator > ERR_BOUND) {
            // t <= t_critical
            t_max = std::min(t_max, t_critical);
        }
        else if (denominator < -ERR_BOUND) {
            // t >= t_critical
            t_min = std::max(t_min, t_critical);
        }

        // Early exit if there's no possible intersection
        if (t_min - t_max > ERR_BOUND) {
            return false;
        }
    }

    // After processing all half-planes, check if [t_min, t_max] intersects with [0, 1]
    double final_t_min = std::max(t_min, 0.0);
    double final_t_max = std::min(t_max, 1.0);

    if (final_t_min - final_t_max > ERR_BOUND) {
        // No overlap between [t_min, t_max] and [0, 1]
        return false;
    }

    // There is an intersection within the segment [p, q]
    return true;
}

int CPolytope::is_in(Point p) {
    bool bd_flag = false;
    for (auto &hp : this->half_planes) {
        int st = hp.is_in(p);
        if (st == -1)
            return false;
        if (st == 0)
            bd_flag = true;
    }

    if (bd_flag == true)
        return 0;
    return 1;
}

CPolytope:: ~CPolytope() {
    ;
}