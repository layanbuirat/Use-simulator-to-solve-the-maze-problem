#include <stdio.h>
#include <stdbool.h>
#include "API.h"

// Maze size
#define MAZE_SIZE 16

// Starting position
int x_pos = 0, y_pos = 0;
bool visited[MAZE_SIZE][MAZE_SIZE] = {false};

// Goal position
int goalX = 12, goalY = 6;

// Directions: 0 = North, 1 = East, 2 = South, 3 = West
int current_direction = 0;

// Flag to terminate recursion
bool goal_reached = false;

// Logging utility
void log_message(char* message) {
    fprintf(stderr, "%s", message);
    fflush(stderr);
}

void log_position(int x, int y) {
    fprintf(stderr, "Current Position: (%d, %d)\n", x, y);
    fflush(stderr);
}

void log_direction() {
    char* directions[] = {"North", "East", "South", "West"};
    fprintf(stderr, "Current Direction: %s\n", directions[current_direction]);
    fflush(stderr);
}

// Utility to check if the position is valid
bool is_valid_position(int x, int y) {
    return x >= 0 && x < MAZE_SIZE && y >= 0 && y < MAZE_SIZE;
}

// Check if the current position is the goal
bool is_goal(int x, int y) {
    return x == goalX && y == goalY;
}

// Update coordinates based on direction
void update_position(int* x, int* y) {
    if (current_direction == 0) (*y)++;           // North
    else if (current_direction == 1) (*x)++;      // East
    else if (current_direction == 2) (*y)--;      // South
    else if (current_direction == 3) (*x)--;      // West
}

// Simulate turning
void turn_left() {
    current_direction = (current_direction + 3) % 4;
    API_turnLeft();
}

void turn_right() {
    current_direction = (current_direction + 1) % 4;
    API_turnRight();
}

void turn_around() {
    current_direction = (current_direction + 2) % 4;
    API_turnRight();
    API_turnRight();
}

// Recursive flood-fill function
void flood_fill(int x, int y) {
    if (goal_reached || !is_valid_position(x, y) || visited[x][y]) {
        return;
    }

    visited[x][y] = true; // Mark as visited
    API_setColor(x, y, 'Y'); // Mark on the maze
    log_position(x, y);

    // Check if we've reached the goal
    if (is_goal(x, y)) {
        log_message("Goal reached!\n");
        goal_reached = true; // Set the flag to stop further recursion
        return;
    }

    // Explore each direction (relative to the current orientation)
    for (int i = 0; i < 4; i++) {
        if (goal_reached) return; // Terminate further exploration if goal is reached

        if (!API_wallFront()) {
            // Move forward if no wall
            API_moveForward();
            update_position(&x, &y);
            flood_fill(x, y);

            // Backtrack after exploring
            if (!goal_reached) { // Only backtrack if goal is not reached
                turn_around();
                API_moveForward();
                update_position(&x, &y);
                turn_around();
            }
        }
        // Turn right to check the next direction
        turn_right();
        log_direction();
    }
}

int main() {
    log_message("Starting flood-fill algorithm...\n");

    API_setColor(goalX, goalY, 'G'); // Mark the goal
    API_setText(goalX, goalY, "Goal");

    // Start flood-fill from the initial position
    flood_fill(x_pos, y_pos);

    if (goal_reached) {
        log_message("Flood-fill complete: Goal successfully reached.\n");
    } else {
        log_message("Flood-fill complete: Goal not found.\n");
    }

    return 0;
}