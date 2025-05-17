#include "Timer.h"


aie::Timer::Timer(float dur)
    : duration{ dur }, current{ dur } {
}

bool aie::Timer::CheckTimesUp(float dt) {
    current -= dt;
    while (current < 0) {
        current += duration;
        return true;
    }
    return false;
}
