#pragma once
#include "Project.hpp"


namespace pp {


struct Config {
	tx::Coord screenDimension = { 800, 480 };
	float trayPercentage = 0.1f;
};
inline Config config;


class Tray : public QWidget {
	Q_OBJECT

public:
private:
	inline static constexpr const char* sectionNames[] = {
		"Playing",
		"Queue",
		"Collection",
		"Config"
	};
	inline static constexpr const tx::u32 sectionCount = 4;

private:
};

/**
 * handles sections switching
 */
class Content : public QWidget {
	Q_OBJECT

public:
private:
};








class Main : public QWidget {
	Q_OBJECT

public:
	Main(QWidget* parent = nullptr)
	    : QWidget(parent) { init_impl(); }
	~Main() {}

private:
	// components


private:
	// init

	void init_impl() {
		this->resize(config.screenDimension.x, config.screenDimension.y);
	}

private:
};

} // namespace pp