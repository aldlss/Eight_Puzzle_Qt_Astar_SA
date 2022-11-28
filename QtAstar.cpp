#include "QtAstar.h"
#include "QToolButton"
#include "QThread"
#include <vector>
#include "solution.h"

QtAstar::QtAstar(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.label->setText("");
	ui.pushButton_2->setEnabled(false);
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
			ui.pushButton->setEnabled(false);
			ui.pushButton_2->setEnabled(true);
			solution a;
			QThread t;
			a.moveToThread(&t);
			if (ui.radioButton->isChecked()) //A*
			{
				connect(&a, &solution::computeUpdate, this, &QtAstar::updateAns);
				t.start();
				a.Astar(digits);
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
{}

void QtAstar::updateAns(int ans)
{
	if(ans>=0)
	{
		ui.label->setText(QString::fromLocal8Bit(std::format("ʹ����A*�㷨�����������{}���ڻ�ԭ", ans).c_str()));
		ui.pushButton->setEnabled(true);
	}
	else
	{
		ui.label->setText(QString::fromLocal8Bit(std::format("{}��֮��δ������������ڳ��Ե�{}��", -ans+1,-ans).c_str()));
	}
}
