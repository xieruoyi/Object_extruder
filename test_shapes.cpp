
#include "test_shapes.h"
#include <algorithm>
#include <array>
#include <map>
#include <math.h>
#define PI 3.14159265

std::pair<footprint, footprint> create_tmd_square()
{
	ClipperLib::IntPoint a = {-275609, 1246175};
	ClipperLib::IntPoint b = {-356927, 1327493};
	ClipperLib::IntPoint c = {-385565, 1298855};
	ClipperLib::IntPoint d = {-304247, 1217537};
	ClipperLib::Path square = {a,b,c,d};
	footprint upper;
	upper.resize(1);
	upper[0].resize(1);
	upper[0][0] = square;
	footprint lower;
	lower.resize(1);
	lower[0].resize(1);
	lower[0][0] = square;
	return {lower, upper};
}
std::pair<footprint, footprint> create_the_second_test()
{
	ClipperLib::IntPoint a = {-3, 1};
	ClipperLib::IntPoint b = {0, 1};
	ClipperLib::IntPoint e = {0, -1};
	ClipperLib::IntPoint f = {-3, -1};

	ClipperLib::IntPoint g = {0, 2};
	ClipperLib::IntPoint h = {3, 2};
	ClipperLib::IntPoint i = {3, -2};
	ClipperLib::IntPoint j = {0, -2};

	ClipperLib::Path square1 = {a, b, g, h, i, j, e, f};
	footprint upper;
	upper.resize(1);
	upper[0].resize(1);
	upper[0][0] = square1;

	footprint lower;
	lower = upper;
	return {lower,upper};
}
std::pair<footprint, footprint> create_sqaure_triangle()
{
	ClipperLib::IntPoint a = {-3, 6};
	ClipperLib::IntPoint b = {-5, 2};
	ClipperLib::IntPoint c = {-1, 2};
	ClipperLib::Path triangle = {a,b,c};

	ClipperLib::IntPoint one = {0, 0};
	ClipperLib::IntPoint two = {0, 6};
	ClipperLib::IntPoint three = {-6, 6};
	ClipperLib::IntPoint four = {-6, 0};
	ClipperLib::Path square = {one,two,a,three,four};

	footprint upper;
	upper.resize(1);
	upper[0].resize(2);
	upper[0][0] = square;
	upper[0][1] = triangle;

	footprint lower;
	lower = upper;

	return{lower,upper};

}
std::pair<footprint, footprint> create_the_first_object()
{
	ClipperLib::IntPoint one = {0, 0};
	ClipperLib::IntPoint two = {0, 5};
	ClipperLib::IntPoint three = {-5, 5};
	ClipperLib::IntPoint four = {-5, 0};
	ClipperLib::IntPoint five = {-1, 1};
	ClipperLib::IntPoint six = {-1, 4};
	ClipperLib::IntPoint seven = {-4, 4};
	ClipperLib::IntPoint eight = {-4, 1};
	ClipperLib::Path lower_square = {one, two, three, four};
	ClipperLib::Path lower_inner_square = {five, six, seven, eight};
	footprint lower;
	lower.resize(1);
	lower[0].resize(2);
	lower[0][0] = lower_square;
	lower[0][1] = lower_inner_square;

	ClipperLib::IntPoint one1 = {0, 0};
	ClipperLib::IntPoint two2 = {0, 2};
	ClipperLib::IntPoint three3 = {-1, 2};
	ClipperLib::IntPoint four4 = {-1, 1};
	ClipperLib::IntPoint five5 = {-2, 1};
	ClipperLib::IntPoint six6 = {-2, 0};
	ClipperLib::Path upper_corner = {one1, two2,three3, four4, five5,six6};


	//ClipperLib::IntPoint a = {-2, 0};
	//ClipperLib::IntPoint b = {-2, 1};
	ClipperLib::IntPoint c = {-3, 1};
	ClipperLib::IntPoint d = {-3, 0};
	ClipperLib::Path middle_square = {six6,five5,c,d};

//	ClipperLib::IntPoint e = {0, 2};
	ClipperLib::IntPoint f = {0, 3};
	ClipperLib::IntPoint g = {-1, 3};
//	ClipperLib::IntPoint h = {-1, 2};
	ClipperLib::Path right_square = {two2,f,g,three3};

	ClipperLib::IntPoint i = {0, 5};
	ClipperLib::IntPoint j = {-5, 5};
	ClipperLib::IntPoint k = {-5, 0};
	ClipperLib::IntPoint l = {-4, 1};
	ClipperLib::IntPoint m = {-4, 4};
	ClipperLib::IntPoint n = {-1, 4};
	ClipperLib::Path left_corner = {f,i,j,k,d,c,l,m,n,g};

	footprint upper;
	upper.resize(4);
	upper[0].resize(1);
	upper[1].resize(1);
	upper[2].resize(1);
	upper[3].resize(1);
	upper[0][0] = upper_corner;
	upper[1][0] = middle_square;
	upper[2][0] = right_square;
	upper[3][0] = left_corner;
	return {lower,upper};


}

std::pair<footprint, footprint> create_the_second_object()
{
	ClipperLib::IntPoint a = {3, 3};
	ClipperLib::IntPoint b = {3, -3};
	ClipperLib::IntPoint c = {-3, -3};
	ClipperLib::IntPoint d = {-3, 0};
	ClipperLib::IntPoint e = {-2, 0};
	ClipperLib::IntPoint f = {-2, -2};
	ClipperLib::IntPoint g = {2, -2};
	ClipperLib::IntPoint h = {2, 2};
	ClipperLib::IntPoint i = {0, 2};
	ClipperLib::IntPoint j = {0, 3};
	ClipperLib::Path right_corner = {a, b, c, d, e, f, g, h, i, j};

	ClipperLib::IntPoint k = {-3, 1};
	ClipperLib::IntPoint l = {-2, 1};
	ClipperLib::Path middle_square = {l, e, d, k};

	ClipperLib::IntPoint m = {-1, 3};
	ClipperLib::IntPoint n = {-1, 2};
	ClipperLib::Path upper_square = {m, j, i, n};

	ClipperLib::IntPoint o = {-2, 2};
	ClipperLib::IntPoint p = {-3, 3};
	ClipperLib::Path left_corner = {m, n, o, l, k, p};

	footprint upper;
	upper.resize(4);
	upper[0].resize(1);
	upper[1].resize(1);
	upper[2].resize(1);
	upper[3].resize(1);
	upper[0][0] = right_corner;
	upper[1][0] = middle_square;
	upper[2][0] = upper_square;
	upper[3][0] = left_corner;

	footprint lower;
	ClipperLib::Path lower_square = {a, b, c, d, k, p, m, j};
	ClipperLib::Path lower_inner_square = {i,n,o,l,e,f,g,h};
//		{h, g, f, e, l, o, n, i};
//	ClipperLib::Path lower_square = {a, b, c, p};
//	ClipperLib::Path lower_inner_square = {h, g, f, o};
	lower.resize(1);
	lower[0].resize(2);
	lower[0][0] = lower_square;
	lower[0][1] = lower_inner_square;
	return {lower, upper};
}



