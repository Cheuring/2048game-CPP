#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<graphics.h>
#include<ctime>
#include<list>
#include<conio.h>

//有效移动
bool validMove;

//分数
char putScore[100];

using namespace std;
//定义格子类
class block {
public:
	int pos;
	int data;
	block() {
		pos = -1;
		data = 0;
	}
	block(int pos, int data) {
		this->pos = pos;
		this->data = data;
	}
	block(const block& another) {
		this->data = another.data;
		this->pos = another.pos;
	}
};
//定义地图
block map[16];

//定义分数
unsigned long long score;

//定义空格子链表
list <block> emptyBlock;

inline int rand2or4() {
	if (rand() & 1) return 2;
	else return 4;
}
list<block>::iterator randPos() {
	auto ite = emptyBlock.begin();
	for (int i = 0; i < rand() % emptyBlock.size(); i++) {
		ite++;
	}
	return ite;
}

//产生新的2或4
void generateRand() {
	auto randNum = randPos();
	map[(*randNum).pos].data = rand2or4();
	emptyBlock.erase(randNum);
}

//初始游戏
void start(){
	score = 0;
	strcpy(putScore, "score: ");
	//初始化map emptyBlock
	for (int i = 0; i < 16; i++) {
		map[i].pos = i;
		map[i].data = 0;
		emptyBlock.push_back(map[i]);
	}
	srand((unsigned int)time(0));
//#if 0
	initgraph(900, 1000);
	setbkcolor(WHITE);
	cleardevice();
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0,_T("Consolas"));
	RECT r = { 0,100,900,1000 };
	drawtext("Please move by 'w' 's' 'a' 'd'\n(press any key to continue)", &r, DT_CENTER);
	_getch();
	settextstyle(80, 0,_T("Consolas"));
	setfillcolor(YELLOW);
	for (int i = 120; i < 1000; i += 220) {
		for (int j = 20; j < 900; j += 220) {
			solidroundrect(j, i, j + 200, i + 200,40,40);
		}
	}
//#endif
	//初始化随机两个格子
	generateRand();
	generateRand();
	
	
}

//输出数组，测试用
void show() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%5d", map[i * 4 + j].data);
		}
		cout << endl;
	}
}

