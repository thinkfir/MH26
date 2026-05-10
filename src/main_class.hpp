#pragma once
#include "Project.hpp"
#include "youtube_api.hpp"

namespace pp {

class Clickable : public QWidget {
	Q_OBJECT

public:
	Clickable(QWidget* parent = nullptr)
	    : QWidget(parent) {}
	~Clickable() {}

signals:
	void clicked();

protected:
	void paintEvent(QPaintEvent*) override {
		QStyleOption opt;
		opt.initFrom(this);
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing);
		this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

private:
	void mouseReleaseEvent(QMouseEvent* event) override {
		if (event->button() == Qt::LeftButton) {
			emit clicked();
		}
		QWidget::mouseReleaseEvent(event);
	}
};

class Button : public Clickable {
	Q_OBJECT

public:
	Button(std::string_view name, QWidget* parent = nullptr)
	    : Clickable(parent) { init_impl(name); }
	~Button() {}

	void syncLabel() {
		m_label->resize(this->size());
	}

protected:
	void paintEvent(QPaintEvent*) override {
		QStyleOption opt;
		opt.initFrom(this);
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing);
		this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

private:
	QLabel* m_label;

private:
	void init_impl(std::string_view name) {
		m_label = new QLabel(this);
		m_label->resize(this->size());
		m_label->setText(QString::fromUtf8(name.data(), name.size()));
		m_label->setAlignment(Qt::AlignCenter);
	}
};






class Tray : public QWidget {
	Q_OBJECT

public:
	Tray(QWidget* parent = nullptr)
	    : QWidget(parent) { init_impl(); }
	~Tray() {}

signals:
	void sectionChange(tx::u32 old, tx::u32 newSection);

protected:
	void paintEvent(QPaintEvent*) override {
		QStyleOption opt;
		opt.initFrom(this);
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing);
		this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

private:
	inline static constexpr const char* sectionNames[] = {
		"Playing",
		"Queue",
		"Collection",
		"Config"
	};
	inline static constexpr const tx::u32 sectionCount = 4;
	/**
	 * each section is a button
	 */

private:
	void init_impl() {
		Config::Tray cfg = config.tray;

		tx::u32 height = std::round((float)config.main.screenDimension.y * config.tray.height);

		m_margin = std::round((float)height * cfg.margin);
		m_padding = std::round((float)height * cfg.padding);
		m_buttonWidth = std::round((float)height * cfg.buttonWidth);
		m_buttonHeight = std::round(height - 2 * m_margin);
		m_buttonPadding = std::round(m_buttonWidth * cfg.buttonPadding);

		tx::u32 width = m_padding * 2 + sectionCount * (m_buttonPadding + m_buttonWidth) - m_buttonPadding;

		tx::u32 x = std::round((config.main.screenDimension.x - width) * 0.5f) - m_margin;
		tx::u32 y = m_margin;


		this->resize(width, height);
		this->move(x, y);

		m_focusAnim = new QFrame(this);
		m_focusAnim->setStyleSheet("background-color: rgba(75, 150, 150, 255); border-radius: 15px;");
		m_focusAnim->resize(m_buttonWidth, m_buttonHeight);
		m_focusAnim->move(m_padding, m_padding);

		for (tx::u32 i = 0; i < sectionCount; i++) {
			Button*& button = m_sectionButtons[i];
			button = new Button(sectionNames[i], this);
			button->resize(m_buttonWidth, m_buttonHeight);
			button->move(
			    findSectionPos_impl(i),
			    m_padding);
			button->syncLabel();

			QObject::connect(button, &Button::clicked, [i, this]() {
				emit sectionChange(currentSection, i);
				sectionChangeAnim(currentSection, i);
				currentSection = i;
			});

			button->setStyleSheet("background-color: rgba(0, 170, 170, 0); border-radius: 15px; color: black");
		}

		this->setStyleSheet("background-color: rgba(200, 255, 255, 175); border-radius: 20px;");
	}

private:
	std::array<Button*, sectionCount> m_sectionButtons;
	QFrame* m_focusAnim;
	tx::u32 currentSection = 0;