std::pair<footprint, footprint> create_triangle_footprints()
{
	ClipperLib::IntPoint two = {3, -3};
	ClipperLib::IntPoint three = {3, 3};
	ClipperLib::IntPoint four = {-3, 3};
	ClipperLib::Path triangle = { two, three, four};
	footprint lower;
	footprint upper;

	lower.resize(1);
	lower[0].resize(1);
	lower[0][0] = triangle;

	upper.resize(1);
	upper[0].resize(1);
	upper[0][0] = triangle;

	return {lower,upper};
}
std::pair<footprint, footprint> create_square_donut_footprints()
{
	//  Bottom surface
	ClipperLib::IntPoint one = {-3, -3};
	ClipperLib::IntPoint two = {3, -3};
	ClipperLib::IntPoint three = {3, 3};
	ClipperLib::IntPoint four = {-3, 3};
	ClipperLib::Path square = {one, two, three, four};
	//  hole surface
	ClipperLib::IntPoint a = {-2, -2};
	ClipperLib::IntPoint b = {2, -2};
	ClipperLib::IntPoint c = {2, 2};
	ClipperLib::IntPoint d = {-2, 2};
	ClipperLib::Path small_square = {d, c, b, a};

	ClipperLib::IntPoint e = {-3, -3};
	ClipperLib::IntPoint f = {3, -3};
	ClipperLib::IntPoint g = {3, 3};
	ClipperLib::IntPoint h = {-3, 3};
	ClipperLib::IntPoint i = {-3, 1};
	ClipperLib::IntPoint j = {-2, 1};
	ClipperLib::IntPoint k = {-2, 2};
	ClipperLib::IntPoint l = {2, 2};
	ClipperLib::IntPoint m = {2, -2};
	ClipperLib::IntPoint n = {-2, -2};
	ClipperLib::IntPoint o = {-2, -1};
	ClipperLib::IntPoint p = {-3, -1};
	ClipperLib::Path blue = {e, f, g, h, i, j, k, l, m, n, o, p};
	ClipperLib::Path green = {p, o, j, i};

	// assign paths to footprints

	footprint lower;
	footprint upper;

	lower.resize(1);
	lower[0].resize(2);
	lower[0][0] = square;
	lower[0][1] = small_square;

	upper.resize(2);
	upper[0].resize(1);
	upper[0][0] = blue;
	upper[1].resize(1);
	upper[1][0] = green;

	return {lower, upper};

//	int num = 30;
//	int radius = 2000;
//	int small_r = 1000;
//	int x;
//	int y;
//	ClipperLib::Path circle;
//	ClipperLib::Path inner_circle;
//	for(int i = 0; i < num; i++)
//	{
//		x = 0 + radius*cos(2*PI/num*i);
//		y = 0 + radius*sin(2*PI/num*i);
//		ClipperLib::IntPoint a = {x,y};
//		circle.push_back(a);
//	}
//
//	ClipperLib::IntPoint one = {-1,-1};
//	ClipperLib::IntPoint two = {-1,1};
//	ClipperLib::IntPoint three = {1,1};
//	ClipperLib::IntPoint four = {1,-1};
////	inner_circle = {one, two, three, four};
////	inner_circle = {four, three, two, one};
//	for(int i = num; i > 0; i--)
//	{
//		x = 0 + small_r*cos(2*PI/num*i);
//		y = 0 + small_r*sin(2*PI/num*i);
//		ClipperLib::IntPoint a = {x,y};
//		inner_circle.push_back(a);
//	}
//	footprint lower;
//	footprint upper;
//	lower.resize(1);
//	lower[0].resize(2);
//	lower[0][0] = circle;
//	lower[0][1] = inner_circle;
//	upper.resize(1);
//	upper[0].resize(2);
//	upper[0][0] = circle;
//	upper[0][1] = inner_circle;
//	return {lower,upper};



	//  Bottom surface
//	ClipperLib::IntPoint one = {-4, -4};
//	ClipperLib::IntPoint two = {4, -4};
//	ClipperLib::IntPoint three = {4, 4};
//	ClipperLib::IntPoint four = {-4, 4};
//	ClipperLib::IntPoint five = {-4, 0};
//	ClipperLib::Path square = {one, two, three, four,five};
//	//  long hole surface
//	ClipperLib::IntPoint a = {-3, -3};
//	ClipperLib::IntPoint b = {3, -3};
//	ClipperLib::IntPoint c = {3, -2};
//	ClipperLib::IntPoint d = {0, -2};
//	ClipperLib::IntPoint e = {-3, -2};
//	ClipperLib::Path long_square = {e, d, c, b, a};
//	// upper left hole
//	ClipperLib::IntPoint f = {-3, 1};
//	ClipperLib::IntPoint g = {-1, 1};
//	ClipperLib::IntPoint h = {-1, 3};
//	ClipperLib::IntPoint i = {-3, 3};
//	ClipperLib::Path left_square = {i, h, g, f};
//
//	//upper right hole
//	ClipperLib::IntPoint j = {2, 2};
//	ClipperLib::IntPoint k = {3, 2};
//	ClipperLib::IntPoint l = {3, 3};
//	ClipperLib::IntPoint m = {2, 3};
//	ClipperLib::Path right_square = {m, l, k, j};
//
//	//  upper surface
//	ClipperLib::IntPoint n = {-4, -4};
//	ClipperLib::IntPoint o = {4, -4};
//	ClipperLib::IntPoint p = {4, 3};
//	ClipperLib::IntPoint q = {4, 4};
//	ClipperLib::IntPoint r = {-4, 4};
//	ClipperLib::Path upper_square = {n, o ,p ,q, r};
//	//  long hole surface
//	ClipperLib::IntPoint s = {-3, -3};
//	ClipperLib::IntPoint t = {-2, -3};
//	ClipperLib::IntPoint u = {3, -3};
//	ClipperLib::IntPoint v = {3, -2};
//	ClipperLib::IntPoint w = {-3, -2};
//	ClipperLib::Path upper_long_square = {w, v, u, t, s};
//	// upper left hole
//	ClipperLib::IntPoint x = {-3, 1};
//	ClipperLib::IntPoint y = {-1, 1};
//	ClipperLib::IntPoint z = {-1, 3};
//	ClipperLib::IntPoint ab = {-3, 3};
//	ClipperLib::Path upper_left_square = {ab,z,y,x};
//
//	//upper right hole
//	ClipperLib::IntPoint ac = {2, 2};
//	ClipperLib::IntPoint ad = {3, 2};
//	ClipperLib::IntPoint ae = {3, 3};
//	ClipperLib::IntPoint af = {2, 3};
//	ClipperLib::Path upper_right_square = {af,ae,ad,ac};
//
//	// assign paths to footprints
//
//	footprint lower;
//	footprint upper;
//
//	lower.resize(1);
//	lower[0].resize(4);
//	lower[0][0] = square;
//	lower[0][1] = long_square;
//	lower[0][2] = left_square;
//	lower[0][3] = right_square;
//
//	upper.resize(1);
//	upper[0].resize(4);
//	upper[0][0] = upper_right_square;
//	upper[0][1] = upper_long_square;
//	upper[0][2] = upper_left_square;
//	upper[0][3] = upper_square;
//	return {lower, upper};
//
//	ClipperLib::IntPoint one = {-40, -40};
//	ClipperLib::IntPoint two = {40, -40};
//	ClipperLib::IntPoint three = {40, 40};
//	ClipperLib::IntPoint four = {-40, 40};
//	ClipperLib::Path square = {one, two, three, four};
//
//	int num = 49;
//	int radius = 24;
//	int left_r = 19;
//	int x;
//	int y;
//	ClipperLib::Path circle;
//	for(int i = 0; i < num; i++)
//	{
//		x = 15 + radius*cos(2*PI/num*i);
//		y = -12 + radius*sin(2*PI/num*i);
//		ClipperLib::IntPoint a = {x,y};
//		circle.push_back(a);
//	}
//	ClipperLib::Path left_circle;
//	for(int i = 0; i < num; i++)
//	{
//		x = -20 + left_r*cos(2*PI/num*i);
//		y = 20 + left_r*sin(2*PI/num*i);
//		ClipperLib::IntPoint b = {x,y};
//		left_circle.push_back(b);
//	}
//
//		footprint lower;
//		footprint upper;
//
//		lower.resize(1);
//		lower[0].resize(3);
//		lower[0][0] = square;
//		lower[0][1] = circle;
//		lower[0][2] = left_circle;
//
//		upper.resize(1);
//		upper[0].resize(3);
//		upper[0][0] = square;
//		upper[0][1] = circle;
//		upper[0][2] = left_circle;
//		return {lower, upper};
//
}
GeometryStorage create_extruded_square_donut()
{
	double z_bottom = 0.0;
	double z_top = 4.0;

	std::pair<footprint, footprint> lower_upper = create_square_donut_footprints();
	footprint lower = lower_upper.first;
	footprint upper = lower_upper.second;

	GeometryStorage square_donut;
	GeometryStorage &geom = square_donut;

	double lc = 1;
	// Add Blue Path
	int pIndex = 1;
	int prev_p = pIndex;

	geom.points.resize(upper[0][0].size() + upper[1][0].size() + lower[0][0].size() +
					   lower[0][1].size()-4);
	for (int i = 0; i < upper[0][0].size(); i++)
	{
		//		double x = upper[0][0][i].X;
		//		double y = upper[0][0][i].Y;
		// gmsh::model::geo::addPoint(x,y,z_top,lc,pIndex);
		geom.points[i].coords[0] = upper[0][0][i].X;
		geom.points[i].coords[1] = upper[0][0][i].Y;
		geom.points[i].coords[2] = z_top;
		geom.points[i].idx = pIndex;
		pIndex++;
	}

	int lIndex = 1;
	geom.lines.resize(upper[0][0].size() + upper[1][0].size() + lower[0][0].size() +lower[0][1].size()-2);
	for (int j = 0; j < pIndex - 2; j++)
	{
		geom.lines[j].endpoints[0] = geom.points[j].idx;
		geom.lines[j].endpoints[1] = geom.points[j + 1].idx;
		geom.lines[j].idx = lIndex;
		lIndex++;
	}
	geom.lines[pIndex - 2].endpoints[0] = geom.points[pIndex - 2].idx;
	geom.lines[pIndex - 2].endpoints[1] = geom.points[prev_p - 1].idx;
	geom.lines[pIndex - 2].idx = lIndex;
	prev_p = pIndex;

	int sIndex = 1;
	std::vector<int> curveTags(lIndex);
	geom.contours.resize(4); // Count it manually
	for (int s = 1; s < lIndex + 1; s++)
	{
		curveTags[s - 1] = s;
	}
	geom.contours[sIndex - 1].lines = curveTags;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	int vIndex = 1;
	geom.surfaces.resize(3);
	for (int v = vIndex - 1; v < sIndex - 1; v++)
	{
		//		gmsh::model::geo::addPlaneSurface({v}, v);
		geom.surfaces[v].idx = vIndex;
		geom.surfaces[v].contours = {vIndex};
		vIndex++;
	}
	int prev_l = lIndex;
	lIndex = lIndex + 1;
	geom.lines[lIndex - 1].endpoints[0] = 12;
	geom.lines[lIndex - 1].endpoints[1] = 5;
	geom.lines[lIndex - 1].idx = lIndex;
	lIndex++;
	geom.lines[lIndex - 1].endpoints[0] = 6;
	geom.lines[lIndex - 1].endpoints[1] = 11;
	geom.lines[lIndex - 1].idx = lIndex;
	lIndex++;
	prev_p = pIndex;

	std::vector<int> curve2(lIndex - prev_l + 1);
	curve2[0] = 13;
	curve2[1] = 5;
	curve2[2] = 14;
	curve2[3] = 11;

	geom.contours[sIndex - 1].lines = curve2;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	for (int v = vIndex - 1; v < sIndex - 1; v++)
	{
		geom.surfaces[v].idx = vIndex;
		geom.surfaces[v].contours = {-vIndex};
		vIndex++;
	}

	// Add Bottom surface
	for (int i = prev_p - 1; i < prev_p + lower[0][0].size() - 1; i++)
	{
		geom.points[i].coords[0] = lower[0][0][i - prev_p + 1].X;
		geom.points[i].coords[1] = lower[0][0][i - prev_p + 1].Y;
		geom.points[i].coords[2] = z_bottom;
		geom.points[i].idx = pIndex;
		pIndex++;
	}

	prev_l = lIndex;
	lIndex = lIndex + 1;
	for (int j = prev_p + 2; j < pIndex + 1; j++)
	{
		geom.lines[j - 1].endpoints[0] = geom.points[j - 3].idx;
		geom.lines[j - 1].endpoints[1] = geom.points[j - 2].idx;
		geom.lines[j - 1].idx = lIndex - 1;
		lIndex++;
	}
	geom.lines[pIndex].endpoints[0] = 16;
	geom.lines[pIndex].endpoints[1] = 13;
	geom.lines[pIndex].idx = lIndex - 1;
	prev_p = pIndex;

	std::vector<int> curve3(lIndex - prev_l);
	for (int s = 1; s < lIndex - prev_l + 1; s++)
	{
		curve3[s - 1] = prev_l + s - 1;
	}
	geom.contours[sIndex - 1].lines = curve3;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	// Add small bottom surface
	for (int i = prev_p - 1; i < prev_p + lower[0][1].size() - 1; i++)
	{
		geom.points[i].coords[0] = lower[0][1][i - prev_p + 1].X;
		geom.points[i].coords[1] = lower[0][1][i - prev_p + 1].Y;
		geom.points[i].coords[2] = z_bottom;
		geom.points[i].idx = pIndex;
		pIndex++;
	}
	prev_l = lIndex;
	lIndex = lIndex + 1;
	for (int j = prev_p + 1; j < pIndex; j++)
	{
		geom.lines[j].endpoints[0] = geom.points[j - 2].idx;
		geom.lines[j].endpoints[1] = geom.points[j - 1].idx;
		geom.lines[j].idx = lIndex - 1;
		lIndex++;
	}
	geom.lines[21].endpoints[0] = 20;
	geom.lines[21].endpoints[1] = 17;
	geom.lines[21].idx = 22;
	prev_p = pIndex;
	std::vector<int> curve4(lIndex - prev_l);
	for (int s = 1; s < lIndex - prev_l + 1; s++)
	{
		curve4[s - 1] = prev_l + s - 1;
	}
	geom.contours[sIndex - 1].lines = curve4;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	geom.surfaces[2].idx = 3;
	geom.surfaces[2].contours = {-3, -4};

	// Outer side surfaces
	geom.lines.resize(30);
	geom.lines[22].endpoints[0] =1; //geom.points[0].idx;
	geom.lines[22].endpoints[1] =13;// geom.points[12].idx;
	geom.lines[22].idx = 23;

	geom.lines[23].endpoints[0] = geom.points[1].idx;
	geom.lines[23].endpoints[1] = geom.points[13].idx;
	geom.lines[23].idx = 24;

	geom.lines[24].endpoints[0] = geom.points[2].idx;
	geom.lines[24].endpoints[1] = geom.points[14].idx;
	geom.lines[24].idx = 25;

	geom.lines[25].endpoints[0] = geom.points[3].idx;
	geom.lines[25].endpoints[1] = geom.points[15].idx;
	geom.lines[25].idx = 26;

	std::vector<int> curve5(4);
	curve5[0] = 1;
	curve5[1] = 24;
	curve5[2] = -15;
	curve5[3] = -23;
	geom.contours.resize(12);
	geom.contours[sIndex - 1].lines = curve5;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	geom.surfaces.resize(11);
	geom.surfaces[3].idx = 4;
	geom.surfaces[3].contours = {-5};
	vIndex++;

	std::vector<int> curve6(6);
	curve6[0] = -26;
	curve6[1] = 4;
	curve6[2] = -13;
	curve6[3] = 12;
	curve6[4] = 23;
	curve6[5] = -18;
	geom.contours[sIndex - 1].lines = curve6;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	geom.surfaces[4].idx = 5;
	geom.surfaces[4].contours = {-6};
	vIndex++;

	std::vector<int> curve7(4);
	curve7[0] = -24;
	curve7[1] = 2;
	curve7[2] = 25;
	curve7[3] = -16;
	geom.contours[sIndex - 1].lines = curve7;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	geom.surfaces[5].idx = 6;
	geom.surfaces[5].contours = {-7};
	vIndex++;

	std::vector<int> curve8(4);
	curve8[0] = -25;
	curve8[1] = 3;
	curve8[2] = 26;
	curve8[3] = -17;
	geom.contours[sIndex - 1].lines = curve8;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	geom.surfaces[6].idx = 7;
	geom.surfaces[6].contours = {-8};
	//	//  Inner side surfaces

	geom.lines[26].endpoints[0] = geom.points[6].idx;
	geom.lines[26].endpoints[1] = geom.points[16].idx;
	geom.lines[26].idx = 27;

	geom.lines[27].endpoints[0] = geom.points[7].idx;
	geom.lines[27].endpoints[1] = geom.points[17].idx;
	geom.lines[27].idx = 28;

	geom.lines[28].endpoints[0] = geom.points[8].idx;
	geom.lines[28].endpoints[1] = geom.points[18].idx;
	geom.lines[28].idx = 29;

	geom.lines[29].endpoints[0] = geom.points[9].idx;
	geom.lines[29].endpoints[1] = geom.points[19].idx;
	geom.lines[29].idx = 30;

	std::vector<int> curve9(4);
	curve9[0] = -27;
	curve9[1] = 7;
	curve9[2] = 28;
	curve9[3] = -19;
	geom.contours[sIndex - 1].lines = curve9;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	geom.surfaces[7].idx = 8;
	geom.surfaces[7].contours = {-9};
	vIndex++;

	std::vector<int> curve10(6);
	curve10[0] = 10;
	curve10[1] = -14;
	curve10[2] = 6;
	curve10[3] = 27;
	curve10[4] = -22;
	curve10[5] = -30;
	geom.contours[sIndex - 1].lines = curve10;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	geom.surfaces[8].idx = 9;
	geom.surfaces[8].contours = {-10};
	vIndex++;

	std::vector<int> curve11(4);
	curve11[0] = -28;
	curve11[1] = 8;
	curve11[2] = 29;
	curve11[3] = -20;
	geom.contours[sIndex - 1].lines = curve11;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	geom.surfaces[9].idx = 10;
	geom.surfaces[9].contours = {-11};
	vIndex++;

	std::vector<int> curve12(4);
	curve12[0] = -29;
	curve12[1] = 9;
	curve12[2] = 30;
	curve12[3] = -21;
	geom.contours[sIndex - 1].lines = curve12;
	geom.contours[sIndex - 1].idx = sIndex;
	sIndex = sIndex + 1;

	geom.surfaces[10].idx = 11;
	geom.surfaces[10].contours = {-12};

	return square_donut;
}

