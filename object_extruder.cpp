#include "object_extruder.h"
#include "gmsh.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>

	/// Compare and hash functions for points/lines/contours
bool free_compare(const Point &lhs, const Point &rhs) { return lhs.coords < rhs.coords; } //Compare the coordinates of points

bool index_compare(const Point &lhs, const Point &rhs) { return lhs.idx < rhs.idx; } //Compare the indices of points

size_t line_hash(const Line &line) { return line.endpoints[0] * line.endpoints[1]; } //Hash function of line. Multiply two endpoints

bool line_compare(const Line &lhs, const Line &rhs)
{
	return ((lhs.endpoints[0] == rhs.endpoints[1] and lhs.endpoints[1] == rhs.endpoints[0]) or (lhs.endpoints[0] == rhs.endpoints[0] and lhs.endpoints[1] == rhs.endpoints[1]));
}//Compare the endpoints of lines to see if they are the same line
size_t line_idx_hash(const Line &line) { return line.idx; }//Hash function of line. Line index

bool line_idx_compare(const Line &lhs, const Line &rhs) { return lhs.idx == rhs.idx; }//Compare the indices of lines

size_t contour_hash(const Contour &contour) { return contour.idx; }//Hash function of contours. Indices

bool contour_compare(const Contour &lhs, const Contour &rhs) { return lhs.idx == rhs.idx; }//Compare contours by their indices

using PointSet = std::set<Point, bool (*)(const Point &, const Point &)>;


// Only returns unique_points for one surface
// Loop through footprint, insert every point, if insert.second is true, add correct index, otherwise, index does not change.
PointSet unique_points_of_footprint(int index, const footprint &surface, double z, double scaling_factor)
{
	PointSet unique_points(free_compare);
	for (int i = 0; i < surface.size(); i++)
	{
		int upper_contour = surface[i].size();
		for (int j = 0; j < upper_contour; j++)
		{
			for (int k = 0; k < surface[i][j].size(); k++)
			{
				Point test;
				test.idx = index;
				test.create_from_clipper_point(surface[i][j][k],z);//Generate points with x,y coordinates from surface[i][j][k], z coordinate from input
				auto ret = unique_points.insert(test);
				if (ret.second)
				{
					index++;
				}
			}
		}
	}
	return unique_points;
}
/// Helper function for function 2
//Loop through footprints, find two endpoints of a line, use insert again, if insert.second = true, add to unique_lines, otherwise, add to repeated lines.
using LineSet =std::unordered_set<Line, size_t (*)(const Line &), bool (*)(const Line &, const Line &)>;
std::pair<LineSet, LineSet> label_unique_repeated_lines(int lIndex, const footprint &surface_footprint,double z_pos, const PointSet &unique_points,double scaling_factor)
{
	LineSet unique_lines(unique_points.size() , line_hash,line_compare);
	LineSet repeated_lines(unique_points.size() , line_hash,line_compare);
	for (int i = 0; i < surface_footprint.size(); i++)
	{
		for (int j = 0; j < surface_footprint[i].size(); j++)
		{
			for (int k = 0; k < surface_footprint[i][j].size() - 1; k++) //Three for loops are used to loop through footprints
			{
				Point a;
				a.create_from_clipper_point(surface_footprint[i][j][k],
											z_pos);
				auto start_pos = unique_points.find(a);
				int start_point = (*start_pos).idx; //Find its index given coordinates
				Point b;
				b.create_from_clipper_point(surface_footprint[i][j][k+1],
											z_pos);
				auto end_pos = unique_points.find(b);
				int end_point = (*end_pos).idx;
				Line c;
				c.idx = lIndex;
				c.endpoints[0] = start_point;
				c.endpoints[1] = end_point;
				auto ret = unique_lines.insert(c);//Create unique lines by inserting it to lineset
				if (ret.second)
				{
					lIndex++;
				}
				else
				{
					auto res = repeated_lines.insert(c); //If the line is repeated, insert it to repeated lines
				}
			}
			Point a;
			a.create_from_clipper_point(surface_footprint[i][j][surface_footprint[i][j].size() - 1], z_pos);
			auto start_pos = unique_points.find(a);
			int start_point = (*start_pos).idx;
			Point b;
			b.create_from_clipper_point(surface_footprint[i][j][0], z_pos);
			auto end_pos = unique_points.find(b);
			int end_point = (*end_pos).idx;
			Line c;
			c.idx = lIndex;
			c.endpoints[0] = start_point;
			c.endpoints[1] = end_point;
			auto ret = unique_lines.insert(c);
			if (ret.second)
			{
				lIndex++;
			}
			else
			{
				auto res = repeated_lines.insert(c);
			}
		}
	}
	return {unique_lines, repeated_lines};
}

/// 2. Label unique lines that are only labelled once
// if a line is labelled once, then it is on  the side wall contour.
LineSet find_sidewall_lines(const footprint &lower, double z_bottom, const std::pair<LineSet,LineSet> &label_result)
{
	LineSet unique_lines = label_result.first;
	LineSet once_lines(unique_lines.bucket_count(), line_hash, line_compare);
	LineSet repeated_lines = label_result.second;
	for (auto it = unique_lines.begin(); it != unique_lines.end(); ++it)
	{
		auto found = repeated_lines.find(*it);//If the iterator can be found in repeated_lines, don't add it to once lines
		if (found == unique_lines.end())
		{
			once_lines.insert(*it);
		}
	}
	return once_lines;
}

