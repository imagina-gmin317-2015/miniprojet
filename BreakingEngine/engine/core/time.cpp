#include "time.h"
#include <chrono>
#include <QtCore>


Time Time::singleton;

Time::Time()
{
    startTime = std::chrono::high_resolution_clock::now();
    lastFrameStartTime = startTime;
    previousFrameTimeToCompute = 0;
    timeScaleFactor = 1.0f;
}


Time::~Time() {

}



void Time::newFrame()
{
    ///Getting a time point
    auto current = std::chrono::high_resolution_clock::now();
    ///Getting duration between two points in float
    std::chrono::duration<float> elapsed_seconds = current - lastFrameStartTime;
    ///Extracting in float
    previousFrameTimeToCompute =  elapsed_seconds.count();
    ///Setting new timestamp for frame' start
    lastFrameStartTime = current;

}

void Time::setTimeScale(float timeScale)
{
    Time::singleton.timeScaleFactor = timeScale;
}

float Time::time()
{
    return fixedTime();
}

float Time::fixedTime() {
    std::chrono::duration<float> elapsed_seconds = singleton.lastFrameStartTime - singleton.startTime;
    return elapsed_seconds.count();
}

float Time::deltaTime() {

    return timeScale() * singleton.previousFrameTimeToCompute;
}

float Time::timeScale() {
 return Time::singleton.timeScaleFactor;
}


float Time::fixedDeltaTime() {
   return singleton.previousFrameTimeToCompute;
}