std::pair<footprint, footprint> create_four_square_footprints()
{
	//  Bottom surface
	ClipperLib::IntPoint one = {-4, -4};
	ClipperLib::IntPoint two = {4, -4};
	ClipperLib::IntPoint three = {4, 4};
	ClipperLib::IntPoint four = {-4, 4};
	ClipperLib::IntPoint five = {-4, 0};
	ClipperLib::Path square = {one, two, three, four,five};
	//  long hole surface
	ClipperLib::IntPoint a = {-3, -3};
	ClipperLib::IntPoint b = {3, -3};
	ClipperLib::IntPoint c = {3, -2};
	ClipperLib::IntPoint d = {0, -2};
	ClipperLib::IntPoint e = {-3, -2};
	ClipperLib::Path long_square = {e, d, c, b, a};
	// upper left hole
	ClipperLib::IntPoint f = {-3, 1};
	ClipperLib::IntPoint g = {-1, 1};
	ClipperLib::IntPoint h = {-1, 3};
	ClipperLib::IntPoint i = {-3, 3};
	ClipperLib::Path left_square = {i, h, g, f};

	//upper right hole
	ClipperLib::IntPoint j = {2, 2};
	ClipperLib::IntPoint k = {3, 2};
	ClipperLib::IntPoint l = {3, 3};
	ClipperLib::IntPoint m = {2, 3};
	ClipperLib::Path right_square = {m, l, k, j};

	//  upper surface
	ClipperLib::IntPoint n = {-4, -4};
	ClipperLib::IntPoint o = {4, -4};
	ClipperLib::IntPoint p = {4, 3};
	ClipperLib::IntPoint q = {4, 4};
	ClipperLib::IntPoint r = {-4, 4};
	ClipperLib::Path upper_square = {n, o ,p ,q, r};
	//  long hole surface
	ClipperLib::IntPoint s = {-3, -3};
	ClipperLib::IntPoint t = {-2, -3};
	ClipperLib::IntPoint u = {3, -3};
	ClipperLib::IntPoint v = {3, -2};
	ClipperLib::IntPoint w = {-3, -2};
	ClipperLib::Path upper_long_square = {w, v, u, t, s};
	// upper left hole
	ClipperLib::IntPoint x = {-3, 1};
	ClipperLib::IntPoint y = {-1, 1};
	ClipperLib::IntPoint z = {-1, 3};
	ClipperLib::IntPoint ab = {-3, 3};
	ClipperLib::Path upper_left_square = {ab,z,y,x};

	//upper right hole
	ClipperLib::IntPoint ac = {2, 2};
	ClipperLib::IntPoint ad = {3, 2};
	ClipperLib::IntPoint ae = {3, 3};
	ClipperLib::IntPoint af = {2, 3};
	ClipperLib::Path upper_right_square = {af,ae,ad,ac};

	// assign paths to footprints

	footprint lower;
	footprint upper;

	lower.resize(1);
	lower[0].resize(4);
	lower[0][0] = square;
	lower[0][1] = long_square;
	lower[0][2] = left_square;
	lower[0][3] = right_square;

	upper.resize(1);
	upper[0].resize(4);
	upper[0][0] = upper_square;
	upper[0][1] = upper_long_square;
	upper[0][2] = upper_left_square;
	upper[0][3] = upper_right_square;
	return {lower, upper};
}