/// Group Lines into Sidewall Contours.
using GraphSet = std::unordered_map<int, GraphPoint>;
//Make a graph, given an index, return adjacent lines
GraphSet adjacentLinesGraph(const footprint &upper,const LineSet &upper_once_lines)
{
	GraphSet upper_graph(upper_once_lines.size());
	for (auto it = upper_once_lines.begin(); it != upper_once_lines.end(); ++it)
	{
		GraphPoint start, end; //Set visited = false in this function and it will be changed in groupIntoContour
		start.idx = it->endpoints[0]; //Treat endpoints[0] as start graphpoint's index
		start.visited = false;
		start.adjacent_lines[0] = it->idx;//Find one of the adjacent_lines of start graphpoint
		end.idx = it->endpoints[1];
		end.adjacent_lines[1] = it->idx;
		end.visited = false;

		auto inserted = upper_graph.insert(GraphSet::value_type(start.idx, start));
		auto inserted2 = upper_graph.insert(GraphSet::value_type(end.idx, end));

		if (!inserted.second) //If start graphpoint is inserted successfully,change its values (can be found in .h file)
		{
			inserted.first->second.adjacent_lines[0] =it->idx;
			inserted.first->second.num_inserted++;
			if(inserted.first->second.num_inserted>2)
			{
				std::cout << "[ERROR] adjacentLinesGraph(): Input polygons not supported. Sidewall points shared by more than 2 lines." << std::endl;
				std::abort();
			}
		}

		if (!inserted2.second)
		{
			inserted2.first->second.adjacent_lines[1] =it->idx;
			inserted2.first->second.num_inserted++;
			if(inserted2.first->second.num_inserted>2)
			{
				std::cout << "[ERROR] adjacentLinesGraph(): Input polygons not supported. Sidewall points shared by more than 2 lines." << std::endl;
				std::abort();
			}
		}
	}
	return upper_graph;
}
//Use adjacent_lines_graph to form contours
std::vector<std::vector<int>> groupIntoContour(GraphSet &graph,const LineSet &unique_lines)
{
	std::vector<std::vector<int>> group;
	std::vector<int> curveTags;
	for (auto it = graph.cbegin(); it != graph.cend(); ++it)
	{
		if (!it->second.visited)
		{
			GraphPoint first;
			auto iterator = graph.find(it->second.idx);
			if (iterator != graph.end())
			{
				iterator->second.visited = true; //If iterator can be found in graph, visited = true
			}
			first = iterator->second; // Identify the graphpoint, find its adjacent lines, then find its another graphpoint, connect them
			Line adj_line;
			adj_line.idx = it->second.adjacent_lines[0];	 // choose one adjacent line
			auto adj_line_pos = unique_lines.find(adj_line); // find its endpoints
			int start_point = adj_line_pos->endpoints[0];
			int end_point = adj_line_pos->endpoints[1];
			GraphPoint sub; // choose the endpoint which has not been visited
			if (it->second.idx == start_point)
			{
				sub.idx = end_point;
			}
			else
			{
				sub.idx = start_point;
			}
			curveTags.push_back(adj_line.idx);//Find its adjacent line and add to curveTags
			auto insert = graph.find(sub.idx);
			GraphPoint next_point;
			insert->second.visited = true;
			next_point = insert->second;//Find the other graphpoint in the other end of the adjacent line
			while (next_point.idx != first.idx) //The rest of the graph is completed in this while loop, it will keep adding elements until it goes back to the first point
			{
				if (next_point.adjacent_lines[0] == adj_line.idx)
				{
					adj_line.idx = next_point.adjacent_lines[1];
				}
				else
				{
					adj_line.idx = next_point.adjacent_lines[0];
				}
				adj_line_pos = unique_lines.find(adj_line);
				start_point = adj_line_pos->endpoints[0];
				end_point = adj_line_pos->endpoints[1];
				if (next_point.idx == start_point)
				{
					next_point.idx = end_point;
				}
				else
				{
					next_point.idx = start_point;
				}
				if (adj_line.endpoints[0] == start_point) // find the correct direction
				{
					adj_line.idx = -adj_line.idx;
				}
				auto repeat = std::find(curveTags.begin(),curveTags.end(),adj_line.idx);
				assert(repeat == curveTags.end());
				curveTags.push_back(adj_line.idx);
				auto inserted = graph.find(next_point.idx);
				inserted->second.visited = true;
				next_point = inserted->second;
			}
			group.push_back(curveTags);
			curveTags.clear();
		}
	}
	return group;
}
// Take a group of line_index, find all points on all contours on a surface
std::vector<std::vector<int>> points_on_all_contours(const std::vector<std::vector<int>> &group,
													 const LineSet &unique_indexed_lines)
{

	std::vector<std::vector<int>> points_on_all_lower_contour;
	std::vector<int> points_on_each_lower_contour;
	for (int k = 0; k < group.size(); k++) // Loop through group, to find points index on different contours.
	{
		for (int n = 0; n < group[k].size(); n++)
		{
			Line line;
			line.idx = group[k][n];
			if(line.idx != -1)
			{
				auto pos = unique_indexed_lines.find(line);
				assert(pos != unique_indexed_lines.end());
				int start = pos->endpoints[0];
				if (std::find(points_on_each_lower_contour.cbegin(),points_on_each_lower_contour.cend(),start) == points_on_each_lower_contour.cend())
				{
					points_on_each_lower_contour.push_back(start);
				}
			}
		}
		points_on_all_lower_contour.push_back(points_on_each_lower_contour);
		points_on_each_lower_contour.clear();
	}
	return points_on_all_lower_contour;
}
//Match lower and upper contours. Find the common contours and connect them by vertical lines. If
//the contours order do not match, correct them.


