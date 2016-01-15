#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time
{
    friend class Scene;
private:
    ///Starting point for program
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

    ///When the last frame start
    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameStartTime;

    ///Time passed to compute the previous frame
    float previousFrameTimeToCompute;

    ///The current time scale for program
    float timeScaleFactor = 1.0f;

protected:
    Time();
    static Time singleton;
    virtual ~Time();
    ///Method called when we compute a new frame
    void newFrame();

public:
    ///Current time scale
    static float timeScale();
    ///Modify time scale
    static void setTimeScale(float timeScale);
    ///Current time
    static float time();
    ///Current time without timescale
    static float fixedTime();
    ///Current delta time (or time for a frame without scaling)
    static float fixedDeltaTime();
    ///How much time passed to compute the previous frame
    static float deltaTime();

};
#endif // TIME_H