std::pair<footprint, footprint> create_cylinder_footprints()
{
	int num = 30;
	int radius = 30;
	int x;
	int y;
	ClipperLib::Path circle;
	for(int i = 0; i < num; i++)
	{
		x = 0 + radius*cos(2*PI/num*i);
		y = 0 + radius*sin(2*PI/num*i);
		ClipperLib::IntPoint a = {x,y};
		circle.push_back(a);
	}
	footprint lower;
	footprint upper;
	lower.resize(1);
	lower[0].resize(1);
	lower[0][0] = circle;
	upper.resize(1);
	upper[0].resize(1);
	upper[0][0] = circle;
	return{upper,lower};

}
std::pair<footprint, footprint> create_washer_footprints()
{
	int num = 30;
	int radius = 100;
	int small_r = 50;
	int x;
	int y;
	ClipperLib::Path circle;
	ClipperLib::Path inner_circle;
	for (int i = 0; i < num; i++)
	{
		x = 0 + radius * cos(2 * PI / num * i);
		y = 0 + radius * sin(2 * PI / num * i);
		ClipperLib::IntPoint a = {x, y};
		circle.push_back(a);
	}

	ClipperLib::IntPoint one = {-1, -1};
	ClipperLib::IntPoint two = {-1, 1};
	ClipperLib::IntPoint three = {1, 1};
	ClipperLib::IntPoint four = {1, -1};
	//	inner_circle = {one, two, three, four};
	//	inner_circle = {four, three, two, one};
	for (int i = 0; i < num; i++)
	{
		x = 0 + small_r * cos(2 * PI / num * i);
		y = 0 + small_r * sin(2 * PI / num * i);
		ClipperLib::IntPoint a = {x, y};
		inner_circle.push_back(a);
	}
	footprint lower;
	footprint upper;
	lower.resize(1);
	lower[0].resize(1);
	lower[0][0] = circle;
//	lower[0][1] = inner_circle;
	upper.resize(2);
	upper[0].resize(1);
	upper[1].resize(1);
	upper[0][0] = circle;
	upper[1][0] = inner_circle;
//	lower.resize(1);
//	lower[0].resize(1);
//	lower[0][0] = circle;
//	//	lower[0][1] = inner_circle;
//	upper.resize(2);
//	upper[0].resize(1);
//	upper[1].resize(1);
//	upper[0][0] = circle;
//	upper[1][0] = inner_circle;
	return {lower, upper};
}

