#include <iostream>
#include <vector>
#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SNAKE_SIZE 20
#define FOOD_SIZE 20

enum directions { UP, DOWN, LEFT, RIGHT };

class Entity {
public:
    Vector2 pos;
};

class Food : public Entity {
public:
    Food(const std::vector<Entity>& snake_positions) {
        bool valid_position;
        do {
            valid_position = true;
            pos.x = (float)(GetRandomValue(0, (SCREEN_WIDTH / SNAKE_SIZE) - 1) * SNAKE_SIZE);
            pos.y = (float)(GetRandomValue(0, (SCREEN_HEIGHT / SNAKE_SIZE) - 1) * SNAKE_SIZE);

            // Ensure food does not spawn on the snake
            for (const auto& segment : snake_positions) {
                if (CheckCollisionRecs({pos.x, pos.y, FOOD_SIZE, FOOD_SIZE},
                                       {segment.pos.x, segment.pos.y,
                                        SNAKE_SIZE, SNAKE_SIZE})) {
                    valid_position = false;
                    break;
                }
            }
        } while (!valid_position);
    }

    void draw() { DrawRectangle(pos.x, pos.y, FOOD_SIZE, FOOD_SIZE, RED); }
};

class Snake {
public:
    std::vector<Entity> positions;
    directions dir;

    Snake(float x, float y, int start_size = 5) {
        positions.push_back({x, y});
        dir = UP;

        for (int i = 1; i < start_size; ++i) {
            positions.push_back({x, y - i * SNAKE_SIZE});
        }
    }

    void add_segment() { positions.push_back(positions.back()); }

    void move() {
        for (int i = positions.size() - 1; i > 0; --i) {
            positions[i] = positions[i - 1];
        }

        switch (dir) {
            case UP:
                positions[0].pos.y -= SNAKE_SIZE;
                break;
            case DOWN:
                positions[0].pos.y += SNAKE_SIZE;
                break;
            case LEFT:
                positions[0].pos.x -= SNAKE_SIZE;
                break;
            case RIGHT:
                positions[0].pos.x += SNAKE_SIZE;
                break;
        }

        if (positions[0].pos.x < 0)
            positions[0].pos.x = SCREEN_WIDTH - SNAKE_SIZE;
        else if (positions[0].pos.x >= SCREEN_WIDTH)
            positions[0].pos.x = 0;
        if (positions[0].pos.y < 0)
            positions[0].pos.y = SCREEN_HEIGHT - SNAKE_SIZE;
        else if (positions[0].pos.y >= SCREEN_HEIGHT)
            positions[0].pos.y = 0;
    }

    void change_direction() {
        if (IsKeyDown(KEY_W) && dir != DOWN)
            dir = UP;
        else if (IsKeyDown(KEY_S) && dir != UP)
            dir = DOWN;
        else if (IsKeyDown(KEY_A) && dir != RIGHT)
            dir = LEFT;
        else if (IsKeyDown(KEY_D) && dir != LEFT)
            dir = RIGHT;
    }

    bool check_collision(const Entity& other) {
        return positions[0].pos.x == other.pos.x && positions[0].pos.y == other.pos.y;
    }

    bool check_self_collision() {
        for (size_t i = 1; i < positions.size(); ++i) {
            if (positions[0].pos.x == positions[i].pos.x &&
                positions[0].pos.y == positions[i].pos.y) {
                return true;
            }
        }
        return false;
    }

    void draw() {
        for (const auto& segment : positions) {
            DrawRectangle(segment.pos.x, segment.pos.y, SNAKE_SIZE, SNAKE_SIZE, BLACK);
        }
    }
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    SetTargetFPS(10);

    Snake snake(100, 100, 5);
    Food food(snake.positions);
    bool game_over = false;

    int score = 0;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_Q)) {
            break;
        }

        snake.change_direction();
        snake.move();

        if (GetTime() > 1 && snake.check_self_collision()) {
            game_over = true;
        }

        if (snake.check_collision(food)){
            score++;
            food = Food(snake.positions);
            snake.add_segment();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Score: %d", score), 10, 10, 20, DARKGRAY);
        DrawText("Press R to reset", SCREEN_WIDTH - 190, SCREEN_HEIGHT - 50, 20,
                 DARKGRAY);
        DrawText("Press Q to quit", SCREEN_WIDTH - 190, SCREEN_HEIGHT - 30, 20,
                 DARKGRAY);

        if (!game_over) {
            snake.draw();
            food.draw();
        } else {
            DrawText("Game Over!", SCREEN_WIDTH / 2 - 150,
                     SCREEN_HEIGHT / 2 - 50, 60, RED);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}