#pragma once
#include "Project.hpp"

namespace pp {

class YoutubeAPI : public QObject {
    Q_OBJECT

public:

    YoutubeAPI(QObject* parent = nullptr)
        : QObject(parent)
    {
        manager = new QNetworkAccessManager(this);
    }

    void search(QString query)
    {
        QString apiKey = "YOUR_API_KEY";

        QString url =
            "https://www.googleapis.com/youtube/v3/search?"
            "part=snippet"
            "&type=video"
            "&videoCategoryId=10"
            "&maxResults=5"
            "&fields=items(id/videoId,snippet/title)"
            "&q=" + query +
            "&key=" + apiKey;

        QNetworkRequest request(url);

        QNetworkReply* reply = manager->get(request);

        connect(reply, &QNetworkReply::finished,
        [reply]() {

            QByteArray data = reply->readAll();

            qDebug() << data;

            QJsonDocument doc =
                QJsonDocument::fromJson(data);

            QJsonObject root = doc.object();

            QJsonArray items =
                root["items"].toArray();

            for (const QJsonValue& item : items) {

                QJsonObject obj = item.toObject();

                QString title =
                    obj["snippet"]
                    .toObject()["title"]
                    .toString();

                QString videoId =
                    obj["id"]
                    .toObject()["videoId"]
                    .toString();

                qDebug() << "TITLE:" << title;
                qDebug() << "VIDEO ID:" << videoId;
            }

            reply->deleteLater();
        });
    }

private:
    QNetworkAccessManager* manager;
};

}