std::pair<footprint, footprint> create_case_1()
{
	int int_scale = 100;

	//  Small Square for both upper and lower footprints
	std::pair<footprint, footprint> lower_upper_square;
	lower_upper_square.first.resize(1);
	lower_upper_square.second.resize(1);
	ClipperLib::IntPoint one = {-3, -3};
	ClipperLib::IntPoint two = {3, -3};
	ClipperLib::IntPoint three = {3, 3};
	ClipperLib::IntPoint four = {-3, 3};
	ClipperLib::Path square = {one, two, three, four};

//	ClipperLib::IntPoint a = {-1, -1};
//	ClipperLib::IntPoint b = {1, -1};
//	ClipperLib::IntPoint c = {1, 1};
//	ClipperLib::IntPoint d = {-1, 1};
//	ClipperLib::Path small_square = {d, c, b, a};

	lower_upper_square.first[0].push_back(square);
//	lower_upper_square.first[0].push_back(small_square);
	lower_upper_square.second = lower_upper_square.first;

	scale_footprint(lower_upper_square.first, int_scale);
	scale_footprint(lower_upper_square.second, int_scale);

	footprint upper;
	// footprint lower = upper_lower_square.second;

	// Convert footprint to Clipper Paths
	ClipperLib::Paths path_subj;
	for (auto &low: lower_upper_square.first)
	{
		path_subj.insert(path_subj.cend(), low.cbegin(), low.cend());
	}

	// Creates a rotated square to act as the conforming object mimicking an object in an adjacent layer.
	int num = 5;
	int radius = 3 * int_scale;
	ClipperLib::Paths diamond(1);
	for (int i = 0; i < num; i++)
	{
		int x = 0.0 + radius * cos(2 * PI / num * i);
		int y = 0.0 + radius * sin(2 * PI / num * i);
		diamond[0].emplace_back(ClipperLib::IntPoint{x, y});
	}
	//Modify the lower footprint with the conforming operation
	footprint lower = conform_polygons(path_subj, diamond);

//	ClipperLib::SimplifyPolygons(lower,)
	//Final shape should be a rectangular prism, but the lower footprint has a diamond imprinted on it.
	return {lower, upper};
}
ClipperLib::Path create_circle(int x0, int y0, int num, int radius)
{
	ClipperLib::Path circle;

	for (int i = 0; i < num; i++)
	{
		int x = x0 + radius * cos(2 * PI / num * i);
		int y = y0 + radius * sin(2 * PI / num * i);
		circle.emplace_back(ClipperLib::IntPoint{x, y});
	}
	return circle;
}