std::pair<LineSet, std::vector<int>>
match_common_contours(const std::vector<std::vector<int>> &upper_group,
										   std::vector<std::vector<int>> &lower_group,
										   int line_size, double z_bottom, const PointSet &upper_index_points,
										   const PointSet &lower_points, const LineSet &unique_indexed_lines,
										   const LineSet &unique_lines)
{
	LineSet vertical_lines(upper_index_points.size() + lower_points.size(), line_idx_hash,line_idx_compare);
	std::vector<std::vector<int>> ordered_lower_group;
	std::vector<int> low_sub_points;
	std::vector<std::vector<int>> points_on_all_contour;
	points_on_all_contour = points_on_all_contours(lower_group, unique_indexed_lines);
	std::vector<int> number; // Find the number of last vertical line on each contour
	for (int i = 0; i < upper_group.size(); i++)
	{
		for (int j = 0; j < upper_group[i].size(); j++)
		{
			int line_num = std::abs(upper_group[i][j]);
			Line it;
			it.idx = line_num;
			auto line_pos = unique_indexed_lines.find(it);
			int start = line_pos->endpoints[0];
			Point up;
			up.idx = start;
			auto up_pointer = upper_index_points.find(up);
			up.coords[0] = up_pointer->coords[0];
			up.coords[1] = up_pointer->coords[1];
			Point low;
			low.coords[0] = up.coords[0];
			low.coords[1] = up.coords[1];
			low.coords[2] = z_bottom;
			auto low_pointer = lower_points.find(low);
			low.idx = low_pointer->idx;
			if (low_pointer != lower_points.end())
			{
				auto prev_line = line_pos; //Look for the previous point
				if(j == 0)
				{
					Line last;
					last.idx = std::abs(upper_group[i][upper_group[i].size()-1]);
					prev_line = unique_indexed_lines.find(last);
				}else
				{
					Line a;
					a.idx = std::abs(upper_group[i][j-1]);
					prev_line = unique_indexed_lines.find(a);
				}
				int prev_point = prev_line->endpoints[0];
				Point prev;
				prev.idx = prev_point;
				auto prev_pos = upper_index_points.find(prev);
				prev.coords[0] = prev_pos->coords[0];
				prev.coords[1] = prev_pos->coords[1];
				auto next_line = line_pos;// Look for the next point
				if( j == upper_group[i].size()-1)
				{
					Line first;
					first.idx = std::abs(upper_group[i][0]);
					next_line = unique_indexed_lines.find(first);
				}else
				{
					Line b;
					b.idx = std::abs(upper_group[i][j+1]);
					next_line = unique_indexed_lines.find(b);
				}
				int next_point = next_line->endpoints[0];
				Point next;
				next.idx = next_point;
				auto next_pos = upper_index_points.find(next);
				next.coords[0] = next_pos->coords[0];
				next.coords[1] = next_pos->coords[1];

				if((up.coords[0] - prev.coords[0] == 0 and next.coords[0] - up.coords[0] == 0) or (up.coords[1] - prev.coords[1] == 0 and next.coords[1] - up.coords[1] == 0) or ((up.coords[0] - prev.coords[0])/(up.coords[1] - prev.coords[1]) == (next.coords[0] - up.coords[0])/(next.coords[1] - up.coords[1])))//on the same x line
				{//if previous and next slope are the same, do not add vertical lines
					continue;
				}else
				{
					line_size++;
					int end_point = (*low_pointer).idx;
					Line c;
					c.endpoints[0] = start;
					c.endpoints[1] = end_point;
					c.idx = line_size;
					vertical_lines.insert(c);
					low_sub_points.push_back(low.idx);
				}
			}
		}
		number.push_back(vertical_lines.size());
		for (int m = 0; m < lower_group.size(); m++) // Loop through lower_group, to insert vectors with correct order into ordered_lower_group
		{
			if(low_sub_points.size() != 0 )
			{
				if (std::find(points_on_all_contour[m].cbegin(), points_on_all_contour[m].cend(),
							  low_sub_points[0]) != points_on_all_contour[m].cend())
				{
					ordered_lower_group.push_back(lower_group[m]);
				}
			}else
			{
				ordered_lower_group.push_back({-1});
			}
		}
		low_sub_points.clear();
	}
	lower_group = ordered_lower_group;
	return {vertical_lines, number};
}

