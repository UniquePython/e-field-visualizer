#include <stdio.h>
#include <raylib.h>
#include <string.h>

#define WIDTH 900
#define HEIGHT 600

#define ROWS 20
#define COLS 20

#define CELL_W ((WIDTH) / (COLS))
#define CELL_H ((HEIGHT) / (ROWS))

float g_grid[ROWS][COLS];

void zero_grid(void)
{
	memset(g_grid, 0, sizeof(g_grid));
}

void draw_grid(void)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			DrawRectangleLines(j * CELL_W, i * CELL_H, CELL_W, CELL_H, GRAY);
		}
	}
}

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "Electric Field Visualizer");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		zero_grid();
		draw_grid();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
