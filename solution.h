#pragma once
#include <vector>
#include <array>
#include <QObject>
#include <QThread>
class solution : public QObject
{
	Q_OBJECT

public:
	solution(QObject* parent = nullptr);
	~solution();

	//判断输入是否符合条件
	static bool judge(std::vector<char>& digits);
	void Astar(const std::vector<char>&digits);
	void SimulatedAnnealing(std::vector<char>&digits);
	void start(int idx, std::vector<char>digits);

	void setRunning(bool Running) { running = Running; }

	signals:
		void computeUpdate(int ans);

private:
	bool running;
	static constexpr int dx[4] = { 1,-1,0,0 };
	static constexpr int dy[4] = { 0,0,-1,1 };
	static constexpr int lastxy[4] = { 1,0,3,2 };
	struct node
	{
		std::array<std::array<int, 3>, 3>board;
		int nowStep;
		int f;
		int x, y;
		int last;
		int getH();

		//为了实现小根堆，所以实际上写的是大于
		bool operator <(const node& b) const;
	};
};

