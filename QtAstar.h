#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtAstar.h"
#include "solution.h"
#include "QThread"
#include <vector>

class QtAstar : public QMainWindow
{
	Q_OBJECT

public:
	QtAstar(QWidget* parent = nullptr);
	~QtAstar();

	void updateAns(int ans);
	signals:
		void startAlg(int idx, std::vector<char>digits);
private:
	QThread* t;
	solution *work;
	Ui::QtAstarClass ui;
};
