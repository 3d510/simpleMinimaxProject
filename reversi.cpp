#include <bits/stdc++.h>

using namespace std;
#define BOARD_SIZE 8

void printBoard(int board[][BOARD_SIZE]);
vector<pair<int,int> > searchValidMoves(int board[][BOARD_SIZE], bool isComp);
bool isEndState(int board[][BOARD_SIZE]);
vector<int> evaluateBoard(int board[][BOARD_SIZE]);
int** updateBoard(int board[][BOARD_SIZE], pair<int,int> move, bool isComp);	
vector<pair<int,int> > findPlacesToRemove(int board[][BOARD_SIZE], bool isComp, int row, int col);

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
	board[3][2] = -1;
	board[3][3] = -1;


	printBoard(board);
	vector<pair<int,int> > v = searchValidMoves(board,false);
	for (int i=0; i<v.size(); i++) {
		cout << v[i].first << " " << v[i].second << endl;
	}
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
		vector<pair<int,int> > places = findPlacesToRemove(board,isComp,i,j);
		if (places.size())
			validMoves.push_back(make_pair(i,j));
	}
	return validMoves;
}

vector<pair<int,int> > findPlacesToRemove(int board[][BOARD_SIZE], bool isComp, int row, int col) {
	// row and col in inputs are row and col of a blank cell (a possible move)
	// output: vector of pairs, each pair (x,y) such that when we play the move on
	// cell (row, col) we will remove all the discs between (x,y) and (row,col)
	// vertically, horizontally, diagonally
	// return empty vector if the cell is not a move

	vector<pair<int,int> > places;

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
		places.push_back(make_pair(row,index));

	// looking in same row, larger columns
	index = col+1;
	while (index<BOARD_SIZE && board[row][index]==oppositeValue)
		index++;
	if (index<BOARD_SIZE && board[row][index]==matchValue && index-col>=2)
		places.push_back(make_pair(row,index));

	// looking in same column, smaller rows
	index = row-1;
	while (index>=0 && board[index][col]==oppositeValue)
		index--;
	if (index>=0 && board[index][col]==matchValue && row-index>=2)
		places.push_back(make_pair(index,col));

	// looking in same column, larger rows
	index = row+1;
	while (index<BOARD_SIZE && board[index][col]==oppositeValue)
		index++;
	if (index<BOARD_SIZE && board[index][col]==matchValue && index-row>=2)
		places.push_back(make_pair(index,col));

	// looking diagonally
	index = 1;
	while (row+index<BOARD_SIZE && col+index<BOARD_SIZE && board[row+index][col+index]==oppositeValue)
		index++;
	if (row+index<BOARD_SIZE && col+index<BOARD_SIZE && board[row+index][col+index]==matchValue && index>1)
		places.push_back(make_pair(row+index,col+index));

	index = 1;
	while (row-index>=0 && col-index>=0 && board[row-index][col-index]==oppositeValue)
		index++;
	if (row+index>=0 && col+index>=0 && board[row-index][col-index]==matchValue && index>1)
		places.push_back(make_pair(row-index,col-index));

	index = 1;
	while (row-index>=0 && col+index<BOARD_SIZE && board[row-index][col+index]==oppositeValue)
		index++;
	if (row-index>=0 && col+index<BOARD_SIZE && board[row-index][col+index]==matchValue && index>1)
		places.push_back(make_pair(row-index,col+index));

	index = 1;
	while (row+index<BOARD_SIZE && col-index>=0 && board[row+index][col-index]==oppositeValue)
		index++;
	if (row+index<BOARD_SIZE && col-index>=0 && board[row+index][col-index]==matchValue && index>1)
		places.push_back(make_pair(row+index,col-index));

	return places;
}

bool isEndState(int board[][BOARD_SIZE]) {
	vector<pair<int,int> > validMovesForComp = searchValidMoves(board, true);
	vector<pair<int,int> > validMovesForHuma = searchValidMoves(board, false);
	return validMovesForComp.size()==0 && validMovesForHuma.size()==0;
}

vector<int> evaluateBoard(int board[][BOARD_SIZE]){
	int humanPoints = 0, computerPoints = 0;
	vector<int> points (3); //Stores human,computer,difference (computer - human) values in that order
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE; j++) {
			if (board[i][j] == 1) 
				humanPoints++;
			else if (board[i][j] == -1) 
				computerPoints++;
		}
	}
	points[0] = humanPoints;
	points[1] = computerPoints;
	points[2] = computerPoints - humanPoints;

	return points;
}

vector<pair<int,int>> findBracket(int board[][BOARD_SIZE], pair<int,int> move, bool isComp){
	
}

int** updateBoard(int board[][BOARD_SIZE], pair<int,int> move, bool isComp){
	int matchValue = 1, oppositeValue;
	pair<int,int> closeMove;
	
	if (isComp)
		matchValue = -1;
	oppositeValue = -matchValue;

	vector<int> bracket = findBracket(board,move,isComp); // row,col,orientation: RIGHT 1, RIGHTUP 2, RIGHTDOWN 3, LEFT 4, LEFTUP 5, LEFTDOWN 6, DOWN 7, UP 8
	closeMove = make_pair(bracket[0],bracket[1]);

	board[move.first][move.second]=matchValue;

	switch ( bracket[2] ) {
		case 1:        
			for(int i=move.second+1;i<closeMove.second;i++){
				board[move.first][i] = matchValue;
			}
			break;
		case 2:
			for(int i=move.second+1;i<closeMove.second;i++) for(int j=move.first-1;j>closeMove.first;j--){
				board[j][i] = matchValue;
			}
			break;
		case 3:
			for(int i=move.second+1;i<closeMove.second;i++) for(int j=move.first+1;j<closeMove.first;j++){
				board[j][i] = matchValue;
			}
			break;
		case 4:
			for(int i=move.second-1;i>closeMove.second;i--){
				board[move.first][i] = matchValue;
			}
			break;
		case 5:
			for(int i=move.second-1;i>closeMove.second;i--) for(int j=move.first-1;j>closeMove.first;j--){
				board[j][i] = matchValue;
			}
			break;	
		case 6:
			for(int i=move.second-1;i>closeMove.second;i--) for(int j=move.first+1;j<closeMove.first;j++){
				board[j][i] = matchValue;
			}
			break;	
		case 7:
			for(int i=move.first+1;i<closeMove.first;i++){
				board[i][move.second] = matchValue;
			}
			break;	
		case 8:
			for(int i=move.first-1;i>closeMove.first;i--){
				board[move.first][i] = matchValue;
			}
			break;	
		default:    
			cout<<"Error, bad input, quitting\n";
			break;
	}	
}