#include "sim.h"

#define ZOOM 8
#define ZOOMED_Y_SIZE (SIM_Y_SIZE / ZOOM + (SIM_Y_SIZE % ZOOM != 0))
#define ZOOMED_X_SIZE (SIM_X_SIZE / ZOOM + (SIM_X_SIZE % ZOOM != 0))
#define CENTER_Y (ZOOMED_Y_SIZE / 2)
#define CENTER_X (ZOOMED_X_SIZE / 2)

void app()
{
    int currently_alive[ZOOMED_Y_SIZE][ZOOMED_X_SIZE] = {0};
    int next_alive[ZOOMED_Y_SIZE][ZOOMED_X_SIZE] = {0};

    currently_alive[CENTER_Y - 1][CENTER_X] = 1;
    currently_alive[CENTER_Y - 1][CENTER_X + 1] = 1;
    currently_alive[CENTER_Y][CENTER_X - 1] = 1;
    currently_alive[CENTER_Y][CENTER_X] = 1;
    currently_alive[CENTER_Y + 1][CENTER_X] = 1;

    while (1)
    {
        for (int y = 0; y < ZOOMED_Y_SIZE; ++y)
        {
            for (int x = 0; x < ZOOMED_X_SIZE; ++x)
            {
                int alive_neighbors = 0;
                alive_neighbors += (x > 0) && (y > 0) && (currently_alive[y - 1][x - 1]);
                alive_neighbors += (y > 0) && (currently_alive[y - 1][x]);
                alive_neighbors +=
                    (x < ZOOMED_X_SIZE - 1) && (y > 0) && (currently_alive[y - 1][x + 1]);
                alive_neighbors += (x < ZOOMED_X_SIZE - 1) && (currently_alive[y][x + 1]);
                alive_neighbors += (x < ZOOMED_X_SIZE - 1) && (y < ZOOMED_Y_SIZE - 1) &&
                                   (currently_alive[y + 1][x + 1]);
                alive_neighbors += (y < ZOOMED_Y_SIZE - 1) && (currently_alive[y + 1][x]);
                alive_neighbors +=
                    (x > 0) && (y < ZOOMED_Y_SIZE - 1) && (currently_alive[y + 1][x - 1]);
                alive_neighbors += (x > 0) && (currently_alive[y][x - 1]);

                if (currently_alive[y][x])
                {
                    next_alive[y][x] = alive_neighbors >= 2 && alive_neighbors <= 3;
                }
                else
                {
                    next_alive[y][x] = alive_neighbors == 3;
                }
            }
        }

        for (int y = 0; y < SIM_Y_SIZE; ++y)
        {
            for (int x = 0; x < SIM_X_SIZE; ++x)
            {
                int color = currently_alive[y / ZOOM][x / ZOOM] ? 0xFFFFFFFF : 0xFF000000;
                simPutPixel(x, y, color);
            }
        }
        simFlush();

        for (int y = 0; y < ZOOMED_Y_SIZE; ++y)
        {
            for (int x = 0; x < ZOOMED_X_SIZE; ++x)
            {
                currently_alive[y][x] = next_alive[y][x];
            }
        }
    }
}