//画出当前游戏画面
void draw() {
//20 + 220 * (pos/4)= x
//120 + 220 * (pos%4)= y
	//system("cls");
	//show();
	//cout << emptyBlock.size() << endl;
//#if 0
	cleardevice();
	for (int i = 120; i < 1000; i += 220) {
		for (int j = 20; j < 900; j += 220) {
			solidroundrect(j, i, j + 200, i + 200, 40, 40);
		}
	}
	
	char c[10];
	snprintf(c, 10, "%lld", score);
	RECT R = { 0,0,500,80 };
	drawtext(putScore, &R, DT_LEFT | DT_SINGLELINE);
	R = { 500,0,900,80 };
	drawtext(c, &R, DT_LEFT | DT_SINGLELINE);
	int x, y;
	RECT r;
	TCHAR s[10] = { 0 };
	for (int i = 0; i < 16; i++) {
		if (map[i].data != 0) {
			x = 20 + 220 * (map[i].pos % 4);
			y = 120 + 220 * (map[i].pos / 4);
			r = { x,y,x + 200,y + 200 };
			//转换
			snprintf(s, 9, "%d", map[i].data);
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	FlushBatchDraw();
//#endif
}


//移动函数
void moveBlock(int start, int distance, int num) {
	if (map[start].data == 0&&map[start+distance].data!=0 ) {
		list<block>::iterator ite = emptyBlock.begin();
		for (; ite != emptyBlock.end();ite++) {
			if ((*ite).pos == map[start].pos) {
				emptyBlock.erase(ite);
				break;
			}
		}
		map[start].data = map[start + distance].data;
		map[start + distance].data = 0;
		emptyBlock.push_back(map[start + distance]);
	}
	if (num == 1) return;
	moveBlock(start + distance, distance, num - 1);
}

//合并函数
void mergeBlock(int pos, int distance,int num) {//num 表示后面还有多少个block
	map[pos].data += map[pos+distance].data;
	score += map[pos + distance].data;
	map[pos + distance].data = 0;
	emptyBlock.push_back(map[pos + distance]);
	if (num == 1) return;
	moveBlock(pos+distance, distance, num-1);
}


//向上移动函数
void moveUp(int line) {
	int flag = 0;
	for (int i = 0; i < 3; i++) {
		while (map[i * 4 + line].data == 0) {
			if (flag >= 3) {
				break;
			}
			moveBlock(i * 4 + line, 4, 3 - i);
			flag++;
		}
		if (map[i * 4 + line].data != 0 && flag != 0) validMove = true;
	}
	for (int i = 1; i < 4; i++) {
		//while (map[(i-1) * 4 + line].data == 0) {
		//	if (flag >= 4 - i) break;
		//	moveBlock((i - 1) * 4 + line, 4, 4-i);
		//	flag++;
		//}
		if (map[(i - 1) * 4 + line].data == map[i * 4 + line].data && map[i * 4 + line].data != 0) {
			validMove = true;
			mergeBlock((i - 1) * 4 + line, 4, 4 - i);
		}
	}
}
//向下移动函数
void moveDown(int line) {
	int flag = 0;
	for (int i = 0; i < 3; i++) {
		while (map[(3 - i) * 4 + line].data == 0) {
			if (flag >= 3) {
				break;
			}
			moveBlock((3 - i) * 4 + line, -4, 3 - i);
			flag++;
		}
		if (map[(3 - i) * 4 + line].data != 0 && flag != 0) validMove = true;
	}
	for (int i = 1; i < 4; i++) {
		//while (map[(4 - i) * 4 + line].data == 0) {
		//	if (flag >= 4 - i) break;
		//	moveBlock((4 - i) * 4 + line, -4, 4 - i);
		//	flag++;
		//}
		if (map[(4 - i) * 4 + line].data == map[(3 - i) * 4 + line].data && map[(3 - i) * 4 + line].data != 0) {
			validMove = true;
			mergeBlock((4 - i) * 4 + line, -4, 4 - i);
		}
	}
}
//向左移动函数
void moveLeft(int row) {
	int flag = 0;
	for (int i = 0; i < 3; i++) {
		while (map[row * 4 + i].data == 0) {
			if (flag > 3) {
				break;
			}
			moveBlock(row * 4 + i, 1, 3 - i);
			flag++;
		}
		if (map[row * 4 + i ].data != 0 && flag != 0) validMove = true;
	}
	for (int i = 1; i < 4; i++) {
		//while (map[row * 4 + i - 1].data == 0) {
		//	if (flag >= 4 - i) break;
		//	moveBlock(row * 4 + i - 1, 1, 4 - i);
		//	flag++;
		//}
		if (map[row * 4 + i].data == map[row * 4 + i - 1].data && map[row * 4 + i].data != 0) {
			validMove = true;
			mergeBlock(row * 4 + i - 1, 1, 4 - i);
		}
	}
}
//向右移动函数
void moveRight(int row) {
	int flag = 0;
	int i;
	for (i = 0; i < 3; i++) {
		while (map[row * 4 + 3 - i].data == 0) {
			if (flag > 3) {
				break;
			}
			moveBlock(row * 4 + 3 - i, -1, 3 - i);
			flag++;
		}
		if (map[row * 4 + 3 - i].data != 0 && flag != 0) validMove = true;
	}
	for ( i = 1; i < 4; i++) {
		//while (map[row * 4 + 4 - i].data == 0) {
		//	if (flag >= 4 - i) break;
		//	moveBlock(row * 4 + 4 - i, -1, 4 - i);
		//	flag++;
		//}
		if (map[row * 4 + 4 - i].data == map[row * 4 + 3 - i].data && map[row * 4 + 3 - i].data != 0) {
			validMove = true;
			mergeBlock(row * 4 + 4 - i, -1, 4 - i);
		}
	}
}


//游戏操作判断
void play() {
	if (GetAsyncKeyState(VK_UP)) {
		for (int i = 0; i < 4; i++) {
			moveUp(i);
		}
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		for (int i = 0; i < 4; i++) {
			moveDown(i);
		}
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		for (int i = 0; i < 4; i++) {
			moveLeft(i);
		}
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		for (int i = 0; i < 4; i++) {
			moveRight(i);
		}
	}
	generateRand();
}

void play2() {
	validMove = false;
	char c = _getch();
	for (int i = 0; i < 4; i++) {
		switch (c)
		{
		case 'w':
			moveUp(i);
			break;
		case 's':
			moveDown(i);
			break;
		case 'a':
			moveLeft(i);
			break;
		case 'd':
			moveRight(i);
			break;
		default:
			break;
		}
	}
	if(validMove)
	generateRand();
}

//方向数组
int direction[4] = { -1,1,-4,4 };

//判断游戏是否结束
bool gameOver() {
	int i = 0;
	while (1) {
		for (int j = 0; j < 4; j++) {
			if ((i + direction[j]) >= 0 && (i + direction[j]) < 16 && map[i].data == map[i + direction[j]].data) {
				if(!((i == 7 && direction[j] == 1)||(i==8&&direction[j]==-1)))
				return false;
			}
		}
		i += 5;
		if (i == 20) i = 2;
		if (i == 12) i = 8;
		if (i == 18) break;
	}
	return true;
}



int main() {
	start();
	BeginBatchDraw();
	while (1) {
		draw();
		if (emptyBlock.size() == 0) {
			if (gameOver()) {
				settextcolor(RED);
				RECT r = { 0,100,900,1000 };
				settextstyle(80, 0, _T("Consolas"));
				drawtext("You Lose!!!  \n(press any key to exit)", &r, DT_CENTER);
				FlushBatchDraw();
				_getch();
				break;
			}
		}
		play2();
		Sleep(10);
	}
	EndBatchDraw();
	closegraph();
#if 0
	while (1) {
		draw();
		if (emptyBlock.size() == 0) {
			if (gameOver()) {
				cout << "You Lose!!!" << endl;
				break;
			}
		}
		play2();
	}
#endif
	return 0;
}