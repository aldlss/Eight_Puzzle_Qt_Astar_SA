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

	//���̳߳�ʼ�����
	t = new QThread;
	work = new solution;
	// work->setThread(t);
	work->moveToThread(t);
	t->start();
	connect(this, &QtAstar::startAlg, work, &solution::start);
	qRegisterMetaType<std::vector<char>>("std::vector<char>");

	connect(work, &solution::computeUpdate, this, &QtAstar::updateAns);

	//cancel
	connect(ui.pushButton_2, &QPushButton::clicked, [&] {
		ui.pushButton->setEnabled(true);
		ui.pushButton_2->setEnabled(false);
		work->setRunning(false);
		ui.label->setText("");
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
			{
				emit startAlg(0, digits);
			}
			else //�˻�
			{

			}
		}
		else
		{
			ui.label->setText(QString::fromLocal8Bit(R"(�������!���ڷ����зֱ�����"12345678X"!)"));
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
		ui.label->setText(QString::fromLocal8Bit(std::format("ʹ����A*�㷨�����������{}���ڻ�ԭ", ans).c_str()));
		ui.pushButton->setEnabled(true);
		ui.pushButton_2->setEnabled(false);
	}
	else
	{
		ui.label->setText(QString::fromLocal8Bit(std::format("Ԥ��{}��֮�ڵò����𰸣�����Ԥ��{}����", -(ans+1), -ans).c_str()));
	}
}
