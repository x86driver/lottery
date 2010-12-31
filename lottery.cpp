#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <iterator>

using namespace std;

static int narray[49];

struct Lottery {
	unsigned char n[6];
	unsigned int idx;
};

class Numbers {
public:
	Numbers(unsigned int max) : max(max), idx(0) {
		lotteries.reserve(max);
        	for (int i = 0; i < 49; ++i)
	                narray[i] = i;
	}
	int exist();	//檢查這次產生出來的是否已經存在於陣列, -1 沒有, 其他代表第幾個
	void generate_all();
	void generate_one();
	void show_one() const {
		for (int i = 0; i < 6; ++i)
			printf("%d ", lottery.n[i]);
		printf("\n");
	}
	void show_all() const;
private:
	unsigned int max, idx;
	struct Lottery lottery;
	vector<struct Lottery> lotteries;
};

void Numbers::show_all() const
{
	vector<struct Lottery>::const_iterator it = lotteries.begin();
	for (; it != lotteries.end(); ++it) {
			printf("%d: ", (*it).idx);
		for (int i = 0; i < 6; ++i) {
			printf("%d ", (*it).n[i]);
		}
		printf("\n");
	}
}

void Numbers::generate_all()
{
//	for (unsigned int i = 0; i < max; ++i) {
//		generate_one();
//		if (exist() == -1)
//	}
	generate_one();
	lottery.idx = idx;
	lotteries.push_back(lottery);
}

void Numbers::generate_one()
{
	random_shuffle(&narray[0], &narray[49]);
	for (int i = 0; i < 6; ++i)
		lottery.n[i] = static_cast<unsigned char>(narray[i]);
	sort(&lottery.n[0], &lottery.n[6]);
}

int Numbers::exist()
{
	int index = 0;
	bool exist = true;
	vector<struct Lottery>::iterator it = lotteries.begin();
	for (; it != lotteries.end(); ++it, ++index) {
		exist = true;
		for (int i = 0; i < 6; ++i) {
			if (lottery.n[i] != (*it).n[i]) {
				exist = false;
				break;
			}
		}
		if (exist == true)
			return index;
	}
	return -1;
}

int main(int argc, char **argv)
{
	srand(time(0));
	if (argc < 2) {
		printf("Usage: %s [number]\n", argv[0]);
		exit(1);
	}
	unsigned int max_num = atoi(argv[1]);
	class Numbers num(max_num);

	printf("Generate %d numbers\n", max_num);

	num.generate_all();
	num.show_all();

	return 0;
}

