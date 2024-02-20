#pragma once
#include <raylib.h>
#include <vector>

#ifndef SNAKE_H
#define SNAKE_H

class Snake
{
public:
#define SQUARE_SIZE 77
	static const int screenWidth = 800;
	static const int screenHeight = 600;

	Vector2 borderGap = Vector2{ screenWidth % SQUARE_SIZE,
									screenHeight % SQUARE_SIZE };

	typedef struct Piece {
		Vector2 position;
		Vector2 size = Vector2{ SQUARE_SIZE, SQUARE_SIZE };
		Color color;

		void Draw()
		{
			DrawRectangleV(position, size, color);
		}

	} Piece;

	typedef struct Pickup {
		Vector2 position;
		Vector2 size = Vector2{ SQUARE_SIZE, SQUARE_SIZE };
		Color color = ORANGE;
		bool active = false; // at the beginning, I want to spawn

		void Draw()
		{
			DrawRectangleV(position, size, color);
		}

		void newRandomLoc(Vector2 gap)
		{
			float x = GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + gap.x / 2;
			float y = GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + gap.y / 2;

			position = Vector2{ x, y };
		}
	} Pickup;

	Pickup pickup = { 0 };

	std::vector <Piece> player;
	Vector2 speed = Vector2{ SQUARE_SIZE, 0 };
	int frameCounter = 0;

	bool gameOver = false;

	bool cheatsOn = true;

	bool canMove = true;

	int frameIncrement = 20;

	void Main();
	void Start();
	void EvalCurFrame();
	void DrawCurFrame();
	void Update();
	void Interaction();
	bool AIStepCheck(char, int);
};
#endif