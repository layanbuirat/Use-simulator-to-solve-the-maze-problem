#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "API.h"

// Direction tracking
// 0 = North, 1 = East, 2 = South, 3 = West
int current_direction = 0;
// Starting position at (0,0)
int x_pos = 0, y_pos = 0;

void log_message(char* message) {
    fprintf(stderr, "%s", message);
    fflush(stderr);
}

void log_integer(int number) {
    fprintf(stderr, "%d", number);
    fflush(stderr);
}

void log_position(int x, int y){
    log_message("Current Position: ");
    log_integer(x);
    log_message(", ");
    log_integer(y);
    log_message("\n");
}

bool reached_goal(int x, int y, int goalX, int goalY){
    if (x == goalX && y == goalY){
        log_message("The mouse has reached the goal!\n");
        return true;
    }
    return false;
}

void update_coordinates(){
    if (current_direction == 0) // North
        y_pos += 1;
    else if (current_direction == 1) // East
        x_pos += 1;
    else if (current_direction == 2) // South
        y_pos -= 1;
    else if (current_direction == 3) // West
        x_pos -= 1;
}

void change_direction(int turn) {
    current_direction = (current_direction + turn + 4) % 4;
}

void mark_visited() {
    API_setColor(x_pos, y_pos, 'R');
}

int main(int argc, char* argv[]) {
    log_message("Starting...\n");

    // Set goal position
    int goalX = 12, goalY = 6;

    API_setColor(x_pos, y_pos, 'R');
    API_setText(x_pos, y_pos, "Start");
    
    API_setColor(goalX, goalY, 'G');
    API_setText(goalX, goalY, "Goal");
    log_message("Colors set...\n");

    while (1) {
        if (reached_goal(x_pos, y_pos, goalX, goalY))
            return 0;

        // Follow the left wall
        while (API_wallLeft()) {
            if (reached_goal(x_pos, y_pos, goalX, goalY))
                return 0;

            log_message("Wall on the left\n");
            if (!API_wallFront()) {
                // Move forward if no wall in front
                mark_visited();
                API_moveForward();
                update_coordinates();
                log_position(x_pos, y_pos);
                log_message("Moved forward\n");
            } else {
                if (!API_wallRight()) {
                    log_message("Wall in front\n");
                    log_message("No wall on right\n");
                    API_turnRight();
                    change_direction(1);
                    log_message("Turned right\n");
                    mark_visited();
                    API_moveForward();
                    update_coordinates();
                    log_position(x_pos, y_pos);
                    log_message("Moved forward\n");  
                } else {
                    // Turn around if walls in all directions
                    API_turnRight();
                    change_direction(1);
                    API_turnRight();
                    change_direction(1);
                    log_message("Turned around\n");
                    mark_visited();
                    API_moveForward();
                    update_coordinates();
                    log_position(x_pos, y_pos);
                    log_message("Moved forward\n");
                }
            }
        }
        
        if (reached_goal(x_pos, y_pos, goalX, goalY))
            return 0;
        
        // Turn left if no wall on the left
        API_turnLeft();
        change_direction(-1);
        log_message("Current direction: ");
        log_integer(current_direction);
        log_message("\n");
        mark_visited();
        API_moveForward();
        update_coordinates();
        log_position(x_pos, y_pos);
    }

    return 0;
}