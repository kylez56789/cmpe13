/*
 * File:   Field.c
 * Author: kylez
 *
 * Created on June 1, 2019, 3:57 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "BOARD.h"
#include <xc.h>
#include "Field.h"

static GuessData prevprevguess;
static GuessData previousguess;

/**
 * This function is optional, but recommended.   It prints a representation of both
 * fields, similar to the OLED display.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     SquareStatus.
 */
void FieldPrint_UART(Field *own_field, Field * opp_field)
{
    uint8_t row, col;
    for (row = 0; row < FIELD_ROWS; row++) {
        for (col = 0; col < FIELD_COLS; col++) {
            if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_EMPTY) {
                printf("o ");
            } else if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_SMALL_BOAT) {
                printf("3 ");
            } else if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_MEDIUM_BOAT) {
                printf("4 ");
            } else if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_LARGE_BOAT) {
                printf("5 ");
            } else if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_HUGE_BOAT) {
                printf("6 ");
            } else if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_HIT) {
                printf("$ ");
            } else if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_MISS) {
                printf("x ");
            }
        }
        printf(" | ");
        for (col = 0; col < FIELD_COLS; col++) {
            if (FieldGetSquareStatus(opp_field, row, col) == FIELD_SQUARE_HIT) {
                printf("$ ");
            } else if (FieldGetSquareStatus(opp_field, row, col) == FIELD_SQUARE_MISS) {
                printf("x ");
            } else if (FieldGetSquareStatus(opp_field, row, col) == FIELD_SQUARE_UNKNOWN) {
                printf("? ");
            } else if (FieldGetSquareStatus(opp_field, row, col) == FIELD_SQUARE_INVALID) {
                printf("I ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

/**
 * FieldInit() will initialize two passed field structs for the beginning of play.
 * Each field's grid should be filled with the appropriate SquareStatus.
 * Additionally, your opponent's field's boatLives parameters should be filled
 *  (your own field's boatLives will be set when boats are added)
 * 
 * FieldAI_PlaceAllBoats() should NOT be called in this function.
 * 
 * @param own_field     //A field representing the agents own ships
 * @param opp_field     //A field representing the opponent's ships
 */

void FieldInit(Field *own_field, Field * opp_field)
{
    uint8_t row, col;
    opp_field->smallBoatLives = 3;
    opp_field->mediumBoatLives = 4;
    opp_field->largeBoatLives = 5;
    opp_field->hugeBoatLives = 6;
    own_field->smallBoatLives = 0;
    own_field->mediumBoatLives = 0;
    own_field->largeBoatLives = 0;
    own_field->hugeBoatLives = 0;
    for (row = 0; row < FIELD_ROWS; row++) {
        for (col = 0; col < FIELD_COLS; col++) {
            opp_field->grid[row][col] = FIELD_SQUARE_UNKNOWN;
            own_field->grid[row][col] = FIELD_SQUARE_EMPTY;
        }
    }
}

/**
 * Retrieves the value at the specified field position.
 * @param f     //The Field being referenced
 * @param row   //The row-component of the location to retrieve
 * @param col   //The column-component of the location to retrieve
 * @return  SQUARE_STATUS_INVALID if row and col are not valid field locations
 *          Otherwise, return the status of the referenced square 
 */

SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col)
{
    SquareStatus status;
    if (row >= FIELD_ROWS || col >= FIELD_COLS) {
        status = FIELD_SQUARE_INVALID;
        return status;
    }
    if (f->grid[row][col] == FIELD_SQUARE_EMPTY) {
        status = FIELD_SQUARE_EMPTY;
    } else if (f->grid[row][col] == FIELD_SQUARE_SMALL_BOAT) {
        status = FIELD_SQUARE_SMALL_BOAT;
    } else if (f->grid[row][col] == FIELD_SQUARE_MEDIUM_BOAT) {
        status = FIELD_SQUARE_MEDIUM_BOAT;
    } else if (f->grid[row][col] == FIELD_SQUARE_LARGE_BOAT) {
        status = FIELD_SQUARE_LARGE_BOAT;
    } else if (f->grid[row][col] == FIELD_SQUARE_HUGE_BOAT) {
        status = FIELD_SQUARE_HUGE_BOAT;
    } else if (f->grid[row][col] == FIELD_SQUARE_UNKNOWN) {
        status = FIELD_SQUARE_UNKNOWN;
    } else if (f->grid[row][col] == FIELD_SQUARE_HIT) {
        status = FIELD_SQUARE_HIT;
    } else if (f->grid[row][col] == FIELD_SQUARE_MISS) {
        status = FIELD_SQUARE_MISS;
    } else if (f->grid[row][col] == FIELD_SQUARE_CURSOR) {
        status = FIELD_SQUARE_CURSOR;
    }
    return status;
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_SQUARE_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */

SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p)
{
    // ADDED
    SquareStatus status = FieldGetSquareStatus(f, row, col);
    if (row >= FIELD_ROWS || col >= FIELD_COLS) {
        status = FIELD_SQUARE_INVALID;
        return status;
    }
    if (p == FIELD_SQUARE_EMPTY) {
        f->grid[row][col] = FIELD_SQUARE_EMPTY;
    } else if (p == FIELD_SQUARE_SMALL_BOAT) {
        f->grid[row][col] = FIELD_SQUARE_SMALL_BOAT;
    } else if (p == FIELD_SQUARE_MEDIUM_BOAT) {
        f->grid[row][col] = FIELD_SQUARE_MEDIUM_BOAT;
    } else if (p == FIELD_SQUARE_LARGE_BOAT) {
        f->grid[row][col] = FIELD_SQUARE_LARGE_BOAT;
    } else if (p == FIELD_SQUARE_HUGE_BOAT) {
        f->grid[row][col] = FIELD_SQUARE_HUGE_BOAT;
    } else if (p == FIELD_SQUARE_UNKNOWN) {
        f->grid[row][col] = FIELD_SQUARE_UNKNOWN;
    } else if (p == FIELD_SQUARE_HIT) {
        f->grid[row][col] = FIELD_SQUARE_HIT;
    } else if (p == FIELD_SQUARE_MISS) {
        f->grid[row][col] = FIELD_SQUARE_MISS;
    } else if (p == FIELD_SQUARE_CURSOR) {
        f->grid[row][col] = FIELD_SQUARE_CURSOR;
    }
    return status;
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_SQUARE_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_TYPE_SMALL);
 * }
 *
 * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ 3 3 3 . . . 3 . . . ]
 *  1 [ 4 4 4 4 . . 3 . . . ]
 *  2 [ . . . . . . 3 . . . ]
 *  3 [ . . . . . . . . . . ]
 *  4 [ . . . . . . . . . . ]
 *  5 [ . . . . . . . . . . ]
 *     
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_SQUARE_*_BOAT values from the
 * SquareStatus enum.
 * @return SUCCESS for success, STANDARD_ERROR for failure
 */
uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type)
{
    SquareStatus status;
    uint8_t x, y;
    status = FieldGetSquareStatus(f, row, col);
    if (status == FIELD_SQUARE_SMALL_BOAT || status == FIELD_SQUARE_MEDIUM_BOAT
            || status == FIELD_SQUARE_LARGE_BOAT || status == FIELD_SQUARE_HUGE_BOAT) {
        return STANDARD_ERROR;
    } else if (status == FIELD_SQUARE_EMPTY) {
        if (dir == FIELD_DIR_EAST) {
            if (boat_type == FIELD_BOAT_TYPE_SMALL) {
                for (x = col; x < col + FIELD_BOAT_SIZE_SMALL; x++) {
                    if (x > FIELD_COLS - 1) {
                        for (x--; x >= col; x--) {
                            FieldSetSquareStatus(f, row, x, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    }
                    status = FieldGetSquareStatus(f, row, x);
                    if (status == FIELD_SQUARE_SMALL_BOAT || status == FIELD_SQUARE_MEDIUM_BOAT ||
                            status == FIELD_SQUARE_LARGE_BOAT || status == FIELD_SQUARE_HUGE_BOAT) {
                        for (x--; x >= col; x--) {
                            FieldSetSquareStatus(f, row, x, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    } else {
                        FieldSetSquareStatus(f, row, x, FIELD_SQUARE_SMALL_BOAT);
                    }
                }
                f->smallBoatLives += FIELD_BOAT_SIZE_SMALL;
            } else if (boat_type == FIELD_BOAT_TYPE_MEDIUM) {
                for (x = col; x < col + FIELD_BOAT_SIZE_MEDIUM; x++) {
                    if (x > FIELD_COLS - 1) {
                        for (x--; x >= col; x--) {
                            FieldSetSquareStatus(f, row, x, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    }
                    status = FieldGetSquareStatus(f, row, x);
                    if (status == FIELD_SQUARE_SMALL_BOAT || status == FIELD_SQUARE_MEDIUM_BOAT ||
                            status == FIELD_SQUARE_LARGE_BOAT || status == FIELD_SQUARE_HUGE_BOAT) {
                        for (x--; x >= col; x--) {
                            FieldSetSquareStatus(f, row, x, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    } else {
                        FieldSetSquareStatus(f, row, x, FIELD_SQUARE_MEDIUM_BOAT);
                    }
                }
                f->mediumBoatLives += FIELD_BOAT_SIZE_MEDIUM;
            } else if (boat_type == FIELD_BOAT_TYPE_LARGE) {
                for (x = col; x < col + FIELD_BOAT_SIZE_LARGE; x++) {
                    if (x > FIELD_COLS - 1) {
                        for (x--; x >= col; x--) {
                            FieldSetSquareStatus(f, row, x, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    }
                    status = FieldGetSquareStatus(f, row, x);
                    if (status == FIELD_SQUARE_SMALL_BOAT || status == FIELD_SQUARE_MEDIUM_BOAT ||
                            status == FIELD_SQUARE_LARGE_BOAT || status == FIELD_SQUARE_HUGE_BOAT) {
                        for (x--; x >= col; x--) {
                            FieldSetSquareStatus(f, row, x, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    } else {
                        FieldSetSquareStatus(f, row, x, FIELD_SQUARE_LARGE_BOAT);
                    }
                }
                f->largeBoatLives += FIELD_BOAT_SIZE_LARGE;
            } else if (boat_type == FIELD_BOAT_TYPE_HUGE) {
                for (x = col; x < col + FIELD_BOAT_SIZE_HUGE; x++) {
                    if (x > FIELD_COLS - 1) {
                        for (x--; x >= col; x--) {
                            FieldSetSquareStatus(f, row, x, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    }
                    status = FieldGetSquareStatus(f, row, x);
                    if (status == FIELD_SQUARE_SMALL_BOAT || status == FIELD_SQUARE_MEDIUM_BOAT ||
                            status == FIELD_SQUARE_LARGE_BOAT || status == FIELD_SQUARE_HUGE_BOAT) {
                        for (x--; x >= col; x--) {
                            FieldSetSquareStatus(f, row, x, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    } else {
                        FieldSetSquareStatus(f, row, x, FIELD_SQUARE_HUGE_BOAT);
                    }
                }
                f->hugeBoatLives += FIELD_BOAT_SIZE_HUGE;
            }
        } else if (dir == FIELD_DIR_SOUTH) {
            if (boat_type == FIELD_BOAT_TYPE_SMALL) {
                for (y = row; y < row + FIELD_BOAT_SIZE_SMALL; y++) {
                    if (y > FIELD_ROWS - 1) {
                        for (y--; y >= row; y--) {
                            FieldSetSquareStatus(f, y, col, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    }
                    status = FieldGetSquareStatus(f, y, col);
                    if (status == FIELD_SQUARE_SMALL_BOAT || status == FIELD_SQUARE_MEDIUM_BOAT ||
                            status == FIELD_SQUARE_LARGE_BOAT || status == FIELD_SQUARE_HUGE_BOAT) {
                        for (y--; y >= row; y--) {
                            FieldSetSquareStatus(f, y, col, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    } else {
                        FieldSetSquareStatus(f, y, col, FIELD_SQUARE_SMALL_BOAT);
                    }
                }
                f->smallBoatLives += FIELD_BOAT_SIZE_SMALL;
            } else if (boat_type == FIELD_BOAT_TYPE_MEDIUM) {
                for (y = row; y < row + FIELD_BOAT_SIZE_MEDIUM; y++) {
                    if (y > FIELD_ROWS - 1) {
                        for (y--; y >= row; y--) {
                            FieldSetSquareStatus(f, y, col, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    }
                    status = FieldGetSquareStatus(f, y, col);
                    if (status == FIELD_SQUARE_SMALL_BOAT || status == FIELD_SQUARE_MEDIUM_BOAT ||
                            status == FIELD_SQUARE_LARGE_BOAT || status == FIELD_SQUARE_HUGE_BOAT) {
                        for (y--; y >= row; y--) {
                            FieldSetSquareStatus(f, y, col, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    } else {
                        FieldSetSquareStatus(f, y, col, FIELD_SQUARE_MEDIUM_BOAT);

                    }
                }
                f->mediumBoatLives += FIELD_BOAT_SIZE_MEDIUM;
            } else if (boat_type == FIELD_BOAT_TYPE_LARGE) {
                for (y = row; y < row + FIELD_BOAT_SIZE_LARGE; y++) {
                    if (y > FIELD_ROWS - 1) {
                        for (y--; y >= row; y--) {
                            FieldSetSquareStatus(f, y, col, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    }
                    status = FieldGetSquareStatus(f, y, col);
                    if (status == FIELD_SQUARE_SMALL_BOAT || status == FIELD_SQUARE_MEDIUM_BOAT ||
                            status == FIELD_SQUARE_LARGE_BOAT || status == FIELD_SQUARE_HUGE_BOAT) {
                        for (y--; y >= row; y--) {
                            FieldSetSquareStatus(f, y, col, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    } else {
                        FieldSetSquareStatus(f, y, col, FIELD_SQUARE_LARGE_BOAT);
                    }
                }
                f->largeBoatLives += FIELD_BOAT_SIZE_LARGE;
            } else if (boat_type == FIELD_BOAT_TYPE_HUGE) {
                for (y = row; y < row + FIELD_BOAT_SIZE_HUGE; y++) {
                    if (y > FIELD_ROWS - 1) {
                        for (y--; y >= row; y--) {
                            FieldSetSquareStatus(f, y, col, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    }
                    status = FieldGetSquareStatus(f, y, col);
                    if (status == FIELD_SQUARE_SMALL_BOAT || status == FIELD_SQUARE_MEDIUM_BOAT ||
                            status == FIELD_SQUARE_LARGE_BOAT || status == FIELD_SQUARE_HUGE_BOAT) {
                        for (y--; y >= row; y--) {
                            FieldSetSquareStatus(f, y, col, FIELD_SQUARE_EMPTY);
                        }
                        return STANDARD_ERROR;
                    } else {
                        FieldSetSquareStatus(f, y, col, FIELD_SQUARE_HUGE_BOAT);
                    }
                }
                f->hugeBoatLives += FIELD_BOAT_SIZE_HUGE;
            }
        }
    }
    return SUCCESS;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The result is stored in gData->result as an
 *               output.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
SquareStatus FieldRegisterEnemyAttack(Field *f, GuessData * gData)
{
    SquareStatus status;
    status = FieldGetSquareStatus(f, gData->row, gData->col);
    if (status == FIELD_SQUARE_INVALID) {
        gData->result = RESULT_MISS;
        return status;
    }
    if (status == FIELD_SQUARE_SMALL_BOAT) {
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->smallBoatLives -= 1;
        if (f->smallBoatLives == 0) {
            gData->result = RESULT_SMALL_BOAT_SUNK;
        } else {
            gData->result = RESULT_HIT;
        }
    } else if (status == FIELD_SQUARE_MEDIUM_BOAT) {
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->mediumBoatLives -= 1;
        if (f->mediumBoatLives == 0) {
            gData->result = RESULT_MEDIUM_BOAT_SUNK;
        } else {
            gData->result = RESULT_HIT;
        }
    } else if (status == FIELD_SQUARE_LARGE_BOAT) {
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->largeBoatLives -= 1;
        if (f->largeBoatLives == 0) {
            gData->result = RESULT_LARGE_BOAT_SUNK;
        } else {
            gData->result = RESULT_HIT;
        }
    } else if (status == FIELD_SQUARE_HUGE_BOAT) {
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->hugeBoatLives -= 1;
        if (f->hugeBoatLives == 0) {
            gData->result = RESULT_HUGE_BOAT_SUNK;
        } else {
            gData->result = RESULT_HIT;
        }
    } else {
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_MISS);
        gData->result = RESULT_MISS;
    }
    return status;
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_SQUARE_HIT at that position. If it was a miss, display a FIELD_SQUARE_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * RESULT_*_BOAT_SUNK.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
SquareStatus FieldUpdateKnowledge(Field *f, const GuessData * gData)
{
    SquareStatus status;
    if (gData->result == RESULT_HIT || gData->result == RESULT_SMALL_BOAT_SUNK ||
            gData->result == RESULT_MEDIUM_BOAT_SUNK || gData->result == RESULT_LARGE_BOAT_SUNK ||
            gData->result == RESULT_HUGE_BOAT_SUNK) {
        prevprevguess.row = previousguess.row;
        prevprevguess.col = previousguess.col;
        prevprevguess.result = previousguess.result;
        previousguess.row = gData->row;
        previousguess.col = gData->col;
        previousguess.result = gData->result;
    }
    if (gData->result == RESULT_HIT) {
        status = FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
    } else if (gData->result == RESULT_SMALL_BOAT_SUNK) {
        status = FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->smallBoatLives = 0;
    } else if (gData->result == RESULT_MEDIUM_BOAT_SUNK) {
        status = FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->mediumBoatLives = 0;
    } else if (gData->result == RESULT_LARGE_BOAT_SUNK) {
        status = FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->largeBoatLives = 0;
    } else if (gData->result == RESULT_HUGE_BOAT_SUNK) {
        status = FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->hugeBoatLives = 0;
    } else if (gData->result == RESULT_MISS) {
        status = FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_MISS);
    }
    return status;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field * f)
{
    uint8_t status = 0;
    if (f->hugeBoatLives > 0) {
        status |= FIELD_BOAT_STATUS_HUGE;
    }
    if (f->largeBoatLives > 0) {
        status |= FIELD_BOAT_STATUS_LARGE;
    }
    if (f->mediumBoatLives > 0) {
        status |= FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->smallBoatLives > 0) {
        status |= FIELD_BOAT_STATUS_SMALL;
    }
    return status;
}

/**
 * This function is responsible for placing all the boats on a field.
 * 
 * @param f         //agent's own field, to be modified in place.
 * @return SUCCESS if all boats could be placed, STANDARD_ERROR otherwise.
 * 
 * This function should never fail when passed a properly initialized field!
 */
uint8_t FieldAIPlaceAllBoats(Field * f)
{
    int boats;
    uint8_t row, col, result;
    BoatDirection dir;
    for (boats = 0; boats < 4;) {
        if (f->hugeBoatLives == 0) {
            row = rand() % 6;
            col = rand() % 10;
            dir = rand() % 2;
            result = FieldAddBoat(f, row, col, dir, FIELD_BOAT_TYPE_HUGE);
            if (result == STANDARD_ERROR) {
            } else {
                boats++;
            }
        } else if (f->largeBoatLives == 0) {
            row = rand() % 6;
            col = rand() % 10;
            dir = rand() % 2;
            result = FieldAddBoat(f, row, col, dir, FIELD_BOAT_TYPE_LARGE);
            if (result == STANDARD_ERROR) {
            } else {
                boats++;
            }
        } else if (f->mediumBoatLives == 0) {
            row = rand() % 6;
            col = rand() % 10;
            dir = rand() % 2;
            result = FieldAddBoat(f, row, col, dir, FIELD_BOAT_TYPE_MEDIUM);
            if (result == STANDARD_ERROR) {
            } else {
                boats++;
            }
        } else if (f->smallBoatLives == 0) {
            row = rand() % 6;
            col = rand() % 10;
            dir = rand() % 2;
            result = FieldAddBoat(f, row, col, dir, FIELD_BOAT_TYPE_SMALL);
            if (result == STANDARD_ERROR) {
            } else {
                boats++;
            }
        }
    }
    if (f->smallBoatLives == FIELD_BOAT_SIZE_SMALL
            && f->mediumBoatLives == FIELD_BOAT_SIZE_MEDIUM
            && f->largeBoatLives == FIELD_BOAT_SIZE_LARGE
            && f->hugeBoatLives == FIELD_BOAT_SIZE_HUGE) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * Given a field, decide the next guess.
 *
 * This function should not attempt to shoot a square which has already been guessed.
 *
 * You may wish to give this function static variables.  If so, that data should be
 * reset when FieldInit() is called.
 * 
 * @param f an opponent's field.
 * @return a GuessData struct whose row and col parameters are the coordinates of the guess.  The 
 *           result parameter is irrelevant.
 */
GuessData FieldAIDecideGuess(const Field * f)
{
    uint8_t row, row2, col, col2;
    GuessData guess;
    if (previousguess.result == RESULT_SMALL_BOAT_SUNK ||
            previousguess.result == RESULT_MEDIUM_BOAT_SUNK ||
            previousguess.result == RESULT_LARGE_BOAT_SUNK ||
            previousguess.result == RESULT_HUGE_BOAT_SUNK) {
        previousguess.result = RESULT_MISS;
        prevprevguess.result = RESULT_MISS;
        row = rand() % 6;
        col = rand() % 10;
        for (; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS;) {
            row = rand() % 6;
            col = rand() % 10;
        }
    } else if (previousguess.result == RESULT_HIT && prevprevguess.result == RESULT_HIT) {
        row = previousguess.row;
        col = previousguess.col;
        row2 = prevprevguess.row;
        col2 = prevprevguess.col;
        if (col == col2) {
            if (row > row2) {
                for (row++; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS
                        || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_HIT
                        || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_INVALID; row++) {
                    if (FieldGetSquareStatus(f, row + 1, col) == FIELD_SQUARE_INVALID ||
                            FieldGetSquareStatus(f, row + 1, col) == FIELD_SQUARE_MISS) {
                        row = row2;
                        for (row--; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS
                                || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_HIT
                                || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_INVALID
                                ; row--) {
                        }
                        guess.row = row;
                        guess.col = col;
                        return guess;
                    }
                }
            } else if (row < row2) {
                for (row--; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS
                        || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_HIT
                        || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_INVALID; row--) {
                    if (FieldGetSquareStatus(f, row - 1, col) == FIELD_SQUARE_INVALID ||
                            FieldGetSquareStatus(f, row - 1, col) == FIELD_SQUARE_MISS) {
                        row = row2;
                        for (row++; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS
                                || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_HIT
                                || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_INVALID
                                ; row++) {
                        }
                        guess.row = row;
                        guess.col = col;
                        return guess;
                    }
                }
            }
        } else if (row == row2) {
            if (col > col2) {
                for (col++; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS
                        || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_HIT
                        || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_INVALID; col++) {
                    if (FieldGetSquareStatus(f, row, col + 1) == FIELD_SQUARE_INVALID ||
                            FieldGetSquareStatus(f, row, col + 1) == FIELD_SQUARE_MISS) {
                        col = col2;
                        for (col--; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS
                                || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_HIT
                                || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_INVALID
                                ; col--) {
                        }
                        guess.row = row;
                        guess.col = col;
                        return guess;
                    }
                }
            } else if (col < col2) {
                for (col--; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS
                        || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_HIT
                        || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_INVALID; col--) {
                    if (FieldGetSquareStatus(f, row, col - 1) == FIELD_SQUARE_INVALID ||
                            FieldGetSquareStatus(f, row, col - 1) == FIELD_SQUARE_MISS) {
                        col = col2;
                        for (col++; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS
                                || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_HIT
                                || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_INVALID
                                ; col++) {
                        }
                        guess.row = row;
                        guess.col = col;
                        return guess;
                    }
                }
            }
        }
    } else if (previousguess.result == RESULT_HIT) {
        row = previousguess.row;
        col = previousguess.col;
        if (FieldGetSquareStatus(f, row + 1, col) != FIELD_SQUARE_MISS &&
                FieldGetSquareStatus(f, row + 1, col) != FIELD_SQUARE_INVALID
                && FieldGetSquareStatus(f, row + 1, col) != FIELD_SQUARE_HIT) {
            row = previousguess.row;
            col = previousguess.col;
            row = row + 1;
        } else if (FieldGetSquareStatus(f, row, col + 1) != FIELD_SQUARE_MISS &&
                FieldGetSquareStatus(f, row, col + 1) != FIELD_SQUARE_INVALID
                && FieldGetSquareStatus(f, row, col + 1) != FIELD_SQUARE_HIT) {
            row = previousguess.row;
            col = previousguess.col;
            col = col + 1;
        } else if (FieldGetSquareStatus(f, row - 1, col) != FIELD_SQUARE_MISS &&
                FieldGetSquareStatus(f, row - 1, col) != FIELD_SQUARE_INVALID
                && FieldGetSquareStatus(f, row - 1, col) != FIELD_SQUARE_HIT) {
            row = previousguess.row;
            col = previousguess.col;
            row = row - 1;
        } else if (FieldGetSquareStatus(f, row, col - 1) != FIELD_SQUARE_MISS &&
                FieldGetSquareStatus(f, row, col - 1) != FIELD_SQUARE_INVALID
                && FieldGetSquareStatus(f, row, col - 1) != FIELD_SQUARE_HIT) {
            row = previousguess.row;
            col = previousguess.col;
            col = col - 1;
        }
    } else {
        row = rand() % 6;
        col = rand() % 10;
        for (; FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_MISS
                || FieldGetSquareStatus(f, row, col) == FIELD_SQUARE_HIT;) {
            row = rand() % 6;
            col = rand() % 10;
        }
    }
    guess.row = row;
    guess.col = col;
    return guess;
}