std::pair<footprint, footprint> create_case_2()
{
	ClipperLib::IntPoint one = {-30, -30};
	ClipperLib::IntPoint two = {30, -30};
	ClipperLib::IntPoint three = {30, 30};
	ClipperLib::IntPoint four = {-30, 30};
	ClipperLib::Path square = {one, two, three, four};
	//  hole surface
	ClipperLib::IntPoint a = {-20, -20};
	ClipperLib::IntPoint b = {20, -20};
	ClipperLib::IntPoint c = {20, 20};
	ClipperLib::IntPoint d = {-20, 20};
	ClipperLib::Path small_square = {d, c, b, a};
	std::pair<footprint, footprint> lower_upper_square;
	lower_upper_square.first.resize(1);
	lower_upper_square.second.resize(1);
	lower_upper_square.first[0].push_back(square);
	lower_upper_square.first[0].push_back(small_square);
	lower_upper_square.second = lower_upper_square.first;
	footprint upper = lower_upper_square.second;
	ClipperLib::Paths path_subj;
	for (auto &low: lower_upper_square.first)
	{
		path_subj.insert(path_subj.cend(), low.cbegin(), low.cend());
	}
	int num = 20;
	int radius = 15;
	ClipperLib::Paths diamond(2);
	//	for (int i = 0; i < num; i++)
	//	{
	//		int x = -20 + radius * cos(2 * PI / num * i);
	//		int y = -20 + radius * sin(2 * PI / num * i);
	//		diamond[0].emplace_back(ClipperLib::IntPoint{x, y});
	//	}
	diamond[0] = create_circle(-20,-20,num,radius);
	diamond[1] = create_circle(20,20,num,radius);
	//
	//	for (int i = 0; i < num; i++)
	//	{
	//		int x = 20 + radius * cos(2 * PI / num * i);
	//		int y = 20 + radius * sin(2 * PI / num * i);
	//		diamond[1].emplace_back(ClipperLib::IntPoint{x, y});
	//	}

	footprint lower = conform_polygons(path_subj, diamond);

	//diamond.resize(4);

//	ClipperLib::Paths diamond(2);
	double big_radius = 25;
	for (int i = 0; i < num; i++)
	{
		int x = -20 + big_radius * cos(2 * PI / num * i);
		int y = 20 + big_radius * sin(2 * PI / num * i);
		diamond[0].emplace_back(ClipperLib::IntPoint{x, y});
	}

	for (int i = 0; i < num; i++)
	{
		int x = 20 + big_radius * cos(2 * PI / num * i);
		int y = -20 + big_radius * sin(2 * PI / num * i);
		diamond[1].emplace_back(ClipperLib::IntPoint{x, y});
	}

	upper = conform_polygons(path_subj, diamond);
	return {lower, upper};
}

