#include<iostream>
#include<fstream>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include<math.h>
#include<time.h>
using namespace std;
#define max 8
#define Name 50
#define Players 2
#define MaxR_C 64
#define Black 0
#define White 1

void Color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void L2R(int R, int cs, int ce, char sym)
{
	for (int c = cs; c <= ce; c++)
	{
		gotoRowCol(R, c);
		cout << sym;
	}
}
void T2B(int C, int rs, int re, char sym)
{
	for (int r = rs; r <= re; r++)
	{
		gotoRowCol(r, C);
		cout << sym;
	}
}
void Init(char B[max][max], int& Turn, const char* FN)
{
	ifstream Rdr(FN);
	Rdr >> Turn;
	if (FN == "New.txt")
		Turn = rand() % Players;
	for (int r = 0; r < max; r++)
	{
		for (int c = 0; c < max; c++)
		{
			Rdr >> B[r][c];
		}
	}
	for (int r = 0; r < max; r++)
	{
		for (int c = 0; c < max; c++)
		{
			if (B[r][c] == '-')
				B[r][c] = ' ';
		}
	}
}
void displayMessage(int turn)
{
	gotoRowCol(0, MaxR_C + 3);
	if (turn == White)
		cout << "White's turn";
	else if (turn == Black)
		cout << "Black's turn";
}
void turnchange(int& T)
{
	T++;
	if (T == Players)
		T = 0;
}
void Grid()
{
	char sym = -37;
	for (int i = 0; i <= MaxR_C; i += 8)
	{
		T2B(i, 0, MaxR_C, sym);
		L2R(i, 0, MaxR_C, sym);
	}
	/*gotoRowCol(67, 60);
	cout << "Save Game";*/
}
void DisplayBoard(char B[max][max])
{
	int r, c;
	for (int ri = 0; ri < max; ri++)
	{
		for (int ci = 0; ci < max; ci++)
		{
			r = ri * (MaxR_C / max) + 4;
			c = ci * (MaxR_C / max) + 4;
			gotoRowCol(r, c);
			cout << B[ri][ci];
		}
	}
}
char ToUpper(char c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return c;
}
void SelectPosition(int& r, int& c)
{
	int ri, ci;
	getRowColbyLeftClick(ri, ci);
	if (ri == 0 && ci == 90)
		r = ri, c = ci;
	else
	{
		r = ri / max;
		c = ci / max;
	}
	/*char C, R;
	cout << "\nPosition: ";
	cin >> C >> R;
	C = ToUpper(C);
	r = R - '1';
	c = C - 'A';*/
}
bool BlackPiece(char s)
{
	return ('A' <= s && s <= 'Z');
}
bool WhitePiece(char s)
{
	return ('a' <= s && s <= 'z');
}
bool isMyPiece(int Turn, char s)
{
	if (Turn == Black)
		return BlackPiece(s);
	else if (Turn == White)
		return WhitePiece(s);
}
bool isValidPosition(int Turn, char s)
{
	if (s == ' ')
		return false;
	return isMyPiece(Turn, s);
	/*if (Turn == Black)
		return BlackPiece(s);
	else if (Turn == White)
		return WhitePiece(s);*/
}
bool isValidDestination(int Turn, char s)
{
	return !isMyPiece(Turn, s);

	/*if (Turn == Black)
		return !BlackPiece(s);
	if (Turn == White)
		return !WhitePiece(s);*/
}
bool VerticalMove(int sr, int sc, int er, int ec)
{
	return sc == ec;
}
bool HorizontalMove(int sr, int sc, int er, int ec)
{
	return sr == er;
}
bool DiagonalMove(int sr, int sc, int er, int ec)
{
	int dr = er - sr;
	int dc = ec - sc;
	return abs(dr) == abs(dc);
}
bool VerticalClear(char B[max][max], int sr, int sc, int er, int ec)
{
	if (sr < er)
	{
		for (int r = sr + 1; r < er; r++)
			if (B[r][sc] != ' ')
				return false;
		return true;
	}
	else
	{
		for (int r = er + 1; r < sr; r++)
			if (B[r][sc] != ' ')
				return false;
		return true;
	}
}
bool HorizontalClear(char B[max][max], int sr, int sc, int er, int ec)
{
	if (sc < ec)
	{
		for (int c = sc + 1; c < ec; c++)
			if (B[sr][c] != ' ')
				return false;
		return true;
	}
	else
	{
		for (int c = ec + 1; c < sc; c++)
			if (B[sr][c] != ' ')
				return false;
		return true;
	}
}
bool DiagonalClear(char B[max][max], int sr, int sc, int er, int ec)
{
	if (sr > er && ec < sc)
	{
		for (int i = sr - 1; i > er; i--)
		{
			sc--;
			if (B[i][sc] != ' ')
				return false;
		}
		return true;
	}
	if (sr > er && ec > sc)
	{
		for (int i = sr - 1; i > er; i--)
		{
			sc++;
			if (B[i][sc] != ' ')
				return false;
		}
		return true;
	}
	if (sr < er && ec < sc)
	{
		for (int i = sr + 1; i < er; i++)
		{
			sc--;
			if (B[i][sc] != ' ')
				return false;
		}
		return true;

	}
	if (sr < er && ec > sc)
	{
		for (int i = sr + 1; i < er; i++)
		{
			sc++;
			if (B[i][sc] != ' ')
				return false;
		}
		return true;

	}
	return false;

	//int dr = er - sr, dc = ec - sc;
	//if (dr < 0 && dc > 0)
	//{
	//	for (int i = 1; i <= dr; i++)
	//	{
	//		if (B[sr - i][sc + i] != ' ')
	//			return false;
	//	}
	//	return true;
	//}
	//if (dr < 0 && dc < 0)
	//{
	//	for (int i = sr-1; i > er; i--)
	//	{
	//		sc--;
	//		if (B[sr][sc] != ' ')
	//			return false;
	//	}
	//	return true;
	//}
	//if (dr > 0 && dc > 0)
	//{
	//	for (int i = 1; i <= dr; i++)
	//	{
	//		if (B[sr + i][sc + i] != ' ')
	//			return false;
	//	}
	//	return true;
	//}
	//if (dr > 0 && dc < 0)
	//{
	//	for (int i = 1; i <= dr; i++)
	//	{
	//		if (B[sr + i][sc - i] != ' ')
	//			return false;
	//	}
	//	return true;
	//}
	//return false;
}
bool isRookLegal(char B[max][max], int sr, int sc, int er, int ec)
{
	return ((VerticalMove(sr, sc, er, ec) && VerticalClear(B, sr, sc, er, ec)) || (HorizontalMove(sr, sc, er, ec) && HorizontalClear(B, sr, sc, er, ec)));
}
bool isBishopLegal(char B[max][max], int sr, int sc, int er, int ec)
{
	return (DiagonalMove(sr, sc, er, ec) && DiagonalClear(B, sr, sc, er, ec));
}
bool isQueenLegal(char B[max][max], int sr, int sc, int er, int ec)
{
	return (isRookLegal(B, sr, sc, er, ec) || (isBishopLegal(B, sr, sc, er, ec)));
}
bool isKingLegal(char B[max][max], int sr, int sc, int er, int ec)
{
	int dr = er - sr;
	int dc = ec - sc;
	return (isQueenLegal(B, sr, sc, er, ec) && (abs(dr) <= 1 && abs(dc) <= 1));
}
bool isKnightLegal(char B[max][max], int sr, int sc, int er, int ec)
{
	int dr = abs(er - sr);
	int dc = abs(ec - sc);
	return ((dr == 1 && dc == 2) || (dr == 2 && dc == 1));
}
bool isPawnLegal(char B[max][max], int sr, int sc, int er, int ec, int Turn)
{
	int dr = er - sr, dc = ec - sc;
	if ((Turn == Black && dr > 0) || (Turn == White && dr < 0))
	{
		if (dc == 0 && abs(dr) == 1)
			return true;
		if (dc == 0 && abs(dr) == 2 && (sr == 6 || sr == 1))
			return true;
		if (DiagonalMove(sr, sc, er, ec) && abs(dr) == 1 && dc == 0 && B[er][ec] != ' ')
			return true;
	}
	return false;
}
bool isValidMove(char B[max][max], int Turn, int sr, int sc, int er, int ec)
{
	switch (B[sr][sc])
	{
	case 'R':
	case 'r':
		return isRookLegal(B, sr, sc, er, ec);
	case 'H':
	case 'h':
		return isKnightLegal(B, sr, sc, er, ec);
	case 'B':
	case 'b':
		return isBishopLegal(B, sr, sc, er, ec);
	case 'Q':
	case 'q':
		return isQueenLegal(B, sr, sc, er, ec);
	case 'K':
	case 'k':
		return isKingLegal(B, sr, sc, er, ec);
	case 'P':
	case 'p':
		return isPawnLegal(B, sr, sc, er, ec, Turn);
	}
}
void Move(char B[max][max], int sr, int sc, int er, int ec)
{
	char s;
	s = B[sr][sc];
	B[sr][sc] = ' ';
	B[er][ec] = s;
}
void SaveGame(char B[max][max], int Turn)
{
	ofstream Writer("SaveGame.txt");
	Writer << Turn;
	Writer << endl;
	for (int r = 0; r < max; r++)
	{
		for (int c = 0; c < max; c++)
		{
			if (B[r][c] == ' ')
				B[r][c] = '-';
			Writer << B[r][c];
		}
		Writer << endl;
	}
	for (int r = 0; r < max; r++)
		for (int c = 0; c < max; c++)
			if (B[r][c] == '-')
				B[r][c] = ' ';
}
void Highlight(char B[max][max], int Turn, int sr, int sc)
{
	char HB[max][max] = { ' ' };
	for (int r = 0; r < max; r++)
		for (int c = 0; c < max; c++)
			HB[r][c] = B[r][c];
	for (int r = 0; r < max; r++)
		for (int c = 0; c < max; c++)
			if (isValidDestination(Turn, B[r][c]) && isValidMove(B, Turn, sr, sc, r, c))
				HB[r][c] = '*';
	DisplayBoard(HB);
}
void FindKingPosition(char B[max][max], int& kr, int& kc, int Turn)
{
	turnchange(Turn);
	for (int r = 0; r < max; r++)
	{
		for (int c = 0; c < max; c++)
		{
			if (isMyPiece(Turn, B[r][c]) && (B[r][c] == 'K' || B[r][c] == 'k'))
			{
				kr = r, kc = c;
				return;
			}
		}
	}
}
bool Check(char B[max][max], int Turn)
{
	int kr, kc;
	FindKingPosition(B, kr, kc, Turn);
	for (int r = 0; r < max; r++)
	{

		for (int c = 0; c < max; c++)
		{
			if (isMyPiece(Turn, B[r][c]) && isValidMove(B, Turn, r, c, kr, kc))
				return true;
		}
	}
	return false;
}
bool SelfCheck(char B[max][max], int Turn, int sr, int sc, int er, int ec)
{
	/*int kr, kc;
	FindKingPosition(B, kr, kc, Turn);
	for (int r = 0; r < max; r++)
		for (int c = 0; c < max; c++)
			if (isValidMove(B, Turn, r, c, kr, kc))
				return true;
	return false;*/
	char CB[max][max];
	for (int r = 0; r < max; r++)
		for (int c = 0; c < max; c++)
			CB[r][c] = B[r][c];
	Move(CB, sr, sc, er, ec);
	turnchange(Turn);
	if (Check(CB, Turn))
		return true;
	return false;
}
void UndoBoard(char Undo[max][max], int& Turn)
{
	ifstream Rdr("SaveGame.txt");
	Rdr >> Turn;
	for (int r = 0; r < max; r++)
	{
		for (int c = 0; c < max; c++)
		{
			Rdr >> Undo[r][c];
		}
	}
	for (int r = 0; r < max; r++)
	{
		for (int c = 0; c < max; c++)
		{
			if (Undo[r][c] == '-')
				Undo[r][c] = ' ';
		}
	}
}
void UndoMove(char B[max][max], char Undo[max][max])
{
	for (int r = 0; r < max; r++)
	{
		for (int c = 0; c < max; c++)
		{
			if (Undo[r][c] == '-')
				Undo[r][c] = ' ';
			B[r][c] = Undo[r][c];
		}
	}
}
bool AnyMoveLeft(char B[max][max], int Turn)
{
	int kr, kc;
	FindKingPosition(B, kr, kc, Turn);
	for (int r = 0; r < max; r++)
		for (int c = 0; c < max; c++)
			if (!isMyPiece(Turn, B[kr][kc]) && isValidMove(B, Turn, kr, kc, r, c))
				return true;
	return false;
}
bool Checkmate(char B[max][max], int Turn)
{
	int kr, kc;
	FindKingPosition(B, kr, kc, Turn);
	for (int r = 0; r < max; r++)
		for (int c = 0; c < max; c++)
			if (Check(B, Turn) && !AnyMoveLeft(B, Turn) && SelfCheck(B, Turn, kr, kc, r, c))
				return true;
	return false;
}
bool Stalemate(char B[max][max], int Turn)
{
	int kr, kc;
	FindKingPosition(B, kr, kc, Turn);
	for (int r = 0; r < max; r++)
		for (int c = 0; c < max; c++)
			if (!Check(B, Turn) && !AnyMoveLeft(B, Turn) && SelfCheck(B, Turn, kr, kc, r, c))
				return true;
	return false;
}
void CopyBoard(const char B[max][max], char D[max][max])
{
	for (int r = 0; r < max; r++)
		for (int c = 0; c < max; c++)
			D[r][c] = B[r][c];
}
void Castling(char B[max][max], int Turn, int sr, int sc, int er, int ec, bool kingMove)
{
	bool possibleRight = false, possibleLeft = false;
	if (sc < ec && sr == er)
	{
		for (int c = sr + 1; c <= er; c++)
		{
			if (!SelfCheck(B, Turn, sr, sc, er, c) && HorizontalClear(B, sr, sc, er, ec))
			{
				possibleRight = true;
			}
		}
	}
	if (sc > ec && sr == er)
	{
		for (int c = sr - 1; c >= er; c--)
		{
			if (!SelfCheck(B, Turn, sr, sc, er, c) && HorizontalClear(B, sr, sc, er, ec))
			{
				possibleLeft = true;
			}
		}
	}
	if (possibleRight)
	{
		Move(B, sr, sc, er, ec);
		Move(B, sr, 7, er, ec - 1);
	}
	if (possibleLeft)
	{
		Move(B, sr, sc, er, ec);
		Move(B, sr, 7, er, ec + 1);
	}
}
int main()
{
	srand(time(0));
	char B[max][max], Undo[max][max], FN[10];
	int Turn = 0, sr = 0, sc = 0, er = 0, ec = 0;
	bool check, GameOver = false;
	cout << "\n\nCHESS GAME.......\n\n";
	cout << "Black are Capital and White are Small letters.... Beep sound when invalid..\n\n";
	cout << "File to Load New.txt/SaveGame.txt: ";
	cin >> FN;
	system("cls");
	gotoRowCol(0, 90);
	cout << '@';
	Init(B, Turn, FN);
	Grid();
	DisplayBoard(B);
	do
	{
		time_t st = time(0);
	x:
		CopyBoard(B, Undo);
		Color(15);
		do
		{
			do
			{
				displayMessage(Turn);
				do
				{
					SelectPosition(sr, sc);
					if (sr == 0 && sc == 90)
					{
						Init(B, Turn, "SaveGame.txt");
						DisplayBoard(B);
						//turnchange(Turn);
						goto x;
					}
					if (!isValidPosition(Turn, B[sr][sc]))
						cout << "\7";
				} while (!isValidPosition(Turn, B[sr][sc]));
				Highlight(B, Turn, sr, sc);
				SelectPosition(er, ec);
				if (!isValidDestination(Turn, B[er][ec]))
					cout << "\7";
				if (SelfCheck(B, Turn, sr, sc, er, ec))
				{
					gotoRowCol(5, 70);
					Color(4);
					cout << "Self Check\7";
				}
				else
				{
					gotoRowCol(5, 70);
					cout << "          ";
				}
			} while (!isValidDestination(Turn, B[er][ec]));
			/*if ((B[sr][sc] == 'k' || B[sr][sc] == 'K') && (sr == er && ec == (max - 1)))
			{
				Castling(B, Turn, sr, sc, er, ec, 1);
			}*/
		} while (!isValidMove(B, Turn, sr, sc, er, ec));
		time_t et = time(0);
		time_t Tt = et - st;
		if (Tt > 60)
		{
			gotoRowCol(MaxR_C / 2, MaxR_C + 25);
			cout << "GAME OVER... TIMES UP..";
			return 0;
		}
		if (!SelfCheck(B, Turn, sr, sc, er, ec))
		{
			Move(B, sr, sc, er, ec);
			DisplayBoard(B);
			if (Check(B, Turn))
			{
				gotoRowCol(7, 70);
				Color(4);
				cout << "Check!!\7";
			}
			else
			{
				gotoRowCol(7, 70);
				cout << "       ";
			}
			if (Checkmate(B, Turn))
			{
				Color(4);
				gotoRowCol(10, 70);
				cout << "Check Mate...\7\7\7";
				GameOver = true;
			}
			if (Stalemate(B, Turn))
			{
				Color(4);
				gotoRowCol(10, 70);
				cout << "Stale Mate...\7\7\7";
				GameOver = true;
			}
			if (!Checkmate(B, Turn))
				turnchange(Turn);
		}
		SaveGame(Undo, Turn);
	} while (!GameOver);
	if (Turn == Black)
	{
		gotoRowCol(20, 70);
		cout << "Black Wins..";
	}
	if (Turn == White)
	{
		gotoRowCol(20, 70);
		cout << "White Wins..";
	}
	return _getch();
}