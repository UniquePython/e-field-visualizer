#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 900
#define HEIGHT 900

#define ROWS 20
#define COLS 20

#define FONT_SIZE 12

#define CELL_W ((WIDTH) / (COLS))
#define CELL_H ((HEIGHT) / (ROWS))

float g_field[ROWS][COLS], g_charges[ROWS][COLS];

void init_charges(void)
{
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			g_charges[i][j] = (rand() % 3) - 1;
}

void zero_field(void)
{
	memset(g_field, 0, sizeof(g_field));
}

void update_field(void)
{
	for (int self_y = 0; self_y < ROWS; self_y++)
	{
		for (int self_x = 0; self_x < COLS; self_x++)
		{
			for (int other_y = 0; other_y < ROWS; other_y++)
			{
				for (int other_x = 0; other_x < COLS; other_x++)
				{
					if (other_y == self_y && other_x == self_x)
						continue;

					float r_sq = (other_x - self_x) * (other_x - self_x) + (other_y - self_y) * (other_y - self_y);
					g_field[self_y][self_x] += g_charges[other_y][other_x] / r_sq;
				}
			}
		}
	}
}

void draw_grid(void)
{
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
		{
			int x = j * CELL_W;
			int y = i * CELL_H;

			DrawRectangleLines(x, y, CELL_W, CELL_H, GRAY);

			char buf[8] = {0};
			snprintf(buf, sizeof(buf), "%.2f", g_field[i][j]);
			int text_width = MeasureText(buf, FONT_SIZE);

			DrawText(buf, x + ((CELL_W - text_width) / 2), y + (CELL_H / 2), FONT_SIZE, BLACK);
		}
}

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "Electric Field Visualizer");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	srand(time(NULL));
	init_charges();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		zero_field();
		update_field();
		draw_grid();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
