#include <bits/stdc++.h>

using namespace std;
#define BOARD_SIZE 8

void printBoard(int board[][BOARD_SIZE]);
vector<pair<int,int> > searchValidMoves(int board[][BOARD_SIZE], bool isComp);
bool isEndState(int board[][BOARD_SIZE]);
vector<int> evaluateBoard(int board[][BOARD_SIZE]);
int** updateBoard(int board[][BOARD_SIZE], pair<int,int> move);	
vector<int> findPlacesToRemove(int board[][BOARD_SIZE], bool isComp, int row, int col);

int main() {
	// intialize the board
	int board[BOARD_SIZE][BOARD_SIZE];
	for (int i=0; i<BOARD_SIZE; i++) 
		for (int j=0; j<BOARD_SIZE; j++) 
			board[i][j]=0;
	board[BOARD_SIZE/2-1][BOARD_SIZE/2-1] =  1; 
	board[BOARD_SIZE/2][BOARD_SIZE/2]     =  1;
	board[BOARD_SIZE/2-1][BOARD_SIZE/2]   = -1;
	board[BOARD_SIZE/2][BOARD_SIZE/2-1]   = -1;

	printBoard(board);
}

void printBoard(int board[][BOARD_SIZE]) {
	cout << endl; 
	cout << "    0 1 2 3 4 5 6 7" << endl;
	for (int i=0; i<BOARD_SIZE; i++) {
		cout << "  " << i << " ";
		for (int j=0; j<BOARD_SIZE; j++) {
			if (board[i][j] == 1) 
				cout << "O ";
			else if (board[i][j] == -1) 
				cout << "X ";
			else 
				cout << ". "; 
		}
		cout << endl;
	}
	cout << endl;
}

vector<pair<int,int> > searchValidMoves(int board[][BOARD_SIZE], bool isComp){
	// initialize values 
	// matchValue is the value for current player
	// oppositeValue if the value for opponent	
	int matchValue = 1, oppositeValue;
	vector<pair<int,int> > validMoves;
	if (isComp)
		matchValue = -1;
	oppositeValue = -matchValue;

	// find valid moves
	for (int i=0; i<BOARD_SIZE; i++) for (int j=0; j<BOARD_SIZE; j++) {
		vector<int> places = findPlacesToRemove(board,isComp,i,j);
		bool selected = false;
		for (int k=0; k<places.size(); k++) 
			if (places[k]!=-1) 
				selected = true;
		if (selected)
			validMoves.push_back(make_pair(i,j));
	}
	return validMoves;
}

vector<int> findPlacesToRemove(int board[][BOARD_SIZE], bool isComp, int row, int col) {
	// board[row][col] is blank (==0) else return [-1,-1,-1,-1]	
	// values in places vector to return
	// places[0] is the largest column value such that
	//      board[row][places[0]] == value of current player 
	//      board[row][places[0]+1 to col -1] == value of oppoment
	//      -1 if no such value
	// similarly definied for others where 
	// places[1] is the smallest column value such that ...
	// places[2] is the largest row value such that ...
	// places[3] is the smallest row value such that ...
	vector<int> places;
	places.assign(4,-1);

	if (board[row][col])
		return places;

	int matchValue = 1, oppositeValue;
	if (isComp)
		matchValue = -1;
	oppositeValue = -matchValue;
	int index;

	// looking in same row, smaller columns
	index = col-1;
	while (index>=0 && board[row][index]==oppositeValue)
		index--;
	if (index>=0 && board[row][index]==matchValue && col-index>=2)
		places[0] = index;

	// looking in same row, larger columns
	index = col+1;
	while (index<BOARD_SIZE && board[row][index]==oppositeValue)
		index++;
	if (index<BOARD_SIZE && board[row][index]==matchValue && index-col>=2)
		places[1] = index;

	// looking in same column, smaller rows
	index = row-1;
	while (index>=0 && board[index][col]==oppositeValue)
		index--;
	if (index>=0 && board[index][col]==matchValue && row-index>=2)
		places[2] = index;

	// looking in same column, larger rows
	index = row+1;
	while (index<BOARD_SIZE && board[index][col]==oppositeValue)
		index++;
	if (index<BOARD_SIZE && board[index][col]==matchValue && index-row>=2)
		places[3] = index;

	return places;
}

bool isEndState(int board[][BOARD_SIZE]) {
	vector<pair<int,int> > validMovesForComp = searchValidMoves(board, true);
	vector<pair<int,int> > validMovesForHuma = searchValidMoves(board, false);
	return validMovesForComp.size()==0 && validMovesForHuma.size()==0;
}