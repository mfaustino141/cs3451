//#####################################################################
// Main
// CS3451 Computer Graphics Starter Code
// Contact: Bo Zhu (bo.zhu@gatech.edu)
//#####################################################################
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>

#include "OpenGLMesh.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

#define DegreesToRadians(degrees) (degrees * M_PI / 180)

class MyDriver : public OpenGLViewer
{
	std::vector<OpenGLTriangleMesh*> mesh_object_array;						////mesh objects, every object you put in this array will be rendered.
	OpenGLSegmentMesh* floor = nullptr;
	std::vector<OpenGLSegmentMesh*> coords, trajectory;
	clock_t startTime;
public:
	virtual void Initialize()
	{
		draw_axes = false;
		startTime = clock();
		OpenGLViewer::Initialize();
		opengl_window->camera_distance = 25.f;
		opengl_window->camera_target = Vector3f(0, 3.5, 0);
		opengl_window->Update_Clip_Planes();
	}

	void Create_Background(const OpenGLColor& color1, const OpenGLColor& color2)
	{
		auto bg = Add_Interactive_Object<OpenGLBackground>();
		bg->Set_Color(color1, color2);
		bg->Initialize();
	}

	virtual void Initialize_Data()
	{
		Create_Background(OpenGLColor(0.53f, 0.81f, 0.98f, 1.f), OpenGLColor(0.13f, 0.56f, 0.92f, 1.f));
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("a3_vert.vert", "a3_frag.frag", "a3_shading");	////bind shader for this assignment

		//og background colors: OpenGLColor(0.71f, 0.6f, 0.17f, 1.f), OpenGLColor(0.71f, 0.87f, 0.17f, 1.f)
		//custom sky colors: OpenGLColor(0.53f, 0.81f, 0.98f, 1.f), OpenGLColor(0.13f, 0.56f, 0.92f, 1.f)

		// Create_Angry_Bird_Palace();					////TODO: Comment this line when you start to implement your customized scene
		Create_Angry_Bird_Garden();			////TODO: Uncomment this line when you start to implement your customized scene

	}

