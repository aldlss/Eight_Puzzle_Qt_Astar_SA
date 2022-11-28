#pragma once
#include <vector>
#include <array>
#include <QObject>
class solution : public QObject
{
	Q_OBJECT

public:
	solution(QObject* parent = nullptr);
	~solution();

	//�ж������Ƿ��������
	static bool judge(std::vector<char>& digits);
	int Astar(const std::vector<char>digits);
	int SimulatedAnnealing(std::vector<char>digits);
	signals:
		void computeUpdate(int ans);
private:
	static constexpr int dx[4] = { 1,-1,0,0 };
	static constexpr int dy[4] = { 0,0,-1,1 };
	struct node
	{
		std::array<std::array<int, 3>, 3>board;
		int nowStep;
		int f;
		int x, y;
		int getH();

		//Ϊ��ʵ��С���ѣ�����ʵ����д���Ǵ���
		bool operator <(const node& b) const;
	};
};

