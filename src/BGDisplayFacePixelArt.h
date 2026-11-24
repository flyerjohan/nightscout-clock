#ifndef BGDISPLAYFACEPIXELART_H
#define BGDISPLAYFACEPIXELART_H

#include "DisplayManager.h"
#include "globals.h"

// Small helpers for faces drawn from text pixel art, one character per pixel and '.' for none.
namespace PixelArt {

// Draws rows of art at (x, y) in one color, skipping pixels outside the panel.
inline void draw(const char* const* rows, int height, int x, int y, uint16_t color) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; rows[row][col] != '\0'; col++) {
            const int px = x + col;
            const int py = y + row;
            if (rows[row][col] != '.' && px >= 0 && px < MATRIX_WIDTH && py >= 0 && py < MATRIX_HEIGHT) {
                DisplayManager.drawPixel(px, py, color);
            }
        }
    }
}

// Draws the art mirrored left to right.
inline void drawMirrored(const char* const* rows, int height, int width, int x, int y, uint16_t color) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            const int px = x + width - 1 - col;
            const int py = y + row;
            if (rows[row][col] != '.' && px >= 0 && px < MATRIX_WIDTH && py >= 0 && py < MATRIX_HEIGHT) {
                DisplayManager.drawPixel(px, py, color);
            }
        }
    }
}

// A line along the bottom row from x to the right edge, in the glucose range color, for faces whose
// main content is not the glucose.
inline void glucoseLine(int x, uint16_t color) {
    for (int px = x; px < MATRIX_WIDTH; px++) {
        DisplayManager.drawPixel(px, MATRIX_HEIGHT - 1, color);
    }
}

}  // namespace PixelArt

#endif  // BGDISPLAYFACEPIXELART_H