ClipperLib::Paths convert_footprint(const footprint &footprint) {
	// Convert footprint to Clipper Paths through a flattening of the data structure
	ClipperLib::Paths paths;
	for (auto &surface: footprint)
	{
		paths.insert(paths.cend(), surface.cbegin(), surface.cend());
	}
	return paths;
}

std::pair<footprint, footprint> create_2x_washer_footprints()
{
	int num = 30;
	int radius = 100;
	int small_r = 10;
	int x = 0;
	int y = 0;
	ClipperLib::Path circle = create_circle(x,y, num, radius);

	ClipperLib::Path inner_1 = create_circle( 50,0,10, small_r);
	ClipperLib::Path inner_2 = create_circle(-50,0,10, small_r);
	ClipperLib::Path inner_3= create_circle(-50,0,10, 7*small_r);

	footprint both;
	both.resize(1);
	both[0].resize(1);
	both[0][0] = circle;

	auto both_paths = convert_footprint(both);

	footprint lower = conform_polygons(both_paths, {inner_1, inner_3});
	footprint upper = conform_polygons(both_paths, {inner_1, inner_2});
	return {lower, upper};
}

//std::pair<footprint, footprint> create_square_and_numerous_circles()
//{
//	ClipperLib::IntPoint one = {-800, -800};
//	ClipperLib::IntPoint two = {800, -800};
//	ClipperLib::IntPoint three = {800, 800};
//	ClipperLib::IntPoint four = {-800, 800};
//	ClipperLib::Path square = {one, two, three, four};
//	std::pair<footprint, footprint> lower_upper_square;
//	lower_upper_square.first.resize(1);
//	lower_upper_square.second.resize(1);
//	lower_upper_square.first[0].push_back(square);
//	lower_upper_square.second[0].push_back(square);
//	footprint upper = lower_upper_square.second;
//	footprint lower = lower_upper_square.first;
//	int num = 36;
//	int radius = 30;
//
////	ClipperLib::Paths diagonal(8);
////	for (int i = 0; i < 8; i ++)
////	{
////		ClipperLib::IntPoint diagonal1 = {-800 + 400 * i, 800};
////		ClipperLib::IntPoint diagonal2 = {-600 + 400 * i, 800};
////		ClipperLib::IntPoint diagonal3 = {-2400 + 400 * i, -800};
////		ClipperLib::IntPoint diagonal4 = {-2200 + 400 * i, -800};
////		diagonal[i] = {diagonal1, diagonal3, diagonal4, diagonal2};
////	}
//	auto path_subj = convert_footprint(lower_upper_square.first);
//	ClipperLib::Paths circles(200);
//	for (int i = 0; i < 10; i ++)
//	{
//		int y_coord = 760 - i * 160;
//		for (int j = 0; j < 20; j ++)
//		{
//			int x_coord = -760 + 80 * j;
//			circles[j + i*20] = create_circle(x_coord,y_coord, num, radius);
//		}
//	}
//
//	lower = conform_polygons(path_subj, circles);
////	upper = conform_polygons(path_subj, diagonal);
//
//	for (int i = 0; i < 10; i ++)
//	{
//		int y_coord = 680 - i * 160;
//		for (int j = 0; j < 19; j ++)
//		{
//			int x_coord = -720 + 80 * j;
//			ClipperLib::Path circle = create_circle(x_coord,y_coord, num, radius);
//			upper[0].push_back(circle);
//			lower[0].push_back(circle);
//		}
//	}
////	auto path_subj2 = convert_footprint(upper);
////	upper = conform_polygons(path_subj, diagonal);
//	return {lower,upper};
//
//}


std::pair<footprint, footprint> create_square_and_14_circles()
{
	//Making squares could be a function ...
	ClipperLib::IntPoint one = {-100, -100};
	ClipperLib::IntPoint two = {100, -100};
	ClipperLib::IntPoint three = {100, 100};
	ClipperLib::IntPoint four = {-100, 100};
	ClipperLib::Path square = {one, two, three, four};
	//One surface with many holes
	footprint initial_shape(1);
	//Outline of footprint
	initial_shape[0].push_back(square);
	//All Circle properties
	int num = 18;
	int radius = 10;
	//Add holes to initial shape
	for (int i = 0; i < 1; i ++)
	{
		int y_coord = 72 - i * 96;
		for (int j = 0; j < 1; j ++)
		{
			int x_coord = -72 + 48 * (j);
			ClipperLib::Path circle = create_circle(x_coord,y_coord, num, radius);
			//Reverse for hole
			ClipperLib::ReversePath(circle);
			initial_shape[0].push_back(circle);
		}
	}
	int nDiagonals = 1;
	ClipperLib::Paths diagonal(nDiagonals);
	for (int i = 0; i < nDiagonals; i ++)
	{
		ClipperLib::IntPoint diagonal1 = {-80 + 100 * i, 100};
		ClipperLib::IntPoint diagonal2 = {0 + 100 * i, 100};
		ClipperLib::IntPoint diagonal3 = {-80 + 100 * i, -100};
		ClipperLib::IntPoint diagonal4 = {-160 + 100 * i, -100};
		diagonal[i] = {diagonal1, diagonal2, diagonal3, diagonal4};
	}
	ClipperLib::Paths circles(6);
	for (int i = 0; i < 2; i ++)
	{
		int y_coord = 24 - i * 96;
		for (int j = 0; j < 3; j ++)
		{
			int x_coord = -48 + 48 * j;
			circles[j + i*3] = create_circle(x_coord,y_coord, num, radius);
		}
	}
	//Use shapes to intersect and difference for extra constraints
	auto path_subj = convert_footprint(initial_shape);
	footprint lower = conform_polygons(path_subj, circles);
	footprint upper = conform_polygons(path_subj, diagonal);
	lower = initial_shape;
//	footprint upper = initial_shape;
	return {lower,upper};
}

