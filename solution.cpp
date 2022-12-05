#include "solution.h"
#include <cctype>
#include <QObject>
#include <unordered_set>
#include <queue>
#include <QDebug>
#include <random>

solution::solution(QObject* parent)
	:QObject(parent)
{
}

solution::~solution()
{
}

void solution::start(int idx, std::vector<char> digits)
{
	std::array<std::array<int, 3>, 3>board;
	//初始化，将char转为3*3格式，方便
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			int num = i * 3 + j;
			if (std::toupper(digits[num]) == 'X')
				board[i][j] = 0;
			else
				board[i][j] = digits[num] - '0';
		}


	if (idx == 0)
		Astar(board);
	else if (idx == 1)
		SimulatedAnnealing(board);
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

//大概是估价函数
int solution::getH(const std::array<std::array<int, 3>, 3>&board)
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

void solution::Astar(const std::array<std::array<int, 3>, 3>&digits)
{
	node now{ .board = digits, .nowStep = 0, .last = -1 }, temp{};

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (std::toupper(digits[i][j]) == 0)
			{
				now.x = j;
				now.y = i;
				break;
			}

	//计算初始条件
	now.f = getH(now.board);
	if (now.f == 0)
	{
		emit astarComputeUpdate(0);
		return;
	}

	std::priority_queue<node>nodes{};
	nodes.emplace(now);
	int nowf = 0;

	while (!nodes.empty()&&running)
	{
		now = nodes.top();
		nodes.pop();
		temp = now;
		++temp.nowStep;
		if(now.f>nowf)
		{
			nowf = now.f;
			emit astarComputeUpdate(-nowf);
		}

		//走步
		for (int i = 0; i <= 3; ++i)
		{
			int xx = now.x + dx[i];
			int yy = now.y + dy[i];

			//超出范围的不走，以及不往回走
			if (xx < 0 || xx >= 3 || yy < 0 || yy >= 3 || lastxy[i] == now.last)
				continue;
			std::swap(temp.board[now.y][now.x], temp.board[yy][xx]);

			if (auto h = getH(temp.board); h != 0)
				temp.f = h + temp.nowStep;
			else
			{
				emit astarComputeUpdate(temp.nowStep);
				return;
			}

			temp.last = i;
			temp.x = xx, temp.y = yy;
			nodes.emplace(temp);

			//可以当做回溯一下
			std::swap(temp.board[now.y][now.x], temp.board[yy][xx]);
		}
	}
}

void solution::SimulatedAnnealing(const std::array<std::array<int, 3>, 3>& digits)
{
	static std::mt19937 mt(std::random_device{}());
	std::uniform_int<int>directRand(0, 3);

	int x, y, xx, yy, del, nowh, direct, ans=0x3F3F3F3F;
	 
	for(int i=0;i<3;++i)
		for(int j=0;j<3;++j)
			if(digits[i][j]==0)
			{
				x = j;
				y = i;
				break;
			}

	int maxRound=3000;
	int originX = x, originY = y;
	if(getH(digits)==0)
	{
		emit SAupdate(maxRound, 0);
		return;
	}

	std::array<std::array<int, 3>, 3>board{};
	for(int i=1;i<=maxRound&&running;++i)
	{
		emit SAupdate(i, -ans);

		int step = 0, best = 0x3F3F3F3F;
		double T = 20;
		board = digits;
		x = originX;
		y = originY;
		while(T>=1e-100)
		{
			++step;

			do//超出范围的不走，以及不往回走，不符合条件就再摇一个方向
				{
				direct = directRand(mt);
				xx = x + dx[direct];
				yy = y + dy[direct];
				} while (xx < 0 || xx >= 3 || yy < 0 || yy >= 3);

			std::swap(board[y][x], board[yy][xx]);

			nowh = getH(board);
			if(nowh==0)
			{
				ans = std::min(ans, step);
				break;
			}

			del = nowh - best;
			if (del < 0)
			{
				best = nowh;
				x = xx;
				y = yy;
			}
			else if (std::generate_canonical<double, 10>(mt) < std::exp(-del / T))//接受新解
			{
				x = xx;
				y = yy;
			}
			else std::swap(board[y][x], board[yy][xx]);

			T *= 0.98;
		}
	}
	if(running)emit SAupdate(maxRound, ans);
}