/// 3. Label contours
using ContourSet = std::unordered_set<Contour, size_t (*)(const Contour &),bool (*)(const Contour &, const Contour &)>;
using contour_footprint_map = std::unordered_map<int,std::pair<int,int>>;//The key is contour index, the second int is footprint number
std::pair<ContourSet, contour_footprint_map> Label_contours(const footprint &footprints, double z_top, double scaling_factor,const PointSet &unique_points, const LineSet &unique_upper_lines, int cIndex)
{
	ContourSet contours(unique_upper_lines.size() , contour_hash,contour_compare);
	contour_footprint_map contour_footprint;
	for (int i = 0; i < footprints.size(); i++)
	{
		for (int j = 0; j < footprints[i].size(); j++)//Loop through footprints
		{
			std::vector<int> curveTags(footprints[i][j].size());
			for (int k = 0; k < footprints[i][j].size() - 1; k++)
			{
				//Find points a&b, and then find line c,correct its direction and then add it to curveTags
				Point a;
				a.create_from_clipper_point(footprints[i][j][k],z_top);
				auto start_pos = unique_points.find(a);
				int start_point = (*start_pos).idx;
				Point b;
				b.create_from_clipper_point(footprints[i][j][k + 1],z_top);
				auto end_pos = unique_points.find(b);
				int end_point = (*end_pos).idx;
				Line c;
				c.endpoints[0] = start_point;
				c.endpoints[1] = end_point;
				auto line_start_pos = unique_upper_lines.find(c);
				assert(line_start_pos != unique_upper_lines.end());
				int line_start = 0;
				if ((*line_start_pos).endpoints[0] != start_point) //Change direction
				{
					line_start = -(*line_start_pos).idx;
				}
				else
				{
					line_start = (*line_start_pos).idx;
				}
				curveTags[k] = line_start;
			}
			Point a;//The last case
			a.create_from_clipper_point(footprints[i][j][footprints[i][j].size() - 1],z_top);
			auto start_pos = unique_points.find(a);
			int start_point = (*start_pos).idx;
			Point b;
			b.create_from_clipper_point(footprints[i][j][0], z_top);
			auto end_pos = unique_points.find(b);
			int end_point = (*end_pos).idx;
			Line c;
			c.endpoints[0] = start_point;
			c.endpoints[1] = end_point;
			auto line_start_pos = unique_upper_lines.find(c);
			assert(line_start_pos != unique_upper_lines.end());
			int line_start = 0;
			if ((*line_start_pos).endpoints[0] != start_point)
			{
				line_start = -(*line_start_pos).idx;
			}
			else
			{
				line_start = (*line_start_pos).idx;
			}
			curveTags[footprints[i][j].size() - 1] = line_start;
			Contour upper_contour;
			contour_footprint.insert(contour_footprint_map ::value_type(cIndex, {i,j}));
			upper_contour.idx = cIndex;
			upper_contour.lines = curveTags;
			contours.insert(upper_contour);
			cIndex++;
		}
	}
	return {contours,contour_footprint};
}
//A helper function in sidecontours
std::pair<int,int> find_endpoints_of_specific_line (int i, LineSet unique_indexed_lines)
{
	Line transient;
	transient.idx = i;
	auto pos = unique_indexed_lines.find(transient);
	assert(pos != unique_indexed_lines.end());
	int up_endpoints = pos->endpoints[0];
	int low_endpoints = pos->endpoints[1];
	return {up_endpoints,low_endpoints};
}