std::pair<footprint, footprint> create_square_and_numerous_circles()
{
	//Making squares could be a function ...
	ClipperLib::IntPoint one = {-800, -800};
	ClipperLib::IntPoint two = {800, -800};
	ClipperLib::IntPoint three = {800, 800};
	ClipperLib::IntPoint four = {-800, 800};
	ClipperLib::Path square = {one, two, three, four};
	//One surface with many holes
	footprint initial_shape(1);
	//Outline of footprint
	initial_shape[0].push_back(square);
	//All Circle properties
	int num = 14;
	int radius = 30;
	//Add holes to initial shape
	for (int i = 0; i < 10; i ++)
	{
		int y_coord = 680 - i * 160;
		for (int j = 0; j < 19; j ++)
		{
			int x_coord = -720 + 80 * j;
			ClipperLib::Path circle = create_circle(x_coord,y_coord, num, radius);
			//Reverse for hole
			ClipperLib::ReversePath(circle);
			initial_shape[0].push_back(circle);
		}
	}
	int nDiagonals = 1;
	ClipperLib::Paths diagonal(nDiagonals);
	for (int i = 0; i < nDiagonals; i ++)
	{
		ClipperLib::IntPoint diagonal1 = {-800 + 400 * i, 800};
		ClipperLib::IntPoint diagonal2 = {-600 + 400 * i, 800};
		ClipperLib::IntPoint diagonal3 = {-2400 + 400 * i, -800};
		ClipperLib::IntPoint diagonal4 = {-2200 + 400 * i, -800};
		diagonal[i] = {diagonal1, diagonal3, diagonal4, diagonal2};
	}
	ClipperLib::Paths circles(200);
	for (int i = 0; i < 10; i ++)
	{
		int y_coord = 760 - i * 160;
		for (int j = 0; j < 20; j ++)
		{
			int x_coord = -760 + 80 * j;
			circles[j + i*20] = create_circle(x_coord,y_coord, num, radius);
		}
	}
	//Use shapes to intersect and difference for extra constraints
	auto path_subj = convert_footprint(initial_shape);
	footprint lower = conform_polygons(path_subj, circles);
	footprint upper = conform_polygons(path_subj, diagonal);
	//lower = initial_shape;
//	upper = initial_shape;
	return {lower,upper};
}

std::vector<ClipperLib::Paths> create_gmsh_polygons(const ClipperLib::PolyTree &polytree)
{
	using namespace ClipperLib;

	// GMSH likes 1 contour exterior, and all
	// interior contours, so need to semi-flatten
	// tree structure
	std::vector<Paths> gmsh_pgons;

	PolyNode *node = polytree.GetFirst();
	std::map<PolyNode *, size_t> node_to_index;
	while (node)
	{
		PolyNode *curnode = node;
		if (curnode->IsHole())
		{
			// add to parent contours as a hole
			PolyNode *parent = node->Parent;
			ReversePath(node->Contour);
			gmsh_pgons[node_to_index[parent]].push_back(node->Contour);
		}
		else
		{
			// create a new exterior contour
			gmsh_pgons.push_back({node->Contour});
			node_to_index[node] = gmsh_pgons.size() - 1;
		}
		node = node->GetNext();
	}

	return gmsh_pgons;
}

std::vector<ClipperLib::Paths> conform_polygons(const ClipperLib::Paths &path_subj,
												const ClipperLib::Paths &path_conform)
{
	ClipperLib::Clipper clipper{};

	clipper.StrictlySimple(true);
	clipper.PreserveCollinear(true);
	clipper.AddPaths(path_subj, ClipperLib::ptSubject, true);
	clipper.AddPaths(path_conform, ClipperLib::ptClip, true);


	std::array<ClipperLib::PolyTree, 2> solns;
	clipper.Execute(ClipperLib::ctDifference, solns[0], ClipperLib::pftNonZero,
					ClipperLib::pftNonZero);
	clipper.Execute(ClipperLib::ctIntersection, solns[1], ClipperLib::pftNonZero,
					ClipperLib::pftNonZero);

	std::vector<ClipperLib::Paths> result = create_gmsh_polygons(solns[0]);
	std::vector<ClipperLib::Paths> result2 = create_gmsh_polygons(solns[1]);
	result.insert(result.cend(), result2.cbegin(), result2.cend());
	return result;
}
void scale_footprint(footprint &footprints, int scale)
{
	for (auto &paths : footprints)
	{
		for (auto &path : paths)
		{
			for (auto &point : path)
			{
				point.X *= scale;
				point.Y *= scale;
			}
		}
	}
}

bool test_repeated_footprint(footprint test)
{
	std::unordered_set<int> dx;
	std::unordered_set<int> dy;
	std::unordered_set< std::pair<int, int>,  pair_hash> x_set;
	std::unordered_set< std::pair<int, int>,  pair_hash> y_set;
	for(int i = 0; i < test.size(); i++)
	{
		for(int j = 0; j < test[i].size(); j++)
		{
			for(int k = 0; k < test[i][j].size()-1; k++)
			{
				Point a;
				a.coords[0] = test[i][j][k].X;
				a.coords[1] = test[i][j][k].Y;

				Point b;
				b.coords[0] = test[i][j][k+1].X;
				b.coords[1] = test[i][j][k+1].Y;

				auto insert1 = x_set.insert(std::unordered_set< std::pair<int, int>,  pair_hash>::value_type(a.coords[0],b.coords[0]));
				auto insert2 = y_set.insert(std::unordered_set< std::pair<int, int>,  pair_hash>::value_type(a.coords[1],b.coords[1]));

				if((insert2.first->first > insert1.first->first and insert2.first->second > insert1.first->first and insert2.first->first < insert1.first->second and insert2.first->first < insert1.first->second) or
					(insert2.first->first < insert1.first->first and insert2.first->second < insert1.first->first and insert2.first->first > insert1.first->second and insert2.first->first > insert1.first->second))
				{
					return false;
				}
			}
		}
	}
	return true;
}

