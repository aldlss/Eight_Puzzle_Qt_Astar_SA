#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtAstar.h"

class QtAstar : public QMainWindow
{
	Q_OBJECT

public:
	QtAstar(QWidget* parent = nullptr);
	~QtAstar();

	void updateAns(int ans);

private:
	Ui::QtAstarClass ui;
};