	void Create_Angry_Bird_Palace()
	{
		//// draw the three axes
		Add_Coord({ Vector3(0, 0.01, 0), Vector3(5, 0.01, 0) }, OpenGLColor(1, 0, 0, 1));	//// X axis
		Add_Coord({ Vector3(0, 0, 0), Vector3(0, 5, 0) }, OpenGLColor(0, 1, 0, 1));	//// Y axis
		Add_Coord({ Vector3(0, 0.01, 0), Vector3(0, 0.01, 5) }, OpenGLColor(0, 0, 1, 1));	//// Z zxis

		//// draw the ground
		Add_Ground();

		//// Step 1: add the castle by reading the model from "castle.obj" 
		//// The model needs to undergo the following transform operations in sequence: 
		//// (1) rotate *counterclockwisely* around the y-axis by 90 degrees, 
		//// (2) uniformly scale by a factor of 5,
		//// (3) translate upwards by 1.3 units in the y direction.
		//// Your task is to specify the values of the 4x4 transform matrix and send it to the mesh model via Set_Model_Matrix().
		//// You are allowed to use a chain of matrix multiplications to calculate the matrix.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		auto castle = Add_Obj_Mesh_Object_From_File("castle.obj", OpenGLColor(.6f, .6f, .6f, 1.f));
		{
			Matrix4f t;
			t << 0., 0., 5., 0.,
				0., 5., 0., 1.3,
				-5., 0., 0., 0.,
				0., 0., 0., 1.;

			castle->Set_Model_Matrix(t);
		}
		/* Your implementation ends. */

		//// Step 2: add the axes statue by reading the model from "axes.obj" 
		//// The model needs to undergo the following transform operations in sequence: 
		//// (1) rotate *counterclockwisely* around the y-axis by 90 degrees, 
		//// (2) uniformly scale by a factor of 2,
		//// (3) translate by 6 units in the positive x direction and 1 unit in the y direction.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		auto axes = Add_Obj_Mesh_Object_From_File("axes.obj", OpenGLColor(.9f, .5f, .0f, 1.f));
		{
			Matrix4f t;
			t << 0., 0., 2., 6.,
				0., 2., 0., 1.,
				-2., 0., 0., 0.,
				0., 0., 0., 1.;

			axes->Set_Model_Matrix(t);
		}
		/* Your implementation ends. */

		//// Step 3: add the magic tower by reading the model from "tower.obj" 
		//// The model needs to undergo the following transform operations in sequence: 
		//// (1) rotate *clockwisely* around the y-axis by 45 degrees, 
		//// (2) non-uniformly scale by factors of 2, 6, 2 in the x, y, z directions
		//// (3) translate by 6 units in the negative x direction and 3 units in the y direction.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		auto tower = Add_Obj_Mesh_Object_From_File("tower.obj", OpenGLColor(.0f, .5f, .5f, 1.f));
		{
			Matrix4f t;
			t << sqrt(2.), 0., -sqrt(2.), -6.,
				0., 6., 0., 3.,
				sqrt(2.), 0., sqrt(2.), 0.,
				0., 0., 0., 1.;

			tower->Set_Model_Matrix(t);
		}
		/* Your implementation ends. */

		//// Step 4: add 24 trees by reading the model from "tree1.obj" 
		//// The 24 trees need to be distributed evenly and at equal distances along the circumference of the inner circle. 
		//// The circle has its center at the origin and a radius of 8.
		//// Each tree needs to be translated in the positive y axis by 0.5 unit to ensure its base is above the ground.
		//// Calculate the transform matrix for each tree in the following for-loop.
		 
		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		int tree_num = 24;
		float pi = 3.14159265358979323846f;
		for (int i = 0; i < tree_num; i++) {
			auto tree = Add_Obj_Mesh_Object_From_File("tree1.obj", OpenGLColor(0.f, 1.f, 0.f, 1.f));

			float angle = (2.0f * pi / tree_num) * i;

			{
				Matrix4f t;
				t << 1., 0., 0., 8 * cos(angle),
					0., 1., 0., 0.5,
					0., 0., 1., 8 * sin(angle),
					0., 0., 0., 1.;
				tree->Set_Model_Matrix(t);
			}
		}
		/* Your implementation ends. */

		//// Step 5: add 36 trees by reading the model from "tree2.obj" 
		//// The 36 trees need to be distributed evenly and at equal distances along the circumference of the outer circle. 
		//// The circle has its center at the origin and a radius of 10.
		//// Each tree needs to be translated in the positive y axis by 0.5 unit to ensure its base is above the ground.
		//// Calculate the transform matrix for each tree in the following for-loop.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		int tree2_num = 36;
		for (int i = 0; i < tree2_num; i++) {
			auto tree = Add_Obj_Mesh_Object_From_File("tree2.obj", OpenGLColor(0.f, 1.f, 0.f, 1.f));

			float angle = (2.0f * pi / tree2_num) * i;

			{
				Matrix4f t;
				t << 1., 0., 0., 10 * cos(angle),
					0., 1., 0., 0.5,
					0., 0., 1., 10 * sin(angle),
					0., 0., 0., 1.;
				tree->Set_Model_Matrix(t);
			}
		}
		/* Your implementation ends. */

		//// Step 6: add 5 stone steps by using the function `Add_Cube()`
		//// `Add_Cube()` adds a cube mesh to the scene with its center at the origin and each side measuring 1 unit.
		//// Each stone step has its size as 1, 0.1, and 0.5 along x, y, z axes.
		//// The center of the first stone step is in (0, 0, 3), incremented by 1 in positive z direction for the following stone steps.
		//// Calculate the transform matrix for each stone step in the following for-loop.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		for (int i = 0; i < 5; i++) {
			auto cube1 = Add_Cube(1.f, OpenGLColor(0.1f, 0.1f, 0.1f, 1.f));
			{
				Matrix4f t;
				t << 1., 0., 0., 0.,
					0., 0.1, 0., 0.,
					0., 0., 0.5, 3 + i,
					0., 0., 0., 1.;
				cube1->Set_Model_Matrix(t);
			}
		}
		/* Your implementation ends. */

