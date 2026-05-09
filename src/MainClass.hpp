#pragma once
#include "Project.hpp"

class Main : public QWidget {
	Q_OBJECT

public:
	Main(QWidget* parent = nullptr)
	    : QWidget(parent) { init_impl(); }
	~Main() {}

private:
	void init_impl() {
	}
};