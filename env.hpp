#pragma once
#include "Project.hpp"

inline void loadEnvFile(const QString& path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    while (!in.atEnd()) {

        QString line = in.readLine().trimmed();

        if (line.isEmpty() || line.startsWith("#"))
            continue;

        QStringList parts = line.split("=");

        if (parts.size() < 2)
            continue;

        QString key = parts[0].trimmed();
        QString value = parts.mid(1).join("=").trimmed();

        qputenv(
            key.toUtf8(),
            value.toUtf8()
        );
    }
}
