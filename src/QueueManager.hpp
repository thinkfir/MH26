#pragma once
#include "Project.hpp"
#include "AudioEngine.hpp"

namespace pp {

class QueueManager : public QObject {
    Q_Object
private:
    AudioEngine* audio = new AudioEngine(this);
    std::vector<std::string> queue;
    int cur = 0;
public:

    void AddTrack(string path) {
        queue.push_back(path);
    }

    void NextTrack() {
        audio->loadPath(cur++)
        audio->play()
    }

    void PreviousTrack() {
        audio->loadPath(cur--)
        audio->play()
    }

}

}