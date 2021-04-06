/*******************************************************************************************
*
*   raylib [core] example - Third Person Orbit Camera Example
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/


#include "FPCamera.h"
#include "RLOctree.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <vector>

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1900;
	int screenHeight = 900;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "raylibExtras [camera] example - First person camera");
	SetTargetFPS(144);

    Image img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
    Texture tx = LoadTextureFromImage(img);
    UnloadImage(img);
    SetTextureFilter(tx, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(tx, TEXTURE_WRAP_CLAMP);

	FPCamera cam;
	cam.Setup(45,Vector3{ 1, 0, 0 });
	cam.MoveSpeed.z = 10;
	cam.MoveSpeed.x = 5;

	cam.FarPlane = 5000;

	std::vector<Vector3> trees;

    float spacing = 4;
    int count = 5;
	int total = 0;

	RLOctree octree;

    for (float x = -count * spacing; x <= count * spacing; x += spacing)
    {
        for (float z = -count * spacing; z <= count * spacing; z += spacing)
        {
            Vector3 pos = { x, 0.5f, z };

            Vector3 min = { x - 0.5f,0,z - 0.5f };
            Vector3 max = { x + 0.5f,2,z + 0.5f };
            total++;
			octree.Add(min, max, reinterpret_cast<void*>(trees.size()));
			trees.push_back(pos);
        }
    }

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		cam.Update();

		BeginDrawing();
		ClearBackground(WHITE);

		cam.BeginMode3D();

		// grid of cube trees on a plane to make a "world"
		DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE); // simple world plane
	
		std::vector<RLOctreeObject::Ptr> visibile;
		int vis = (int)octree.GetObjectsInFrustum(visibile, cam.GetFrustum());

		for (auto item : visibile)
		{
			size_t index = reinterpret_cast<size_t>(item->Object);
			Vector3 pos = trees[index];

            DrawCubeTexture(tx, Vector3{ pos.x, 1.5f, pos.z }, 1, 1, 1, GREEN);
            DrawCubeTexture(tx, Vector3{ pos.x, 0.5f, pos.z }, 0.25f, 1, 0.25f, BROWN);
		}

		octree.DrawDebug();

		cam.EndMode3D();

		DrawText(TextFormat("%d visible of %d total", vis, total), 200, 0, 20, GREEN);

		DrawFPS(0, 0);
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	UnloadTexture(tx);
	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}