	tx::u32 m_margin;
	tx::u32 m_padding;
	tx::u32 m_buttonWidth;
	tx::u32 m_buttonHeight;
	tx::u32 m_buttonPadding;

	void sectionChangeAnim(tx::u32 from, tx::u32 to) {
		tx::u32 left = std::min(from, to),
		        right = std::max(from, to),
		        leftPos = findSectionPos_impl(left),
		        rightPos = findSectionPos_impl(right),
		        width = rightPos + m_buttonWidth - leftPos,
		        toPos = findSectionPos_impl(to);

		QPropertyAnimation* stage1 = new QPropertyAnimation(m_focusAnim, "geometry");
		stage1->setEndValue(QRect(leftPos, m_padding, width, m_buttonHeight));
		stage1->setDuration(100);
		stage1->setEasingCurve(QEasingCurve::InCubic);

		QPropertyAnimation* stage2 = new QPropertyAnimation(m_focusAnim, "geometry");
		stage2->setEndValue(QRect(toPos, m_padding, m_buttonWidth, m_buttonHeight));
		stage2->setDuration(100);
		stage1->setEasingCurve(QEasingCurve::OutCubic);

		QSequentialAnimationGroup* sequence = new QSequentialAnimationGroup(this);
		sequence->addAnimation(stage1);
		sequence->addAnimation(stage2);

		sequence->start(QAbstractAnimation::DeleteWhenStopped);
	}

	// helpers

	tx::u32 findSectionPos_impl(tx::u32 sectionIndex) {
		return sectionIndex * (m_buttonWidth + m_buttonPadding) + m_padding;
	}
};


class Content_Playing : public QWidget {
	Q_OBJECT

public:
	Content_Playing(QWidget* parent = nullptr)
	    : QWidget(parent) { init_impl(); }
	~Content_Playing() {}

protected:
	void paintEvent(QPaintEvent*) override {
		QStyleOption opt;
		opt.initFrom(this);
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing);
		this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

private:
	void init_impl() {
		m_label = new QLabel(this);
		m_label->setText("Playing");
	}

private:
	QLabel* m_label;
};

class Content_Queue : public QWidget {
	Q_OBJECT

public:
	Content_Queue(QWidget* parent = nullptr)
	    : QWidget(parent) { init_impl(); }
	~Content_Queue() {}

protected:
	void paintEvent(QPaintEvent*) override {
		QStyleOption opt;
		opt.initFrom(this);
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing);
		this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

private:
	void init_impl() {
		m_label = new QLabel(this);
		m_label->setText("Queue");
	}

private:
	QLabel* m_label;
};

class Content_Collection : public QWidget {
	Q_OBJECT

public:
	Content_Collection(QWidget* parent = nullptr)
	    : QWidget(parent) { init_impl(); }
	~Content_Collection() {}

protected:
	void paintEvent(QPaintEvent*) override {
		QStyleOption opt;
		opt.initFrom(this);
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing);
		this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

private:
	void init_impl() {
		m_label = new QLabel(this);
		m_label->setText("Collection");
	}

private:
	QLabel* m_label;
};

class Content_Config : public QWidget {
	Q_OBJECT

public:
	Content_Config(QWidget* parent = nullptr)
	    : QWidget(parent) { init_impl(); }
	~Content_Config() {}

protected:
	void paintEvent(QPaintEvent*) override {
		QStyleOption opt;
		opt.initFrom(this);
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing);
		this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

private:
	void init_impl() {
		m_label = new QLabel(this);
		m_label->setText("Config");
	}

private:
	QLabel* m_label;
};


/**
 * handles sections switching
 */
class Content {
public:
	Content(QWidget* parent) : m_parent(parent) { init_impl(); }
	~Content() {}

