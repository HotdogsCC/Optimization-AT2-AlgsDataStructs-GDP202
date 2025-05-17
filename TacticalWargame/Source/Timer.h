#pragma once

namespace aie {
    class Timer
    {
    private:
        const float duration;
        float current;
    public:
        Timer(float dur);
        bool CheckTimesUp(float dt);
    };

}