std::vector<int> generate_line_loop_between_vertical_lines (const std::vector<int> &point_on_contour,int first_up_endpoint, int second_up_endpoint, const LineSet &unique_endpoints_lines, int negative1_for_lower_or_1_for_upper, PointSet unique_index_points)
{
	std::vector<int> line_loop;
	auto first_position =std::find(point_on_contour.cbegin(), point_on_contour.cend(), first_up_endpoint);//Find the first point on the upper footprint and consider it as the first point of the line loop
	assert(first_position != point_on_contour.end());
	std::vector<int> connectFirstLastupper = point_on_contour;
	connectFirstLastupper.insert(connectFirstLastupper.end(),std::begin(point_on_contour), std::prev(point_on_contour.end()));
	//Find the first and second upper endpoints, then find its line
	auto first_pos =std::find(connectFirstLastupper.cbegin(),connectFirstLastupper.cend(), first_up_endpoint);
	auto second_pos =std::find(connectFirstLastupper.cbegin(),connectFirstLastupper.cend(), second_up_endpoint);
	Line temp;
	temp.endpoints[0] = *first_pos;
	temp.endpoints[1] = *second_pos;
	auto line_position = unique_endpoints_lines.find(temp);
	if(line_position != unique_endpoints_lines.end())
	{
		int line_idx = line_position->idx;
		if (line_position->endpoints[0] != *first_pos)
		{
			line_loop.push_back(-line_idx * negative1_for_lower_or_1_for_upper);//Correct its direction
		}
		else
		{
			line_loop.push_back(line_idx * negative1_for_lower_or_1_for_upper);
		}
	}else{
		while (*first_pos != *second_pos) // If there are a lot of points between two vertical lines, add them to upper_line_loop
		{
			Line tem;
			auto next_first = std::next(first_pos);
			Point a;
			a.idx = *next_first;
			auto next_first_point_pos = unique_index_points.find(a);
			Point b;
			b.idx = *first_pos;
			auto first_point_pos = unique_index_points.find(b);
			Point c;
			c.idx = *second_pos;
			auto second_point_pos = unique_index_points.find(c);

			if((next_first_point_pos->coords[0] < first_point_pos->coords[0] and next_first_point_pos->coords[0] < second_point_pos->coords[0]) or (next_first_point_pos->coords[0] > first_point_pos->coords[0] and next_first_point_pos->coords[0] > second_point_pos->coords[0]))
			{ //if the next_first_point is not between first_point_pos and second_point_pos, use the second_pos to find the correct order
				tem.endpoints[0] = *second_pos;
				tem.endpoints[1] = *std::next(second_pos);
				auto line_pos = unique_endpoints_lines.find(tem);
				assert(line_pos != unique_endpoints_lines.end());
				int line_idx = line_pos->idx;
				if (line_pos->endpoints[0] != *first_pos)
				{
					line_loop.push_back(-line_idx * negative1_for_lower_or_1_for_upper);
				}
				else
				{
					line_loop.push_back(line_idx * negative1_for_lower_or_1_for_upper);
				}
				second_pos++;
			}else
			{//otherwise,use the first_pos to find the correct order
				tem.endpoints[0] = *first_pos;
				tem.endpoints[1] = *next_first;
				auto line_pos = unique_endpoints_lines.find(tem);
				assert(line_pos != unique_endpoints_lines.end());
				int line_idx = line_pos->idx;
				if (line_pos->endpoints[0] != *first_pos)
				{
					line_loop.push_back(-line_idx * negative1_for_lower_or_1_for_upper);
				}
				else
				{
					line_loop.push_back(line_idx * negative1_for_lower_or_1_for_upper);
				}
				first_pos++;
			}
		}
	}

	return line_loop;
}

ContourSet sidecontours(const std::vector<std::vector<int>> &upper_group,
						const std::vector<std::vector<int>> &lower_group, const LineSet &unique_indexed_lines,
						const LineSet &vertical_lines, const LineSet &unique_endpoints_lines, const PointSet &unique_index_points, int cIndex,
						int lIndex, int all_lIndex,std::vector<int> number) // lIndex is the index before adding vertical lines
{
	ContourSet side(vertical_lines.size(), contour_hash, contour_compare);
	std::vector<std::vector<int>> points_on_all_lower_contour;
	points_on_all_lower_contour = points_on_all_contours(lower_group, unique_indexed_lines);
	std::vector<std::vector<int>> points_on_upper_contour;
	points_on_upper_contour = points_on_all_contours(upper_group, unique_indexed_lines);
	int common_contour_size;
	common_contour_size = std::max(points_on_upper_contour.size(),points_on_all_lower_contour.size());
	for (int i = lIndex + 1; i < all_lIndex + 1; i++)
	{
		if (std::find(number.cbegin(), number.cend(), i - lIndex) ==number.cend()) // check if i-lIndex is the last one of a contour
		{
			auto first_pair = find_endpoints_of_specific_line(i,unique_indexed_lines);

			int first_up_endpoint = first_pair.first;
			int first_low_endpoint = first_pair.second;

			auto second_pair = find_endpoints_of_specific_line(i+1,unique_indexed_lines);
			int second_up_endpoint = second_pair.first;
			int second_low_endpoint = second_pair.second;

			std::vector<int> upper_line_loop;
			std::vector<int> low_line_loop;
			for (int j = 0; j < common_contour_size; j++)
			{
				auto second_position =std::find(points_on_upper_contour[j].cbegin(),points_on_upper_contour[j].cend(), second_up_endpoint);
				if (second_position !=points_on_upper_contour[j].cend()) // Find the correct contour that contains these points
				{
					upper_line_loop = generate_line_loop_between_vertical_lines(points_on_upper_contour[j],first_up_endpoint,second_up_endpoint,unique_endpoints_lines,1, unique_index_points);
					low_line_loop = generate_line_loop_between_vertical_lines(points_on_all_lower_contour[j],first_low_endpoint,second_low_endpoint,unique_endpoints_lines,-1,unique_index_points);
					upper_line_loop.push_back(i + 1); // vertical lines
					for (int p = 0; p < low_line_loop.size(); p++)
					{
						upper_line_loop.push_back(low_line_loop[p]); // add low_line_loop
					}
					upper_line_loop.push_back(-i); // vertical lines
					Contour contour;
					contour.idx = cIndex + 1;
					contour.lines = upper_line_loop;
					side.insert(contour);
					cIndex++;
					upper_line_loop.clear();
					low_line_loop.clear();

				}
			}
		}
		else // if this is the last point on a contour, then we need to connect it with the first point
		{
			int first = 0;
			auto it = std::find(number.cbegin(), number.cend(), i - lIndex);
			assert(it != number.end());
			if (*it == number[0]) // if this is just the first contour, the first point is lIndex + 1;
			{
				first = lIndex + 1; // Look for first vertical line
			}
			else // if it is not the first one, we need to know how many points are on the previous contours
			{
				auto prev = std::prev(it);
				first = *prev + lIndex + 1;
			}
			auto first_pair = find_endpoints_of_specific_line(i,unique_indexed_lines);
			int first_up_endpoints = first_pair.first;
			int first_low_endpoints = first_pair.second;

			auto second_pair = find_endpoints_of_specific_line(first,unique_indexed_lines);
			int second_up_endpoints = second_pair.first;
			int second_low_endpoints = second_pair.second;

			std::vector<int> upper_line_loops;
			std::vector<int> lower_line_loops;
			for (int j = 0; j < common_contour_size; j++)
			{
				if (std::find(points_on_upper_contour[j].cbegin(), points_on_upper_contour[j].cend(),second_up_endpoints) != points_on_upper_contour[j].cend())
				{
					upper_line_loops = generate_line_loop_between_vertical_lines(points_on_upper_contour[j],first_up_endpoints,second_up_endpoints,unique_endpoints_lines,1,unique_index_points);
					lower_line_loops = generate_line_loop_between_vertical_lines(points_on_all_lower_contour[j],first_low_endpoints,second_low_endpoints,unique_endpoints_lines,-1,unique_index_points);
					upper_line_loops.push_back(first);
					for (int &low_line_loop : lower_line_loops)
					{
						upper_line_loops.push_back(low_line_loop);
					}
					upper_line_loops.push_back(-i);
				}
			}
			Contour contours;
			contours.idx = cIndex + 1;
			contours.lines = upper_line_loops;
			side.insert(contours);
			cIndex++;
		}
	}
	return side;
}

