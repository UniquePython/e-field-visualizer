#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 900

#define ROWS 20
#define COLS 20

#define FONT_SIZE 12

#define CELL_W ((WIDTH) / (COLS))
#define CELL_H ((HEIGHT) / (ROWS))

#define ARROW_THICK 2.0f
#define ARROW_LEN ((CELL_W) * 0.4f)
#define BARB_LEN ((ARROW_LEN) * 0.3f)
#define BARB_ANGLE (135.0f * DEG2RAD)

float g_field_x[ROWS][COLS], g_field_y[ROWS][COLS], g_charges[ROWS][COLS];

void find_min_max(float *min, float *max)
{
	*min = 0;
	*max = 0;

	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
		{
			float magnitude = sqrtf(powf(g_field_x[i][j], 2.0f) + powf(g_field_y[i][j], 2.0f));

			if (magnitude > *max)
				*max = magnitude;

			if (magnitude < *min)
				*min = magnitude;
		}
}

float normalize(float min, float max, float val)
{
	float delta = max - min;
	if (delta == 0)
		return 1.0f;
	return (val - min) / delta;
}

void init_charges(void)
{
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			g_charges[i][j] = (rand() % 3) - 1;
}

void zero_field(void)
{
	memset(g_field_x, 0, sizeof(g_field_x));
	memset(g_field_y, 0, sizeof(g_field_y));
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
					float r = sqrtf(r_sq);

					float field = g_charges[other_y][other_x] / r_sq;
					g_field_x[self_y][self_x] += (self_x - other_x) * field / r;
					g_field_y[self_y][self_x] += (self_y - other_y) * field / r;
				}
			}
		}
	}
}

void draw_grid(void)
{
	float min, max;
	find_min_max(&min, &max);

	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
		{
			int x = j * CELL_W;
			int y = i * CELL_H;

			float magnitude = sqrtf(powf(g_field_x[i][j], 2.0f) + powf(g_field_y[i][j], 2.0f));

			// Heatmap
			Color c = ColorLerp(BLUE, RED, normalize(min, max, magnitude));
			DrawRectangle(x, y, CELL_W, CELL_H, c);
			DrawRectangleLines(x, y, CELL_W, CELL_H, GRAY);

			// Arrow
			float cx = x + CELL_W / 2.0f;
			float cy = y + CELL_H / 2.0f;

			if (magnitude == 0)
				continue;

			float dx = (g_field_x[i][j] / magnitude) * ARROW_LEN;
			float dy = (g_field_y[i][j] / magnitude) * ARROW_LEN;

			Vector2 start = {cx, cy};
			Vector2 end = {cx + dx, cy + dy};

			DrawLineEx(start, end, ARROW_THICK, RAYWHITE);

			float bx_r = dx * cosf(BARB_ANGLE) - dy * sinf(BARB_ANGLE);
			float by_r = dx * sinf(BARB_ANGLE) + dy * cosf(BARB_ANGLE);
			float bx_l = dx * cosf(-BARB_ANGLE) - dy * sinf(-BARB_ANGLE);
			float by_l = dx * sinf(-BARB_ANGLE) + dy * cosf(-BARB_ANGLE);

			Vector2 barb_r = {end.x + bx_r * BARB_LEN / ARROW_LEN, end.y + by_r * BARB_LEN / ARROW_LEN};
			Vector2 barb_l = {end.x + bx_l * BARB_LEN / ARROW_LEN, end.y + by_l * BARB_LEN / ARROW_LEN};

			DrawLineEx(end, barb_r, ARROW_THICK, RAYWHITE);
			DrawLineEx(end, barb_l, ARROW_THICK, RAYWHITE);

			// Text
			// char buf[8] = {0};
			// snprintf(buf, sizeof(buf), "%.2f", magnitude);
			// int text_width = MeasureText(buf, FONT_SIZE);

			// DrawText(buf, x + ((CELL_W - text_width) / 2), y + (CELL_H / 2), FONT_SIZE, BLACK);
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
