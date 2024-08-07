#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

#define QR_PIXEL_SIZE 7
#define QR_VERSION 6
// actual maximum capacity is 136
#define QR_CAPACITY 128

// this is about 40 seconds
#define TIMER_DURATION 4096
#define TIMER_BAR_HEIGHT 28
#define TIMER_BAR_THICKNESS 4

#define IDLE_TEXT F("SOS")
#define IDLE_TEXT_VELOCITY 2

#endif