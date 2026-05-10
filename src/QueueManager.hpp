#pragma once
#include "Project.hpp"
#include "AudioEngine.hpp"

namespace pp {

class QueueManager : public QObject {
private:
    AudioEngine* audio = new AudioEngine(this);
    vector<string> queue;
    int cur = 0;
public:

    void NextTrack() {
        audio->loadPath(cur++)
    }

    void PreviousTrack() {
        audio->loadPath(cur--)
    }

}

}