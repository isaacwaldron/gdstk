/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#ifndef GDSTK_HEADER_CLIPPER_TOOLS
#define GDSTK_HEADER_CLIPPER_TOOLS

#define __STDC_FORMAT_MACROS
#define _USE_MATH_DEFINES

#include <stdint.h>

#include "array.h"
#include "polygon.h"

namespace gdstk {

enum struct Operation { Or, And, Xor, Not };
enum struct OffsetJoin { Miter, Bevel, Round };
enum struct ShortCircuit { None, Any, All };

// The following operations are executed in an integer grid of vertices, so the
// geometry should be scaled by a large enough factor to garante a minimal
// precision level.  However, if the scaling factor is too large, it may cause
// overflow of coordinates.

// Resulting polygons are appended to result.
ErrorCode boolean(const Array<Polygon*>& polys1, const Array<Polygon*>& polys2, Operation operation,
                  double scaling, Array<Polygon*>& result);

inline ErrorCode boolean(const Polygon& poly1, const Polygon& poly2, Operation operation,
                         double scaling, Array<Polygon*>& result) {
    const Polygon* p1 = &poly1;
    const Polygon* p2 = &poly2;
    const Array<Polygon*> polys1 = {.capacity = 1, .count = 1, .items = (Polygon**)&p1};
    const Array<Polygon*> polys2 = {.capacity = 1, .count = 1, .items = (Polygon**)&p2};
    return boolean(polys1, polys2, operation, scaling, result);
}

// Dilates or erodes polygons acording to distance (negative distance results
// in erosion).  The effects of internal polygon edges (in polygons with holes,
// for example) can be suppressed by setting use_union to true.  Resulting
// polygons are appended to result.
ErrorCode offset(const Array<Polygon*>& polys, double distance, OffsetJoin join, double tolerance,
                 double scaling, bool use_union, Array<Polygon*>& result);

// Check whether the points in groups are inside or outside the set of
// polygons.  Checking within each group can be short-circuited to analyse if
// *any* point in the group is inside the polygon set, or if *all* of the
// points in the group are.  When no short-circuit is used, checks for each
// point are appended to results; otherwise, a single check per group.
void inside(const Array<Polygon*>& groups, const Array<Polygon*>& polygons,
            ShortCircuit short_circuit, double scaling, Array<bool>& result);

inline void inside(const Array<Polygon*>& groups, const Polygon& polygon,
                   ShortCircuit short_circuit, double scaling, Array<bool>& result) {
    const Polygon* p_poly = &polygon;
    const Array<Polygon*> polygons = {.capacity = 1, .count = 1, .items = (Polygon**)&p_poly};
    inside(groups, polygons, short_circuit, scaling, result);
}

inline void inside(const Polygon& points, const Array<Polygon*>& polygons,
                   ShortCircuit short_circuit, double scaling, Array<bool>& result) {
    const Polygon* p_points = &points;
    const Array<Polygon*> groups = {.capacity = 1, .count = 1, .items = (Polygon**)&p_points};
    inside(groups, polygons, short_circuit, scaling, result);
}

inline void inside(const Polygon& points, const Polygon& polygon, ShortCircuit short_circuit,
                   double scaling, Array<bool>& result) {
    const Polygon* p_points = &points;
    const Array<Polygon*> groups = {.capacity = 1, .count = 1, .items = (Polygon**)&p_points};
    const Polygon* p_poly = &polygon;
    const Array<Polygon*> polygons = {.capacity = 1, .count = 1, .items = (Polygon**)&p_poly};
    inside(groups, polygons, short_circuit, scaling, result);
}

inline bool all_inside(const Polygon& points, const Array<Polygon*>& polygons, double scaling) {
    const Polygon* p_points = &points;
    const Array<Polygon*> groups = {.capacity = 1, .count = 1, .items = (Polygon**)&p_points};
    bool value = false;
    Array<bool> result = {.capacity = 1, .count = 0, .items = &value};
    inside(groups, polygons, ShortCircuit::All, scaling, result);
    return value;
}

inline bool all_inside(const Polygon& points, const Polygon& polygon, double scaling) {
    const Polygon* p_points = &points;
    const Array<Polygon*> groups = {.capacity = 1, .count = 1, .items = (Polygon**)&p_points};
    const Polygon* p_poly = &polygon;
    const Array<Polygon*> polygons = {.capacity = 1, .count = 1, .items = (Polygon**)&p_poly};
    bool value = false;
    Array<bool> result = {.capacity = 1, .count = 0, .items = &value};
    inside(groups, polygons, ShortCircuit::All, scaling, result);
    return value;
}

inline bool any_inside(const Polygon& points, const Array<Polygon*>& polygons, double scaling) {
    const Polygon* p_points = &points;
    const Array<Polygon*> groups = {.capacity = 1, .count = 1, .items = (Polygon**)&p_points};
    bool value = false;
    Array<bool> result = {.capacity = 1, .count = 0, .items = &value};
    inside(groups, polygons, ShortCircuit::Any, scaling, result);
    return value;
}

inline bool any_inside(const Polygon& points, const Polygon& polygon, double scaling) {
    const Polygon* p_points = &points;
    const Array<Polygon*> groups = {.capacity = 1, .count = 1, .items = (Polygon**)&p_points};
    const Polygon* p_poly = &polygon;
    const Array<Polygon*> polygons = {.capacity = 1, .count = 1, .items = (Polygon**)&p_poly};
    bool value = false;
    Array<bool> result = {.capacity = 1, .count = 0, .items = &value};
    inside(groups, polygons, ShortCircuit::Any, scaling, result);
    return value;
}

// Slice the given polygon along the coordinates in posiotions.  Cuts are
// vertical (horizontal) when x_axis is set to true (false).  Argument result
// must be an array with length at least positions.count + 1.  The resulting
// slices are appendend to the arrays in their respective bins.
ErrorCode slice(const Polygon& polygon, const Array<double>& positions, bool x_axis, double scaling,
                Array<Polygon*>* result);

}  // namespace gdstk

#endif
