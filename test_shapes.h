//
// Created by HP on 2022/5/18.
//
#ifndef MANUALEXTRUDER_TEST_SHAPE_H
#define MANUALEXTRUDER_TEST_SHAPE_H
#include "object_extruder.h"
std::pair<footprint, footprint> create_square_donut_footprints();
GeometryStorage create_extruded_square_donut();
std::pair<footprint, footprint> create_cylinder_footprints();
std::pair<footprint, footprint> create_washer_footprints();
std::pair<footprint, footprint> create_2x_washer_footprints();
std::pair<footprint, footprint> create_four_square_footprints();
std::pair<footprint, footprint> create_case_1();
std::pair<footprint, footprint> create_case_2();
std::pair<footprint, footprint> create_triangle_footprints();
std::pair<footprint, footprint> create_the_first_object();
std::pair<footprint, footprint> create_the_second_object();
std::pair<footprint, footprint> create_sqaure_triangle();
std::pair<footprint, footprint> create_the_second_test();
std::pair<footprint, footprint> create_tmd_square();

bool test_repeated_footprint(footprint test);

// Creates a ClipperLib path representing a circle
ClipperLib::Path create_circle(int x0, int y0, int num, int radius);

// These two functions are used to emulate the conformal operations used in the layout engine.
std::vector<ClipperLib::Paths> create_gmsh_polygons(const ClipperLib::PolyTree &polytree);
ClipperLib::Paths convert_footprint(const footprint& footprint);
// A footprint can be conformed by passing it in as the subject parameter and using some other set
// of paths to act as the object in the other layer touching it.
ClipperLib::Paths convert_footprint(const footprint& footprint);
std::pair<footprint, footprint> create_2x_washer_footprints();
std::pair<footprint, footprint> create_square_and_numerous_circles();
std::pair<footprint, footprint> create_square_and_14_circles();
std::vector<ClipperLib::Paths> conform_polygons(const ClipperLib::Paths &path_subj,
												const ClipperLib::Paths &path_conform);
// Large integers must be used to make accurate geometry, so we can scale up small integers with
// this function.
void scale_footprint(footprint &footprint, int scale);
#endif // MANUALEXTRUDER_TEST_SHAPE_H
