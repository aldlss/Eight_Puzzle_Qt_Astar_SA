#include "solution.h"
#include <cctype>
#include <QObject>
#include <unordered_set>
#include <queue>

solution::solution(QObject* parent)
	:QObject(parent)
{
}

solution::~solution()
{
}



bool solution::judge(std::vector<char>& digits)
{
	auto mapp = std::unordered_set<char>{ '1','2','3','4','5','6','6','7','8','X' };
	for (char& q : digits)
	{
		q = std::toupper(q);
		if (auto result = mapp.find(q); result != mapp.end())
			mapp.erase(result);
		else break;
	}
	if (mapp.empty())
		return true;
	return false;
}

int solution::node::getH()
{
	constexpr int xx[] = { 2,0,1,2,0,1,2,0,1 };
	constexpr int yy[] = { 2,0,0,0,1,1,1,2,2 };
	int res = 0;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			int num = board[i][j];
			res += abs(xx[num] - j) + abs(yy[num] - i);
		}
	return res / 2;
	// for (int i = 0; i < 3; ++i)
	// 	for (int j = 0; j < 3; ++j)
	// 		if (board[i][j] == 0)
	// 			return abs(i - 2) + abs(j - 2);
}

bool solution::node::operator<(const node& b) const
{
	return f > b.f;
}

int solution::Astar(const std::vector<char>digits)
{
	node now{ .nowStep = 0 }, temp{};
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			int num = i * 3 + j;
			if (std::toupper(digits[num]) == 'X')
			{
				now.x = j;
				now.y = i;
				now.board[i][j] = 0;
			}
			else
				now.board[i][j] = digits[num] - '0';
		}

	now.f = now.getH();
	if (now.f == 0)
		return 0;
	std::priority_queue<node>nodes{};
	nodes.emplace(now);
	int n = 0;
	while (!nodes.empty())
	{
		now = nodes.top();
		nodes.pop();
		temp = now;
		++temp.nowStep;
		if(temp.nowStep>n)
		{
			n = temp.nowStep;
			emit computeUpdate(-n);
		}

		for (int i = 0; i <= 3; ++i)
		{
			int xx = now.x + dx[i];
			int yy = now.y + dy[i];
			if (xx < 0 || xx >= 3 || yy < 0 || yy >= 3)
				continue;
			std::swap(temp.board[now.y][now.x], temp.board[yy][xx]);

			if (auto h = temp.getH(); h != 0)
				temp.f = h + temp.nowStep;
			else
				return temp.nowStep;
			temp.x = xx, temp.y = yy;
			nodes.emplace(temp);

			std::swap(temp.board[now.y][now.x], temp.board[yy][xx]);
		}
	}
	return -1;
}
