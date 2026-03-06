#include <stdio.h>
#include <raylib.h>

#define WIDTH 900
#define HEIGHT 600

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "Electric Field Visualizer");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
