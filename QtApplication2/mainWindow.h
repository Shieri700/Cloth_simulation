#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QStringList>
#include <QCheckBox>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMenuBar;
class QGroupBox;
class QPushButton;
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE
class GlWidget;

class MainWindow : public QDialog {
	Q_OBJECT

public:
	MainWindow();
	void saveFile();

protected:
	void keyPressEvent(QKeyEvent* event);
	void creatMenu();
	void creattaskGroupBox();

private slots:
	void task3ButtonPress();
	void fix_points_checkBox_Changes(int state);
	void ball_checkBox_Changes(int state);
	void wind_checkBox_Changes(int state);

private:
	GlWidget* task1Widget;
	QMenuBar* menuBar;
	QMenu* fileMenu;
	QGroupBox* taskGroupBox;

	QPushButton* task3Button;
	QCheckBox* fix_points_checkBox;
	QCheckBox* ball_checkBox;
	QCheckBox* wind_checkBox;
};
#endif

