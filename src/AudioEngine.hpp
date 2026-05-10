#pragma once
#include "Project.hpp"

namespace pp {

class AudioEngine : public QObject {
    Q_OBJECT 
private:
    string m_path;
    QMediaPlayer* player = new QMediaPlayer(this);
    QAudioOutput* output = new QAudioOutput(this);
    QMediaMetaData data = player->metaData();


public:

    string path() {
        return m_path;
    }

    void loadPath(string path) {
        player->setSource(
            QUrl::fromLocalFile(QString::fromStdString(path)));
    }

    void setVolume(int volume) {
        output->setVolume(static_cast<float>(volume));
    }

    void play() {
        player->play();
    }

    QMediaMetaData getRawMetadata() {
        return data;
    }

    string getName() {
        return data.value(QMediaMetaData::Title).toString().toStdString();
    }

    string getArtist() {
        return data.value(QMediaMetaData::Author).toString().toStdString();
    }

    int getDuration() {
        return data.value(QMediaMetaData::Duration).toInt();
    }

    QPixmap getCover() {
        return data.value(QMediaMetaData::CoverArtImage).to();
    }

};  
}