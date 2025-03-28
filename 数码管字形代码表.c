#include <stdio.h>

struct Seg
{
	char ch;
	char a, b, c, d, e, f, g, dp;
};

struct Seg segList[] = {
	{' ', 0, 0, 0, 0, 0, 0, 0, 0},
	{'-', 0, 0, 0, 0, 0, 0, 1, 0},
	{'.', 0, 0, 0, 0, 0, 0, 0, 1},
	{'0', 1, 1, 1, 1, 1, 1, 0, 0},
	{'1', 0, 1, 1, 0, 0, 0, 0, 0},
	{'2', 1, 1, 0, 1, 1, 0, 1, 0},
	{'3', 1, 1, 1, 1, 0, 0, 1, 0},
	{'4', 0, 1, 1, 0, 0, 1, 1, 0},
	{'5', 1, 0, 1, 1, 0, 1, 1, 0},
	{'6', 1, 0, 1, 1, 1, 1, 1, 0},
	{'7', 1, 1, 1, 0, 0, 0, 0, 0},
	{'8', 1, 1, 1, 1, 1, 1, 1, 0},
	{'9', 1, 1, 1, 1, 0, 1, 1, 0},
	{'A', 1, 1, 1, 0, 1, 1, 1, 0},
	{'a', 1, 1, 1, 1, 1, 0, 1, 0},
	{'b', 0, 0, 1, 1, 1, 1, 1, 0},
	{'C', 1, 0, 0, 1, 1, 1, 0, 0},
	{'c', 0, 0, 0, 1, 1, 0, 1, 0},
	{'d', 0, 1, 1, 1, 1, 0, 1, 0},
	{'E', 1, 0, 0, 1, 1, 1, 1, 0},
	{'F', 1, 0, 0, 0, 1, 1, 1, 0},
	{'G', 1, 0, 1, 1, 1, 1, 0, 0},
	{'H', 0, 1, 1, 0, 1, 1, 1, 0},
	{'J', 0, 1, 1, 1, 0, 0, 0, 0},
	{'L', 0, 0, 0, 1, 1, 1, 0, 0},
	{'n', 0, 0, 1, 0, 1, 0, 1, 0},
	{'o', 0, 0, 1, 1, 1, 0, 1, 0},
	{'P', 1, 1, 0, 0, 1, 1, 1, 0},
	{'q', 1, 1, 1, 0, 0, 1, 1, 0},
	{'r', 0, 0, 0, 0, 1, 0, 1, 0},
	{'U', 0, 1, 1, 1, 1, 1, 0, 0},
	{'u', 0, 0, 1, 1, 1, 0, 0, 0},
	{'y', 0, 1, 1, 1, 0, 1, 1, 0},
	{'_', 0, 0, 0, 1, 0, 0, 0, 0}
};

struct Seg SearchSeg(char c)
{
	char i, n = sizeof(segList)/sizeof(struct Seg);
	for(i = 0; i < n; i++)
	{
		if(segList[i].ch == c)
		{
			return segList[i];
		}
	}
	return SearchSeg(' ');
}

unsigned char SegCodeCC(char c)//共阴
{
	struct Seg seg = SearchSeg(c);
	unsigned char segCode;
	segCode = seg.dp << 7;
	segCode += seg.g << 6;
	segCode += seg.f << 5;
	segCode += seg.e << 4;
	segCode += seg.d << 3;
	segCode += seg.c << 2;
	segCode += seg.b << 1;
	segCode += seg.a << 0;
	return segCode;
}

unsigned char SegCodeCA(char c)//共阳
{
	return 0xff - SegCodeCC(c);
}

int main()
{
	char i, c, n = sizeof(segList)/sizeof(struct Seg);
	for(i = 0; i < n; i++)
	{
		c = segList[i].ch;
		printf("字符 %c 共阳字形代码 0x%02x 共阴字形代码 0x%02x\n", c, SegCodeCA(c), SegCodeCC(c));
	}
	getchar();
	return 0;
}