using footprint_contour_map = std::unordered_map<std::pair<int,int>,int,pair_hash>;
footprint_contour_map convert_key(const contour_footprint_map &contour_surface_set)
{
	footprint_contour_map converted_map;
	for (auto x:contour_surface_set)
	{
		int key = x.first;
		std::pair<int,int> footprint_pair = x.second;
		converted_map.insert(std::unordered_map<std::pair<int,int>,int,pair_hash>::value_type(footprint_pair,key));
	}
	return converted_map;
}
GeometryStorage add_surface(const footprint &lower,const footprint &upper, ContourSet side, int cIndex,const footprint_contour_map converted_upper, const footprint_contour_map converted_lower, const contour_footprint_map &upper_contour_surface_set,const contour_footprint_map &lower_contour_surface_set, GeometryStorage &object)
{
	GeometryStorage &geom = object;
	int upper_num = 0;
	for (int v = 0; v < upper.size(); v++)
	{
		geom.surfaces[v].idx = v + 1;
		geom.upper.push_back(v+1);
		for (int i = 0; i < upper[v].size(); i++)
		{
			auto iterator = converted_upper.find({v,i});
			geom.surfaces[v].contours.push_back(iterator->second);
			upper_num++;
			auto it = upper_contour_surface_set.find(iterator->second);
			if(it != upper_contour_surface_set.end())
			{
				geom.upper_map.insert(std::unordered_map<int, int>::value_type(it->second.first,v+1));
			}
		}
	}
	for (int v = upper.size(); v < upper.size() + lower.size(); v++)
	{
		geom.surfaces[v].idx = v + 1;
		geom.lower.push_back(v + 1);
		for (int i = 0; i < lower[v - upper.size()].size(); i++)
		{
			auto iterator = converted_lower.find({v-upper.size(),i});
			geom.surfaces[v].contours.push_back(-iterator->second);
			auto it = lower_contour_surface_set.find(iterator->second); //find_contour, then get its footprint number
			if(it != lower_contour_surface_set.end())
			{
				geom.lower_map.insert(std::unordered_map<int, int>::value_type(it->second.first,v+1));//it->second.first is footprint i number
			}
		}
		upper_num ++;
	}

	for (int v = upper.size() + lower.size(); v < upper.size() + lower.size() + side.size(); v++)
	{
		geom.surfaces[v].idx = v + 1;
		geom.surfaces[v].contours.push_back(-(cIndex + 1));
		geom.sides.push_back(v+1);
		cIndex++;
	}
	return object;
}

