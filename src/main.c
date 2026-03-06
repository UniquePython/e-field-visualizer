#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define WIDTH 900
#define HEIGHT 900

#define ROWS 20
#define COLS 20

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
			float magnitude = sqrtf(g_field_x[i][j] * g_field_x[i][j] + g_field_y[i][j] * g_field_y[i][j]);
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

void zero_charges(void)
{
	memset(g_charges, 0, sizeof(g_charges));
}

void zero_field(void)
{
	memset(g_field_x, 0, sizeof(g_field_x));
	memset(g_field_y, 0, sizeof(g_field_y));
}

void update_field(void)
{
	for (int self_y = 0; self_y < ROWS; self_y++)
		for (int self_x = 0; self_x < COLS; self_x++)
			for (int other_y = 0; other_y < ROWS; other_y++)
				for (int other_x = 0; other_x < COLS; other_x++)
				{
					if (other_y == self_y && other_x == self_x)
						continue;
					if (g_charges[other_y][other_x] == 0)
						continue;

					float r_sq = (other_x - self_x) * (other_x - self_x) + (other_y - self_y) * (other_y - self_y);
					float r = sqrtf(r_sq);

					float field = g_charges[other_y][other_x] / r_sq;
					g_field_x[self_y][self_x] += (self_x - other_x) * field / r;
					g_field_y[self_y][self_x] += (self_y - other_y) * field / r;
				}
}

void handle_input(void)
{
	if (IsKeyPressed(KEY_R))
		zero_charges();

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		Vector2 mouse = GetMousePosition();
		int col = (int)(mouse.x / CELL_W);
		int row = (int)(mouse.y / CELL_H);

		if (col < 0 || col >= COLS || row < 0 || row >= ROWS)
			return;

		float new_charge = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ? 1.0f : -1.0f;

		if (g_charges[row][col] == new_charge)
			g_charges[row][col] = 0;
		else
			g_charges[row][col] = new_charge;
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

			float magnitude = sqrtf(g_field_x[i][j] * g_field_x[i][j] + g_field_y[i][j] * g_field_y[i][j]);

			Color c = ColorLerp(BLACK, YELLOW, normalize(min, max, magnitude));
			DrawRectangle(x, y, CELL_W, CELL_H, c);

			if (magnitude == 0)
				continue;

			if (g_charges[i][j] == 0)
			{
				float cx = x + CELL_W / 2.0f;
				float cy = y + CELL_H / 2.0f;

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
			}
		}

	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
		{
			if (g_charges[i][j] == 0)
				continue;
			int cx = j * CELL_W + CELL_W / 2;
			int cy = i * CELL_H + CELL_H / 2;
			Color cc = g_charges[i][j] > 0 ? RED : BLUE;
			DrawCircle(cx, cy, CELL_W * 0.3f, cc);
		}
}

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "Electric Field Visualizer");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	zero_charges();

	while (!WindowShouldClose())
	{
		handle_input();

		BeginDrawing();
		ClearBackground(BLACK);
		zero_field();
		update_field();
		draw_grid();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}