	void changeSection(tx::u32 oldSection, tx::u32 newSection) {
		changeSection_impl(oldSection, newSection);
	}

private:
	void init_impl() {
		Config::Content cfg = config.content;

		m_dimension.y = std::round((float)config.main.screenDimension.y * config.content.height);
		m_margin = std::round((float)m_dimension.y * cfg.margin);
		m_dimension.x = config.main.screenDimension.x - m_margin * 2;

		m_pos.x = m_margin;
		m_pos.y = m_margin + std::round((float)config.main.screenDimension.y * config.tray.height);

		m_sectionPlaying = new Content_Playing(m_parent);
		m_sectionPlaying->resize(m_dimension.x, m_dimension.y);
		m_sectionPlaying->move(m_pos.x, m_pos.y);
		m_sectionQueue = new Content_Queue(m_parent);
		m_sectionQueue->resize(m_dimension.x, m_dimension.y);
		m_sectionQueue->move(m_pos.x, m_pos.y);
		m_sectionQueue->hide();
		m_sectionCollection = new Content_Collection(m_parent);
		m_sectionCollection->resize(m_dimension.x, m_dimension.y);
		m_sectionCollection->move(m_pos.x, m_pos.y);
		m_sectionCollection->hide();
		m_sectionConfig = new Content_Config(m_parent);
		m_sectionConfig->resize(m_dimension.x, m_dimension.y);
		m_sectionConfig->move(m_pos.x, m_pos.y);
		m_sectionConfig->hide();
	}

private:
	tx::Coord m_pos;
	tx::Coord m_dimension;
	tx::u32 m_margin;

	QWidget* m_parent;

	Content_Playing* m_sectionPlaying;
	Content_Queue* m_sectionQueue;
	Content_Collection* m_sectionCollection;
	Content_Config* m_sectionConfig;


	void changeSection_impl(tx::u32 oldSection, tx::u32 newSection) {
		if (newSection == oldSection) return;
		//QMessageBox::information(m_parent, "", QString("changing section into %1").arg(newSection));

		applySection_impl(oldSection, [&](QWidget* section) {
			section->hide();
		});
		applySection_impl(newSection, [&](QWidget* section) {
			section->show();
		});
		oldSection = newSection;
	}


	template <class Func>
	void applySection_impl(tx::u32 sectionIndex, Func&& f) {
		switch (sectionIndex) {
		case 0:
			f(m_sectionPlaying);
			break;
		case 1:
			f(m_sectionQueue);
			break;
		case 2:
			f(m_sectionCollection);
			break;
		case 3:
			f(m_sectionConfig);
			break;
		}
	}
};








class Main : public QWidget {
	Q_OBJECT

public:
	Main(QWidget* parent = nullptr)
	    : QWidget(parent) { init_impl(); }
	~Main() {}

private:
	// components

	QLabel* m_windowBox;
	QGraphicsBlurEffect* m_blurEffect;

	Tray* m_tray;
	Content* m_content;

private:
	// init

	void init_impl() {
		this->resize(
		    config.main.screenDimension.x,
		    config.main.screenDimension.y);

		m_windowBox = new QLabel(this);

		//m_windowBox->setStyleSheet("background-color: #00FF00");
		m_windowBox->resize(
		    config.main.screenDimension.x,
		    config.main.screenDimension.y);
		m_windowBox->lower();

		QPixmap backgroundPix((tx::getExeDir() / "assets/chen.jpg").c_str());
		backgroundPix = backgroundPix.scaled(800, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		m_windowBox->setPixmap(backgroundPix);

		// Inside your init_impl or constructor
		m_blurEffect = new QGraphicsBlurEffect(this);
		m_blurEffect->setBlurRadius(10.0);
		m_blurEffect->setBlurHints(QGraphicsBlurEffect::AnimationHint);
		m_windowBox->setGraphicsEffect(m_blurEffect);

		m_tray = new Tray(this);
		m_content = new Content(this);
		QObject::connect(m_tray, &Tray::sectionChange, [this](tx::u32 oldSection, tx::u32 newSection) {
			m_content->changeSection(oldSection, newSection);
		});
	}

private:
};

} // namespace pp