GeometryStorage create_extruded_object(const footprint &lower,const footprint &upper,
									   double z_bottom, double z_top,double scaling_factor)
{
//	auto time1 = std::chrono::high_resolution_clock::now();
	GeometryStorage extruded_object;
	GeometryStorage &geom = extruded_object;

	//	1) Label unique points
//	auto t1 = std::chrono::high_resolution_clock::now();
	PointSet upper_points = unique_points_of_footprint(1, upper, z_top, scaling_factor);
	PointSet lower_points =
		unique_points_of_footprint(upper_points.size() + 1, lower, z_bottom, scaling_factor);
	PointSet unique_points(free_compare);
	unique_points.insert(upper_points.cbegin(), upper_points.cend());
	unique_points.insert(lower_points.cbegin(), lower_points.cend());

	PointSet upper_index_points(index_compare);
	upper_index_points.insert(upper_points.cbegin(), upper_points.cend());
	PointSet lower_index_points(index_compare);
	lower_index_points.insert(lower_points.cbegin(), lower_points.cend());
	PointSet unique_index_points(index_compare);
	unique_index_points.insert(upper_index_points.cbegin(),upper_index_points.cend());
	unique_index_points.insert(lower_index_points.cbegin(),lower_index_points.cend());
	geom.points.assign(unique_points.cbegin(), unique_points.cend());
//	auto t2 = std::chrono::high_resolution_clock::now();
//	std::cout << "Label unique points took "<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<< " milliseconds\n";

	// 2) Label unique lines
//	t1 = std::chrono::high_resolution_clock::now();
	auto upper_unique_repeated =
		label_unique_repeated_lines(1, upper, z_top, upper_points, scaling_factor);
	LineSet unique_upper_lines = upper_unique_repeated.first; /// first is unique lines,second is repeated lines
	auto lower_unique_repeated = label_unique_repeated_lines(
		unique_upper_lines.size() + 1, lower, z_bottom, lower_points, scaling_factor);
	LineSet unique_lower_lines = lower_unique_repeated.first;

	LineSet unique_lines(unique_points.size() , line_hash,line_compare);
	unique_lines.insert(unique_upper_lines.cbegin(), unique_upper_lines.cend());
	unique_lines.insert(unique_lower_lines.cbegin(), unique_lower_lines.cend());

	/// Create a set of lines based on index
	LineSet unique_indexed_upper_lines(unique_upper_lines.size(), line_idx_hash, line_idx_compare);
	unique_indexed_upper_lines.insert(unique_upper_lines.cbegin(), unique_upper_lines.cend());
	LineSet unique_indexed_lower_lines(unique_upper_lines.size(), line_idx_hash, line_idx_compare);
	unique_indexed_lower_lines.insert(unique_lower_lines.cbegin(), unique_lower_lines.cend());

	LineSet unique_indexed_lines(unique_upper_lines.size() + unique_lower_lines.size(),line_idx_hash, line_idx_compare);
	unique_indexed_lines.insert(unique_indexed_lower_lines.cbegin(),unique_indexed_lower_lines.cend());
	unique_indexed_lines.insert(unique_indexed_upper_lines.cbegin(),unique_indexed_upper_lines.cend());
	geom.lines.assign(unique_indexed_lines.cbegin(), unique_indexed_lines.cend());
//	t2 = std::chrono::high_resolution_clock::now();
//	std::cout << "Label unique lines took "<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<< " milliseconds\n";

	// 2) determine lines that make up contours
//	t1 = std::chrono::high_resolution_clock::now();
	LineSet upper_once_lines =find_sidewall_lines(upper, z_top,  upper_unique_repeated);
	LineSet lower_once_lines =find_sidewall_lines(lower, z_bottom,  lower_unique_repeated);

	GraphSet upper_graph = adjacentLinesGraph(upper, upper_once_lines);
	std::vector<std::vector<int>> upper_contour = groupIntoContour(upper_graph, unique_indexed_upper_lines);
	GraphSet lower_graph = adjacentLinesGraph(lower, lower_once_lines);
	std::vector<std::vector<int>> lower_contour = groupIntoContour(lower_graph, unique_indexed_lower_lines);

	// Match lower and upper common contours
	auto vertical_lines_and_last_vertical_line_number =
		match_common_contours(upper_contour, lower_contour, unique_lines.size(), z_bottom,
							  upper_index_points, lower_points, unique_indexed_lines, unique_lines);
	LineSet vertical_lines =vertical_lines_and_last_vertical_line_number.first;
	std::vector<int> number = vertical_lines_and_last_vertical_line_number.second;

//	///all_set is only for convenience of testing
//	std::vector<LineSet> all_set;
//	all_set = transfer_from_vector_of_int_to_LineSet(lower_contour, unique_indexed_lines); // convenient to test, make vector of vector of int to vector of lineset

	LineSet all_lines(unique_points.size() , line_hash,line_compare);
	unique_indexed_lines.insert(vertical_lines.cbegin(), vertical_lines.cend());
	all_lines.insert(vertical_lines.cbegin(), vertical_lines.cend());
	all_lines.insert(unique_lines.cbegin(), unique_lines.cend());
	geom.lines.assign(all_lines.cbegin(), all_lines.cend());
//	t2 = std::chrono::high_resolution_clock::now();
//	std::cout << "Match upper/lower contours and generate vertical lines took "<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<< " milliseconds\n";

	// 3) Create contours for top and bottom faces
//	t1 = std::chrono::high_resolution_clock::now();
	auto upper_info = Label_contours(upper, z_top, scaling_factor, unique_points, unique_upper_lines, 1);
	ContourSet upper_contours = upper_info.first;
	contour_footprint_map upper_contour_surface_set = upper_info.second;
	auto lower_info =  Label_contours(lower, z_bottom, scaling_factor, unique_points,unique_lower_lines, upper_contours.size() + 1);
	ContourSet lower_contours =lower_info.first;
	contour_footprint_map lower_contour_surface_set = lower_info.second;

	ContourSet all_contours(unique_lines.size(), contour_hash,contour_compare);

	all_contours.insert(upper_contours.cbegin(), upper_contours.cend());
	all_contours.insert(lower_contours.cbegin(), lower_contours.cend());
	int cIndex = all_contours.size();
	int all_lines_size = all_lines.size();
	// Create contours for sidewalls
//	auto sidetime = std::chrono::high_resolution_clock::now();
	ContourSet side =sidecontours(upper_contour, lower_contour, unique_indexed_lines, vertical_lines,unique_lines,unique_index_points, cIndex, unique_lines.size(), all_lines_size, number);
//	auto endtime = std::chrono::high_resolution_clock::now();
//	std::cout << "sidecontours took "<< std::chrono::duration_cast<std::chrono::milliseconds>(endtime-sidetime).count()<< " milliseconds\n";

	all_contours.insert(side.cbegin(), side.cend());
	geom.contours.assign(all_contours.cbegin(), all_contours.cend());
//	t2 = std::chrono::high_resolution_clock::now();
//	std::cout << "Label all contours took "<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<< " milliseconds\n";


	geom.surfaces.resize(upper.size() + lower.size() + side.size());
//	t1 = std::chrono::high_resolution_clock::now();
	footprint_contour_map converted_upper = convert_key(upper_contour_surface_set);
	footprint_contour_map converted_lower = convert_key(lower_contour_surface_set);
//	t2 = std::chrono::high_resolution_clock::now();
//	std::cout << "convert key took "<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<< " milliseconds\n";
	extruded_object = add_surface(lower,upper,side,cIndex,converted_upper, converted_lower, upper_contour_surface_set,lower_contour_surface_set,extruded_object);



//	auto time2 = std::chrono::high_resolution_clock::now();
//	std::cout << "The whole function took "<< std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1).count()<< " milliseconds\n";
	return extruded_object;
}

