#pragma once
#include "tx/math.h"
#include "tx/utility.h"
#include "tx/data.h"
#include "tx/resource.h"
#include "tx/json.h"
#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace std {
using bitSpan = span<tx::u8>;
}

// main include
#include <QtWidgets/QApplication>

#include <QDebug>

// UI library
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QTimer>
#include <QProgressBar>
#include <QSlider>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>

#include <QFileDialog>

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QPainter>
#include <QGraphicsBlurEffect>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>

#include <QStringListModel>
#include <QListView>
#include <QTreeView>
#include <QVariant>

#include <QPoint>

#include <QEvent>
#include <QKeyEvent>

// Helpers
#include <QDir>
#include <QUrl>
#include <QFile>
#include <QPixmap>
#include <QMovie>
#include <QImage>
#include <QIcon>
#include <QFont>
#include <QLocale>

// Audio
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaMetaData>


namespace pp {

struct Config {
	struct General {
		tx::Coord screenDimension = { 800, 480 };
	} main;
	struct Tray {
		float height = 0.1f; // percentage of the entire screen
		float margin = 0.1f; // percentage of tray height
		float padding = 0.1f; // percentage of tray height, padding between button and border
		float buttonWidth = 3.0f; // percentage of tray height
		float buttonPadding = 0.1f; // percentage of button width, padding between buttons
	} tray;
	struct Content {
		float height = 0.9f; // percentage of the entire screen
		float margin = 0.1f; // percentage of tray height
	} content;
};
constexpr const Config config;
}