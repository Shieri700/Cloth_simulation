#include <QtWidgets>
#include <string>
#include "mainWindow.h"

#include "glWidget.h"

using namespace std;

MainWindow::MainWindow()
{
	task1Widget = new GlWidget(this);
	creatMenu();
	creattaskGroupBox();

	connect(task3Button, SIGNAL(released()), this, SLOT(task3ButtonPress()));
	connect(fix_points_checkBox, SIGNAL(stateChanged(int)), this, SLOT(fix_points_checkBox_Changes(int)));
	connect(ball_checkBox, SIGNAL(stateChanged(int)), this, SLOT(ball_checkBox_Changes(int)));
	connect(wind_checkBox, SIGNAL(stateChanged(int)), this, SLOT(wind_checkBox_Changes(int)));

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->setMenuBar(menuBar);
	mainLayout->addWidget(task1Widget, 8);
	mainLayout->addWidget(taskGroupBox, 1);
	setLayout(mainLayout);
}

void MainWindow::saveFile() {
	std::string filename = "savedFile.obj";
	task1Widget->saveFile(filename);
}

void MainWindow::creatMenu() {
	menuBar = new QMenuBar;
	fileMenu = new QMenu(tr("&File"), this);
	menuBar->addMenu(fileMenu);
	QAction* saveFileAct = new QAction(tr("&save"), this);
	connect(saveFileAct, &QAction::triggered, this, &MainWindow::saveFile);
	fileMenu->addAction(saveFileAct);
}

void MainWindow::creattaskGroupBox() {
	taskGroupBox = new QGroupBox(tr("Select task"));
	QVBoxLayout* layout = new QVBoxLayout;
	task3Button = new QPushButton(tr("start / stop"));
	
	fix_points_checkBox = new QCheckBox(tr("fix two points"));
	ball_checkBox = new QCheckBox(tr("ball"));
	wind_checkBox = new QCheckBox(tr("wind"));

	layout->addWidget(task3Button);
	layout->addWidget(fix_points_checkBox);
	layout->addWidget(ball_checkBox);
	layout->addWidget(wind_checkBox);
	taskGroupBox->setLayout(layout);
}

void MainWindow::task3ButtonPress() { task1Widget->on_animation = !task1Widget->on_animation; }

void MainWindow::fix_points_checkBox_Changes(int state) {
	if (state == Qt::Unchecked) {
		task1Widget->fixPoints(false);
	}
	else {
		task1Widget->fixPoints(true);
	}
}

void MainWindow::ball_checkBox_Changes(int state) {
	if (state == Qt::Unchecked) {
		task1Widget->ballChanges(false);
	}
	else {
		task1Widget->ballChanges(true);
	}
}

void MainWindow::wind_checkBox_Changes(int state) {
	if (state == Qt::Unchecked) {
		task1Widget->windChanges(false);
	}
	else {
		task1Widget->windChanges(true);
	}
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Escape)
		close();
	else
		task1Widget->keyPressEvent(e);
}