void add_extruded_object_to_gmsh(const GeometryStorage &object, double lc,double scaling_factor)
{
	for (auto &point : object.points)
	{
		gmsh::model::geo::addPoint(point.coords[0] * scaling_factor, point.coords[1] * scaling_factor, point.coords[2], lc,
								   point.idx);
	}

	for (auto &line : object.lines)
	{
		gmsh::model::geo::addLine(line.endpoints[0], line.endpoints[1], line.idx);
	}

	for (auto &contour : object.contours)
	{
		gmsh::model::geo::addCurveLoop(contour.lines, contour.idx);
	}

	for (auto &surface : object.surfaces)
	{
		gmsh::model::geo::addPlaneSurface(surface.contours, surface.idx);
	}
	std::vector<int> surface_tags;
	surface_tags = object.upper;
	surface_tags.insert(surface_tags.end(),object.lower.begin(),object.lower.end());
	surface_tags.insert(surface_tags.end(),object.sides.begin(),object.sides.end());
	gmsh::model::geo::addPhysicalGroup(2, surface_tags, -1);
}

void reindex_extruded_object(GeometryStorage &object, int point_start_idx, int line_start_index, int contour_start_idx, int surface_start_idx)
{
	for (int i = 0; i < object.points.size(); i ++)
	{
		object.points[i].idx = object.points[i].idx + point_start_idx - 1;
	}

	for (int i = 0; i < object.lines.size(); i ++)
	{
		object.lines[i].endpoints[0] = object.lines[i].endpoints[0] + point_start_idx - 1;
		object.lines[i].endpoints[1] = object.lines[i].endpoints[1] + point_start_idx - 1;
		object.lines[i].idx = object.lines[i].idx + line_start_index - 1;
	}

	for (int i = 0; i < object.contours.size(); i ++)
	{
		for (int j = 0; j < object.contours[i].lines.size(); j ++)
		{
			if (object.contours[i].lines[j] < 0)
			{
				object.contours[i].lines[j] = object.contours[i].lines[j] - line_start_index + 1;
			}else
			{
				object.contours[i].lines[j] = object.contours[i].lines[j] + line_start_index - 1;
			}
		}
		object.contours[i].idx = object.contours[i].idx + contour_start_idx - 1;
	}

	for (int i = 0; i < object.surfaces.size(); i ++)
	{
		for (int j = 0; j < object.surfaces[i].contours.size(); j ++)
		{
			if (object.surfaces[i].contours[j] < 0)
			{
				object.surfaces[i].contours[j] = object.surfaces[i].contours[j] - contour_start_idx + 1;
			}else
			{
				object.surfaces[i].contours[j] = object.surfaces[i].contours[j] + contour_start_idx - 1;
			}
		}

		object.surfaces[i].idx = object.surfaces[i].idx + surface_start_idx - 1;
	}

	for (int i = 0; i < object.sides.size(); i ++)
	{
		object.sides[i] = surface_start_idx + object.sides[i] - 1;
	}

	for (int i = 0; i < object.lower.size(); i ++)
	{
		object.lower[i] = surface_start_idx + object.lower[i] - 1;
	}

	for (int i = 0; i < object.upper.size(); i ++)
	{
		object.upper[i] = surface_start_idx + object.upper[i] - 1;
	}
	for(auto& it:object.upper_map)
	{
		it.second = it.second + surface_start_idx - 1;
	}

	for(auto& it:object.lower_map)
	{
		it.second = it.second + surface_start_idx - 1;
	}

}
