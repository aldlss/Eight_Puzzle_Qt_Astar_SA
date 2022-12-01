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

	void start(int idx, std::vector<char>digits);

	void setRunning(bool Running) { running = Running; }

	signals:
		void astarComputeUpdate(int ans);
		void SAupdate(int now,int best);
private:
	void Astar(const std::array<std::array<int, 3>, 3>& digits);
	void SimulatedAnnealing(const std::array<std::array<int, 3>, 3>& digits);

	bool running;
	static constexpr int dx[4] = { 1,-1,0,0 };
	static constexpr int dy[4] = { 0,0,-1,1 };
	static constexpr int lastxy[4] = { 1,0,3,2 };
	int getH(const std::array<std::array<int, 3>, 3>& board);
	struct node
	{
		std::array<std::array<int, 3>, 3>board;
		int nowStep;
		int f;
		int x, y;
		int last;

		//为了实现小根堆，所以实际上写的是大于
		bool operator <(const node& b) const;
	};
};

