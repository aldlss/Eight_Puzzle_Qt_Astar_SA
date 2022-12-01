#include "QtAstar.h"
#include "QToolButton"
#include "QThread"
#include <vector>
#include "solution.h"
#include "QMetaType"

QtAstar::QtAstar(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.label->setText("");
	ui.pushButton_2->setEnabled(false);

	//多线程初始化相关
	t = new QThread;
	work = new solution;
	// work->setThread(t);
	work->moveToThread(t);
	t->start();
	connect(this, &QtAstar::startAlg, work, &solution::start);
	qRegisterMetaType<std::vector<char>>("std::vector<char>");

	connect(work, &solution::astarComputeUpdate, this, &QtAstar::updateAns);
	connect(work, &solution::SAupdate,this, [&](int now,int ans) {
		if(ans>=0)
		{
			ui.label->setText(QString::fromLocal8Bit(std::format("共进行{}次模拟退火，最佳能用{}步还原", now, ans!=0x3F3F3F3F?(std::to_string(ans)).c_str():"INF").c_str()));
			ui.pushButton->setEnabled(true);
			ui.pushButton_2->setEnabled(false);
		}
		else
		{
			ans = std::abs(ans);
			ui.label->setText(QString::fromLocal8Bit(std::format("正在进行第{}模拟次退火，之前最佳为{}步还原", now, ans != 0x3F3F3F3F ? (std::to_string(ans)).c_str() : "INF").c_str()));
		}
	});

	//cancel
	connect(ui.pushButton_2, &QPushButton::clicked, [&] {
		ui.pushButton->setEnabled(true);
		ui.pushButton_2->setEnabled(false);
		work->setRunning(false);
		// ui.label->setText("");
	});

	//start
	connect(ui.pushButton, &QToolButton::clicked, [&] {
		std::vector<char> digits = {};
		for (int i = 1; i <= 9; ++i)
		{
			auto name = std::format("lineEdit_{}", i);
			auto qLineEdie = this->findChild<QLineEdit*>(QString::fromStdString(name));
			digits.emplace_back(qLineEdie->text().toStdString()[0]);
		}

		if (solution::judge(digits))
		{
			work->setRunning(true);
			ui.pushButton->setEnabled(false);
			ui.pushButton_2->setEnabled(true);

			if (ui.radioButton->isChecked()) //A*
				emit startAlg(0, digits);
			else //退火
				emit startAlg(1, digits);
		}
		else
		{
			ui.label->setText(QString::fromLocal8Bit(R"(输入错误!请在方框中分别输入"12345678X"!)"));
		}
	});
}

QtAstar::~QtAstar()
{
	work->setRunning(false);
	t->quit();
	t->wait();
	delete t;
	delete work;
}

void QtAstar::updateAns(int ans)
{
	if(ans>=0)
	{
		ui.label->setText(QString::fromLocal8Bit(std::format("使用了A*算法，算出最快可在{}步内还原", ans).c_str()));
		ui.pushButton->setEnabled(true);
		ui.pushButton_2->setEnabled(false);
	}
	else
	{
		ui.label->setText(QString::fromLocal8Bit(std::format("预估{}步之内得不出答案，正在预估{}步内", -(ans+1), -ans).c_str()));
	}
}
