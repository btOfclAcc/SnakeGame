#include "Snake.h"
#include <raylib.h>
#include <string>


Vector2& operator+= (Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

bool operator==(const Vector2& a, const Vector2& b)
{
	return a.x == b.x && a.y == b.y;
}



void Snake::Main()
{
	InitWindow(screenWidth, screenHeight, "Snake");

	Start();
	SetTargetFPS(60); // make it stead on all devices

	while (!WindowShouldClose())
	{
		Update();
	}
	CloseWindow();
}


void Snake::Start()
{
	player.clear();
	gameOver = false;
	pickup.active = false;
	speed = Vector2{ SQUARE_SIZE, 0 };
	frameIncrement = 20;

	Piece head;
	head.color = DARKGREEN;
	head.position = Vector2{ borderGap.x / 2, borderGap.y / 2 };
	player.push_back(head);
}

void Snake::EvalCurFrame()
{
	if (!gameOver) {
		frameCounter++;

		if (frameCounter % frameIncrement == 0) {
			for (int i = player.size() - 1; i > 0; i--) {
				player[i].position = player[i - 1].position;
			}

			// move the head of the player with the speed
			player[0].position += speed;

			canMove = true;
		}

		Interaction();

		// bounds top-left and lower-right
		Vector4 bounds =
			Vector4{ borderGap.x / 2,
					borderGap.y / 2,
					screenWidth - borderGap.x / 2,
					screenHeight - borderGap.y / 2 };

		if (player[0].position.x < bounds.x ||
			player[0].position.x >= bounds.z ||
			player[0].position.y < bounds.y ||
			player[0].position.y >= bounds.w)
		{
			// game over
			gameOver = true;
		}

		for (int i = 1; i < player.size() - 1; i++) {
			if (player[0].position == player[i].position) {
				gameOver = true;
			}
		}

		// check if there is a pickup
		if (!pickup.active) {
			// re-spawn
			pickup.newRandomLoc(borderGap);
			pickup.active = true;

			// you have to check that it's not colliding with player or its tail
			for (Piece p : player) {
				while (p.position == pickup.position) {
					pickup.newRandomLoc(borderGap);
				}
			}
		}

		// if we run into the pickup
		if (player[0].position == pickup.position) {
			if (frameIncrement > 1) {
				frameIncrement--;
			}
			pickup.active = false;

			Piece newPiece;
			newPiece.color = GREEN;
			player.push_back(newPiece);
		}
	}
	else {
		if (IsKeyPressed(KEY_ENTER)) {
			Start();
		}
	}

}

void Snake::DrawCurFrame()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	if (!gameOver) {
		// draw vertical lines
		for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++) {
			Vector2 start = Vector2{ borderGap.x / 2 + SQUARE_SIZE * i,
										borderGap.y / 2 };
			Vector2 end = Vector2{ borderGap.x / 2 + SQUARE_SIZE * i,
										screenHeight - borderGap.y / 2 };
			DrawLineV(start, end, GRAY);
		}
		// draw horizontal lines
		for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++) {
			Vector2 start = Vector2{ borderGap.x / 2,
										borderGap.y / 2 + SQUARE_SIZE * i };

			Vector2 end = Vector2{ screenWidth - borderGap.x / 2,
										borderGap.y / 2 + SQUARE_SIZE * i };

			DrawLineV(start, end, GRAY);
		}

		// draw player
		for (Piece h : player)
			h.Draw();

		pickup.Draw();
	}
	else {
		int fontSize = 36;
		char over[] = "Game is Over";

		float posX = screenWidth / 2 - MeasureText(over, fontSize) / 2;
		DrawText(over, posX, screenHeight / 2 - fontSize / 2, fontSize, RED);
	}


	EndDrawing();
}

void Snake::Update()
{
	EvalCurFrame();
	DrawCurFrame();
}

void Snake::Interaction()
{
	if (canMove) {
		if (cheatsOn) {
			Vector2 moveDirectory = Vector2{ pickup.position.x - player[0].position.x, pickup.position.y - player[0].position.y };

			if (AIStepCheck('x', moveDirectory.x) && moveDirectory.x != 0) {
				if (moveDirectory.x >= 1) {
					speed = Vector2{ SQUARE_SIZE, 0 };
					canMove = false;
				}
				else if (moveDirectory.x <= -1) {
					speed = Vector2{ -SQUARE_SIZE, 0 };
					canMove = false;
				}
			}
			else if (AIStepCheck('y', moveDirectory.y) && moveDirectory.y != 0) {
				if (moveDirectory.y <= 1) {
					speed = Vector2{ 0, -SQUARE_SIZE };
					canMove = false;
				}
				else if (moveDirectory.y >= -1) {
					speed = Vector2{ 0, SQUARE_SIZE };
					canMove = false;
				}
			}
		}

		if (IsKeyPressed(KEY_RIGHT)) {
			if (speed.x == 0) {
				speed = Vector2{ SQUARE_SIZE, 0 };
				canMove = false;
			}
		}
		if (IsKeyPressed(KEY_LEFT)) {
			if (speed.x == 0) {
				speed = Vector2{ -SQUARE_SIZE, 0 };
				canMove = false;
			}
		}
		if (IsKeyPressed(KEY_UP)) {
			if (speed.y == 0) {
				speed = Vector2{ 0, -SQUARE_SIZE };
				canMove = false;
			}
		}
		if (IsKeyPressed(KEY_DOWN)) {
			if (speed.y == 0) {
				speed = Vector2{ 0, SQUARE_SIZE };
				canMove = false;
			}
		}
	}
}

bool Snake::AIStepCheck(char direction, int distance)
{
	bool colisionAlert = false;
	Vector2 tempPosition = player[0].position;

	if (distance >= 1) {
		if (direction == 'x') {
			tempPosition.x++;
		}
		else if (direction == 'y') {
			tempPosition.y++;
		}
	}
	else if (distance <= -1) {
		if (direction == 'x') {
			tempPosition.x--;
		}
		else if (direction == 'y') {
			tempPosition.y--;
		}
	}

	for (int i = 1; i < player.size() - 1; i++) {
		if (player[0].position == player[i].position) {
			colisionAlert = true;
			break;
		}
	}

	Vector4 bounds =
		Vector4{ borderGap.x / 2,
				borderGap.y / 2,
				screenWidth - borderGap.x / 2,
				screenHeight - borderGap.y / 2 };

	if (tempPosition.x < bounds.x ||
		tempPosition.x >= bounds.z ||
		tempPosition.y < bounds.y ||
		tempPosition.y >= bounds.w)
	{
		colisionAlert = true;
	}

	if (colisionAlert) {
		return false;
	}
	else {
		return true;
	}
}
