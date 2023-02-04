
#ifndef MANUALEXTRUDER_OBJECT_EXTRUDER_H
#define MANUALEXTRUDER_OBJECT_EXTRUDER_H
#include "clipper.hpp"
#include <unordered_set>
#include <unordered_map>

// We assume that contours do not touch, all lines belong to only one sidewall contour.

struct Point
{
	// XYZ coordinates
	std::array<double, 3> coords{0, 0, 0};
	int idx = -1;
//	void create_from_clipper_point(const ClipperLib::IntPoint &cpoint, double scale, double zpos)
//	{
//		coords[0] = cpoint.X * scale;
//		coords[1] = cpoint.Y * scale;
//		coords[2] = zpos;
//	};
	void create_from_clipper_point(const ClipperLib::IntPoint &cpoint, double zpos)
	{
		coords[0] = cpoint.X;
		coords[1] = cpoint.Y;
		coords[2] = zpos;
	};
	bool operator<(const Point &r) const { return (idx < r.idx); }
};

struct GraphPoint //Given a point, return its adjacent lines and index. DFS
{
	// Assuming each point is adjacent to only 2 lines!
	std::array<int, 2> adjacent_lines;
	int idx = -1;
	bool visited;
	int num_inserted = 1;
	bool operator<(const GraphPoint &r) const { return (idx < r.idx); }
};

struct Line
{
	// Straight line between the start [0] and end point [1]
	std::array<int, 2> endpoints{0, 0};
	int idx = -1;
	bool operator<(const Line &r) const { return (idx < r.idx); }
};

struct Contour
{
	// Must be a closed set of lines and orientation matters when defining surface
	std::vector<int> lines;
	int idx = -1;
	bool operator<(const Contour &r) const { return (idx < r.idx); }
};

struct Surface
{
	// The first contour defines the boundary of the surface. Any remaining contours define holes in
	// the surface
	std::vector<int> contours;
	int idx;
	bool operator<(const Surface &r) const { return (idx < r.idx); }
};

/// All of the geometry entities needed to define a single ExtrudedObject
struct GeometryStorage
{
	// Entities are stored in sorted arrays, so the index within each struct should start from 0
	std::vector<Point> points;
	std::vector<Line> lines;
	std::vector<Contour> contours;
	std::vector<Surface> surfaces;

	std::vector<int> sides;//side surfaces
	std::vector<int> upper;//upper surfaces
	std::vector<int> lower;//lower surfaces
	std::unordered_map<int, int> upper_map, lower_map;//first int: footprint number, second int: surface number
};

/// Simple type describing a footprint (surface) returned by Clipper.
/// Outer vector represents all surfaces defining the footprint. They must be adjacent (share
/// lines). Paths represent a single surface, where the first path ([0]) is the exterior contour and
/// the remaining paths [1:end] represent any present holes.
using footprint = std::vector<ClipperLib::Paths>;
bool free_compare(const Point &lhs, const Point &rhs);
bool index_compare(const Point &lhs, const Point &rhs);

// Compare lines based on endpoints
size_t line_hash(const Line &line);
bool line_compare(const Line &lhs, const Line &rhs);
// Compare lines based on index
size_t line_idx_hash(const Line &line);
bool line_idx_compare(const Line &lhs, const Line &rhs);

size_t contour_hash(const Contour &contour);
bool contour_compare(const Contour &lhs, const Contour &rhs);

using PointSet = std::set<Point, bool (*)(const Point &, const Point &)>;

struct pair_hash {
	std::size_t operator () (const std::pair<int,int> &p) const {
		auto h1 = std::hash<int>{}(p.first);
		auto h2 = std::hash<int>{}(p.second);

		// Mainly for demonstration purposes, i.e. works but is overly simple
		// In the real world, use sth. like boost.hash_combine
		return (53 + h1) *53 +  h2;
	}
};

PointSet unique_points_of_footprint(int index, const footprint &surface, double z, double scaling_factor);

using LineSet =std::unordered_set<Line, size_t (*)(const Line &), bool (*)(const Line &, const Line &)>;
std::pair<LineSet, LineSet> label_unique_repeated_lines(int lIndex, const footprint &surface_footprint,double z_pos, const PointSet &unique_points,double scaling_factor);
LineSet find_sidewall_lines(const footprint &lower, double z_bottom, const std::pair<LineSet,LineSet> &label_result);

using GraphSet = std::unordered_map<int, GraphPoint>;
GraphSet adjacentLinesGraph(const footprint &upper,const LineSet &upper_once_lines);
std::vector<std::vector<int>> groupIntoContour(GraphSet &graph,const LineSet &unique_lines);
std::vector<std::vector<int>> points_on_all_contours(const std::vector<std::vector<int>> &group,const LineSet &unique_indexed_lines);
std::pair<LineSet, std::vector<int>> match_common_contours(const std::vector<std::vector<int>> &upper_group,
										   std::vector<std::vector<int>> &lower_group,
										   int line_size, double z_bottom, const PointSet &upper_index_points,
										   const PointSet &lower_points,
	const std::unordered_set<Line, size_t (*)(const Line &), bool (*)(const Line &, const Line &)>
		&unique_indexed_lines,
	const std::unordered_set<Line, size_t (*)(const Line &), bool (*)(const Line &, const Line &)>
		&unique_lines);
using ContourSet = std::unordered_set<Contour, size_t (*)(const Contour &),bool (*)(const Contour &, const Contour &)>;
using contour_footprint_map = std::unordered_map<int,std::pair<int,int>>;
using footprint_contour_map = std::unordered_map<std::pair<int,int>,int,pair_hash>;
std::pair<ContourSet,contour_footprint_map> Label_contours(const footprint &footprints, double z_top, double scaling_factor,const PointSet &unique_points, const LineSet &unique_upper_lines, int cIndex);
std::pair<int,int> find_endpoints_of_specific_line (int i, LineSet unique_indexed_lines);
std::vector<int> generate_line_loop_between_vertical_lines (const std::vector<int> &point_on_contour,int first_up_endpoint, int second_up_endpoint,const LineSet &unique_endpoints_lines, int negative1_for_lower_or_1_for_upper);
ContourSet sidecontours(const std::vector<std::vector<int>> &upper_group,
						const std::vector<std::vector<int>> &lower_group, const LineSet &unique_indexed_lines,
						const LineSet &vertical_lines, const LineSet &unique_endpoints_lines, const PointSet &unique_index_points, int cIndex,
						int lIndex, int all_lIndex,std::vector<int> number);
GeometryStorage add_surface(const footprint &lower,const footprint &upper, ContourSet side, int cIndex,const contour_footprint_map &upper_contour_surface_set,const contour_footprint_map &lower_contour_surface_set, GeometryStorage &object);
GeometryStorage create_extruded_object(const footprint &lower, const footprint &upper,
									   double z_bottom, double z_top,double scaling_factor);
void add_extruded_object_to_gmsh(const GeometryStorage &object,double lc,double scaling_factor);
void reindex_extruded_object(GeometryStorage &object, int point_start_idx, int line_start_index, int contour_start_idx, int surface_start_idx);
#endif // MANUALEXTRUDER_OBJECT_EXTRUDER_H