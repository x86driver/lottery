#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <string.h>

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
	                narray[i] = i+1;
	}
	unsigned int exist();
	unsigned int exist(struct Lottery &lot);
	void generate_all();
	void generate_one();
	void show_one(unsigned int index) const {
		printf("%d: ", index);
		for (int i = 0; i < 6; ++i)
			printf("%d ", lotteries[index].n[i]);
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
	FILE *fp = fopen("a.txt", "wb");
	if (!fp) {
		perror("fopen");
		return;
	}
	time_t t = time(0);
	fprintf(fp, "Generate on %s\n", asctime(localtime(&t)));
	vector<struct Lottery>::const_iterator it = lotteries.begin();
	for (; it != lotteries.end(); ++it) {
			fprintf(fp, "%d: ", (*it).idx);
		for (int i = 0; i < 6; ++i) {
			fprintf(fp, "%d ", (*it).n[i]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void Numbers::generate_all()
{
	int progress = 0;
	const double factor = 10000.0;
	int div = max / (100 * factor);
	for (unsigned int i = 0; i < max; ++i) {
		if ((i % div) == 0) {
			++progress;
			printf("\rGenerating... %.4f %%", (double)(progress/factor));
			fflush(NULL);
		}
regen:
		generate_one();
		unsigned int e = exist();
		if (e == ~1U) {
			lottery.idx = idx++;
			lotteries.push_back(lottery);
		} else {
			goto regen;
//			printf("the same with %d,\t", e);
//			show_one(e);
		}
	}
	printf("\nDone.\n");
}

void Numbers::generate_one()
{
	random_shuffle(&narray[0], &narray[49]);
	for (int i = 0; i < 6; ++i)
		lottery.n[i] = static_cast<unsigned char>(narray[i]);
	sort(&lottery.n[0], &lottery.n[6]);
}

unsigned int Numbers::exist(struct Lottery &lot)
{
	struct Lottery back;
	memcpy(&back, &lottery, sizeof(struct Lottery));
	memcpy(&lottery, &lot, sizeof(struct Lottery));
	unsigned int i = exist();
	memcpy(&lottery, &back, sizeof(struct Lottery));
	return i;
}

unsigned int Numbers::exist()
{
	bool exist = true;
	vector<struct Lottery>::iterator it = lotteries.begin();
	for (; it != lotteries.end(); ++it) {
		exist = true;
		for (int i = 0; i < 6; ++i) {
			if (lottery.n[i] != (*it).n[i]) {
				exist = false;
				break;
			}
		}
		if (exist == true)
			return (*it).idx;
	}
	return ~1U;
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

	struct Lottery lot = {
		{2, 13, 17, 27, 31, 36},
	};

	printf("Generate %d numbers\n", max_num);

	num.generate_all();
	num.show_all();
	unsigned int bingo = num.exist(lot);
	if (bingo != ~1U)
		printf("Win on %u!!", bingo);
	else
		printf("Lose!!\n");
	return 0;
}