		//// Step 7: add 6 keyframes of a throwing angry bird following a parabola.
		//// The angry bird is thrown from (-5, 0, 0) with initial velocity (5, 9.8, 0) (unit/sec)
		//// Its angular velocity omega = 150 (deg/sec) *clockwise* 		 
		//// The gravity is g = 9.8 (unit/sec) in negative y direction
		//// The x coordinate of the bird can be calculated as x = x0 + ux * t
		//// The y coordinate of the bird can be calculated as y = uy * t - 0.5 * g * t * t
		//// Your task is to draw keyframes at time t = 0.2, 0.5, 0.8, 1.1, 1.4, 1.7 respectively.
		//// To this end, you need to calculate the transform matrix for each keyframe of the angry bird in the following for-loop.
		//// You can temporarily uncomment the following line to visualize the parabola trajectory as a reference during implementation. 
		//// Comment it out again once you complete the task.

		// Add_Arc_Trajectory();

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		std::vector<float> time = { 0.2, 0.5, 0.8, 1.1, 1.4, 1.7 };
		int bird_num = 6;
		for (int i = 0; i < bird_num; i++) {
			auto bird = Add_Obj_Mesh_Object_From_File("bird.obj", OpenGLColor(1.f, 0.2f, 0.f, 1.f));
			{
				Matrix4f t;
				t << cos(150 * time[i] * pi / 180), sin(150 * time[i] * pi / 180), 0., -5. + 5. * time[i],
					-sin(150 * time[i] * pi / 180), cos(150 * time[i] * pi / 180), 0., (9.8 * time[i]) - (0.5 * 9.8 * time[i] * time[i]),
					0., 0., 1., 0.,
					0., 0., 0., 1.;
				bird->Set_Model_Matrix(t);
			}
		}
		/* Your implementation ends. */
	}

	//// Step 8: Create a new garden scene by using the mesh objects we provided, or download your own from online resources. 
	//// Practise matrix transformation by mimicking the way we setup matrices in the `Create_Angry_Bird_Palace()` function.

	void Create_Angry_Bird_Garden()
	{
		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */

		//// draw the three axes, comment them out if you don't need them
		// Add_Coord({ Vector3(0, 0.01, 0), Vector3(5, 0.01, 0) }, OpenGLColor(1, 0, 0, 1));	//// X axis
		// Add_Coord({ Vector3(0, 0, 0), Vector3(0, 5, 0) }, OpenGLColor(0, 1, 0, 1));	//// Y axis
		// Add_Coord({ Vector3(0, 0.01, 0), Vector3(0, 0.01, 5) }, OpenGLColor(0, 0, 1, 1));	//// Z zxis

		//// draw the ground, comment them out if you don't need them
		// Add_Ground();

		int scale = 1;
		int xOffset = 0;
		int yOffset = 0;
		int zOffset = 0;
		
		Matrix4f rotate90y;
		rotate90y << 0., 0., 1., 0.,
			0., 1., 0., 0.,
			-1., 0., 0., 0.,
			0., 0., 0., 1.;

		Matrix4f rotate45y;
		rotate45y << sqrt(2) / 2., 0., sqrt(2) / 2., 0.,
			0., 1., 0., 0.,
			-sqrt(2) / 2., 0., sqrt(2) / 2., 0.,
			0., 0., 0., 1.;

		auto building1 = Add_Obj_Mesh_Object_From_File("building1.obj", OpenGLColor(0.84f, 0.37f, 0.31f, 1.f));
		{
			Matrix4f t;
			t << 5. * scale, 0., 0., 0. + xOffset,
				0., 5. * scale, 0., 1.2 + yOffset,
				0., 0., 5. * scale, -7. + zOffset,
				0., 0., 0., 1.;

			building1->Set_Model_Matrix(t * rotate90y);
		}

		auto chair2 = Add_Obj_Mesh_Object_From_File("chair2.obj", OpenGLColor(0.44f, 0.13f, 0.17f, 1.f));
		{
			Matrix4f t;
			t << 2. * scale, 0., 0., -1. + xOffset,
				0., 2. * scale, 0., 1. + yOffset,
				0., 0., 2. * scale, -7. + zOffset,
				0., 0., 0., 1.;

			chair2->Set_Model_Matrix(t * rotate90y * rotate90y);
		}

		for(int i = 0; i < 2; i++){
			auto light1 = Add_Obj_Mesh_Object_From_File("light1.obj", OpenGLColor(0.95f, 0.82f, 0.90f, 1.f));
			{
				Matrix4f t;
				t << 1.6 * scale, 0., 0., -3. + (6*i) + xOffset,
					0., 1.6 * scale, 0., 0.9 + yOffset,
					0., 0., 1.6 * scale, -5.8 + zOffset,
					0., 0., 0., 1.;

				light1->Set_Model_Matrix(t);
			}
		}

		for(int i = 0; i < 7; i++){
			for(int j = 0; j < 2; j++){
				auto vase2 = Add_Obj_Mesh_Object_From_File("vase2.obj", OpenGLColor(0.59f, 0.26f, 0.35f, 1.f));
				{
					Matrix4f t;
					t << 1.8 * scale, 0., 0., -3. + (6*j) + xOffset,
						0., 1.8 * scale, 0., 0.5 + yOffset,
						0., 0., 1.8 * scale, -3. + (2*i) + zOffset,
						0., 0., 0., 1.;

					vase2->Set_Model_Matrix(t * rotate90y);
				}
			}
		}
		
		{
			auto ground = Add_Obj_Mesh_Object_From_File("ground.obj", OpenGLColor(0.74f, 0.62f, 0.64f, 1.f));
			Matrix4f t;
			t << 5.3, 0., 0., 0.,
				0., 20., 0., 0.005,
				0., 0., 25., 0.,
				0., 0., 0., 1.;

			ground->Set_Model_Matrix(t);
		}
		
		for(int i = 0; i < 2; i++){
			auto fountain1 = Add_Obj_Mesh_Object_From_File("fountain1.obj", OpenGLColor(0.88f, 0.84f, 0.97f, 1.f));
			{
				Matrix4f t;
				t << 3. * scale, 0., 0., -7. + (14*i) + xOffset,
					0., 3. * scale, 0., 1.5 + yOffset,
					0., 0., 3. * scale, 1.5 + zOffset,
					0., 0., 0., 1.;

				fountain1->Set_Model_Matrix(t);
			}
		}

		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 2; j++){
				auto chair3 = Add_Obj_Mesh_Object_From_File("chair3.obj", OpenGLColor(0.56f, 0.29f, 0.26f, 1.f));
				{
					Matrix4f t;
					t << 1.9 * scale, 0., 0., -7. + (14*i) + xOffset,
						0., 1.9 * scale, 0., 0.2 + yOffset,
						0., 0., 1.9 * scale, -0.7 + (4.5*j) + zOffset,
						0., 0., 0., 1.;

					chair3->Set_Model_Matrix(t * rotate90y);
				}
			}
		}

		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 2; j++){
				auto chair3 = Add_Obj_Mesh_Object_From_File("chair3.obj", OpenGLColor(0.56f, 0.29f, 0.26f, 1.f));
				{
					Matrix4f t;
					t << 1.9 * scale, 0., 0., -9. + (14*i) + (4.1*j) + xOffset,
						0., 1.9 * scale, 0., 0.2 + yOffset,
						0., 0., 1.9 * scale, -0.7 + (4.5*0.5) + zOffset,
						0., 0., 0., 1.;

					chair3->Set_Model_Matrix(t);
				}
			}
		}

		for(int i = 0; i < 9; i++){
			auto fence3 = Add_Obj_Mesh_Object_From_File("fence3.obj", OpenGLColor(0.56f, 0.29f, 0.26f, 1.f));
			{
				Matrix4f t;
				t << 2.3 * scale, 0., 0., -9. + (2.3*i) + xOffset,
					0., 2.3 * scale, 0., 0.7 + yOffset,
					0., 0., 2.3 * scale, -10. + zOffset,
					0., 0., 0., 1.;

				fence3->Set_Model_Matrix(t * rotate90y * rotate90y);
			}
		}

		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 6; j++){
				for(int k = 0; k < 4; k++){
					auto garden_tree1 = Add_Obj_Mesh_Object_From_File("garden_tree1.obj", OpenGLColor(0.8f, 0.82f, 0.47f, 1.f));
					{
						Matrix4f t;
						t << 2. * scale, 0., 0., -9. + (13*i) + (1*j) + xOffset,
							0., 2. * scale, 0., 0.7 + yOffset,
							0., 0., 2. * scale, -9. + (1*k) + zOffset,
							0., 0., 0., 1.;

						garden_tree1->Set_Model_Matrix(t);
					}
				}
			}
		}

		{
			auto chair1 = Add_Obj_Mesh_Object_From_File("chair1.obj", OpenGLColor(0.78f, 0.69f, 0.87f, 1.f));
			Matrix4f t;
			t << 1.5 * scale, 0., 0., 6. + xOffset,
				0., 1.5 * scale, 0., 0.4 + yOffset,
				0., 0., 1.5 * scale, -3.7 + zOffset,
				0., 0., 0., 1.;

			chair1->Set_Model_Matrix(t);
		}

		{
			auto chair1 = Add_Obj_Mesh_Object_From_File("chair1.obj", OpenGLColor(0.78f, 0.69f, 0.87f, 1.f));
			Matrix4f t;
			t << 1.5 * scale, 0., 0., 7.5 + xOffset,
				0., 1.5 * scale, 0., 0.4 + yOffset,
				0., 0., 1.5 * scale, -3.1 + zOffset,
				0., 0., 0., 1.;

			chair1->Set_Model_Matrix(t * rotate45y * rotate90y * rotate90y * rotate90y);
		}

		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 2; j++){
				auto chair4 = Add_Obj_Mesh_Object_From_File("chair4.obj", OpenGLColor(0.56f, 0.29f, 0.26f, 1.f));
				{
					Matrix4f t;
					t << 2.5 * scale, 0., 0., -8. + (13*i) + (3.2*j) + xOffset,
						0., 2.5 * scale, 0., 0.45 + yOffset,
						0., 0., 2.5 * scale, 7. + zOffset,
						0., 0., 0., 1.;

					chair4->Set_Model_Matrix(t);
				}
			}
		}

		{
			auto ground = Add_Obj_Mesh_Object_From_File("ground.obj", OpenGLColor(.51f, .56f, .24f, 1.f));
			Matrix4f t;
			t << 20., 0., 0., 0.,
				0., 20., 0., 0.,
				0., 0., 25., 0.,
				0., 0., 0., 1.;

			ground->Set_Model_Matrix(t);
		}

		/* Your implementation ends. */
	}

	//////////////////////////////////////////////////////////////
	//// The following functions are auxiliary functions to add mesh objects into the scene
	//////////////////////////////////////////////////////////////

	//// This function adds a mesh object from an obj file
	int Add_Obj_Mesh_Object(std::string obj_file_name)
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();

		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File(obj_file_name, meshes);
		mesh_obj->mesh = *meshes[0];
		std::cout << "load tri_mesh from obj file, #vtx: " << mesh_obj->mesh.Vertices().size() << ", #ele: " << mesh_obj->mesh.Elements().size() << std::endl;

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	//// This function adds a mesh object from an .obj file and allows the user to specify its model matrix
	OpenGLTriangleMesh* Add_Obj_Mesh_Object_From_File(std::string file_name, OpenGLColor color = OpenGLColor::White())
	{
		int obj_idx = Add_Obj_Mesh_Object(file_name);
		auto obj = mesh_object_array[obj_idx];
		obj->color = color; // set color

		obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shading"));
		Set_Polygon_Mode(obj, PolygonMode::Fill);
		Set_Shading_Mode(obj, ShadingMode::A2);
		obj->Set_Data_Refreshed();
		obj->Initialize();

		return obj;
	}
	//// This function adds a cube with a specified length and color
	OpenGLTriangleMesh* Add_Cube(float length = 1, OpenGLColor color = OpenGLColor::White()) {
		auto obj = Add_Interactive_Object<OpenGLTriangleMesh>();
		mesh_object_array.push_back(obj);
		obj->color = color; // set color
		// set up vertices and elements
		std::vector<Vector3> vertices{ Vector3(0,0,0),Vector3(1,0,0),Vector3(0,1,0),Vector3(1,1,0), Vector3(0,0,1),Vector3(1,0,1),Vector3(0,1,1),Vector3(1,1,1) };
		std::vector<Vector3i> elements{ Vector3i(4,5,7),Vector3i(4,7,6),
										Vector3i(5,1,7),Vector3i(7,1,3),
										Vector3i(2,3,1),Vector3i(0,2,1),
										Vector3i(6,2,4),Vector3i(2,0,4),
										Vector3i(2,6,3),Vector3i(6,7,3),
										Vector3i(0,1,4),Vector3i(1,5,4) };
		for (auto& v3 : vertices) { v3 -= Vector3(0.5, 0.5, 0.5); v3 *= length; };
		obj->mesh.Vertices() = vertices;
		obj->mesh.Elements() = elements;

		obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shading"));
		Set_Polygon_Mode(obj, PolygonMode::Fill);
		Set_Shading_Mode(obj, ShadingMode::A2);
		obj->Set_Data_Refreshed();
		obj->Initialize();

		return obj;
	}

	//// This function adds an axis with a specified color
	void Add_Coord(std::vector<Vector3> pts, OpenGLColor color)
	{
		auto axis = Add_Interactive_Object<OpenGLSegmentMesh>();
		coords.push_back(axis);
		Set_Polygon_Mode(axis, PolygonMode::Fill);
		Set_Shading_Mode(axis, ShadingMode::None);
		auto line_mesh = &axis->mesh;
		line_mesh->Vertices() = pts;
		line_mesh->Elements() = { Vector2i(0,1) };
		Set_Line_Width(axis, 2.5f);
		Set_Color(axis, color);
		axis->Set_Data_Refreshed();
		axis->Initialize();
	}

	//// This function adds a wireframe ground
	void Add_Ground()
	{
		floor = Add_Interactive_Object<OpenGLSegmentMesh>();
		Set_Polygon_Mode(floor, PolygonMode::Fill);
		Set_Shading_Mode(floor, ShadingMode::None);
		auto line_mesh = &floor->mesh;
		std::vector<Vector3> pts;
		std::vector<Vector2i> eles;
		for (int i = -10; i <= 10; i++) {
			pts.push_back(Vector3(i, 0, -10));
			pts.push_back(Vector3(i, 0, 10));
			pts.push_back(Vector3(-10, 0, i));
			pts.push_back(Vector3(10, 0, i));
		}
		for (int i = 0; i < pts.size(); i += 2) {
			eles.push_back(Vector2i(i, i + 1));
		}
		line_mesh->Vertices() = pts;
		line_mesh->Elements() = eles;
		Set_Line_Width(floor, 3.f);
		Set_Color(floor, OpenGLColor(0, 0, 0, 1));
		floor->Set_Data_Refreshed();
		floor->Initialize();
	}

	//// This function adds a parabola trajectory
	void Add_Arc_Trajectory()
	{
		float ux = 5, uy = 9.8, g = 9.8;
		std::vector<Vector3> pts;
		std::vector<Vector2i> eles;
		for (float t = 0; t <= 2; t += 0.02) {
			float x = ux * t - 5;
			float y = uy * t - 0.5 * g * t * t;
			pts.push_back(Vector3(x, y, 0));
		}
		for (int i = 0; i < pts.size() - 1; i++) {
			eles.push_back(Vector2i(i, i + 1));
		}
		auto traj = Add_Interactive_Object<OpenGLSegmentMesh>();
		trajectory.push_back(traj);
		Set_Polygon_Mode(traj, PolygonMode::Fill);
		Set_Shading_Mode(traj, ShadingMode::None);
		auto line_mesh = &traj->mesh;
		line_mesh->Vertices() = pts;
		line_mesh->Elements() = eles;
		Set_Line_Width(traj, 2.f);
		Set_Color(traj, OpenGLColor(0, 0, 1, 1));
		traj->Set_Data_Refreshed();
		traj->Initialize();
	}

	void Init_Bird_Vertices(OpenGLTriangleMesh* obj)
	{
		std::vector<Vector3>& vertices = obj->mesh.Vertices();
		for (auto& v : vertices) {
			v += Vector3(-8.35, 0, 0);
			auto x = v.x(); v.x() = v.z(); v.z() = -x;
			v /= 10.f;
		}
	}

	//// Go to next frame 
	virtual void Toggle_Next_Frame()
	{
		for (auto& mesh_obj : mesh_object_array) {
			mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
		}
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc, char* argv[])
{
	MyDriver driver;
	driver.Initialize();
	driver.Run();
}

#endif