#include "Project.hpp"
#include "main_class.hpp"
#include "env.hpp"

// 800x480

int main(int argc, char* argv[]) {
	std::cout << "PiPod starting...\n";
	loadEnvFile(".env");
	QApplication app(argc, argv);

	pp::Main window;
	window.show();
	//window.showFullScreen();
	return app.exec();
}
