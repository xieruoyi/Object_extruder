#include "gmsh.h"
#include <algorithm>
#include <array>
#include "object_extruder.h"
#include "test_shapes.h"


/// This the final functionality needed.

int main()
{
	gmsh::initialize();
	footprint lower; // lower = create test data
	footprint upper; // upper = create test data
	std::pair<footprint, footprint> low_up =create_tmd_square();
	int a = 0;
	if(test_repeated_footprint(low_up.first) == false)
	{
		a ++;
	}
	if(test_repeated_footprint(low_up.second) == false)
	{
		a++;
	}
	// create_square_donut_footprints();
	GeometryStorage object = create_extruded_object(low_up.first,low_up.second, 0.0, 5, 1);
	// Pass object to gmsh for visualization
	add_extruded_object_to_gmsh(object,1,0.001);
//	std::pair<footprint, footprint> low_up_2 = create_the_second_object();
//	GeometryStorage object2 = create_extruded_object(low_up_2.first,low_up_2.second, 5.0, 9.0, 1);
//	reindex_extruded_object(object2,object.points.size()+1,object.lines.size()+1,object.contours.size()+1,object.surfaces.size()+1);//,object.sides.size()+1,object.upper.size()+1,object.lower.size()+1
//	add_extruded_object_to_gmsh(object2,1.0,1);
	gmsh::model::geo::synchronize();
	gmsh::fltk::run();
	gmsh::finalize();
	return 0;
}
