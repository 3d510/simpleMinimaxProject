#include <bits/stdc++.h>

using namespace std;
#define BOARD_SIZE 8
#define MAX_DEPTH 5
#define INF 100

int minimax(vector<vector<int> > board, int depth, bool isComp, int alpha, int beta);
pair<int,int> findBestMove(vector<vector<int> > board);
void printBoard(vector<vector<int> > board);
vector<pair<int,int> > searchValidMoves(vector<vector<int> > board, bool isComp);
bool isEndState(vector<vector<int> > board);
vector<int> evaluateBoard(vector<vector<int> > board);
vector<vector<int> > updateBoard(vector<vector<int> > board, pair<int,int> move, bool isComp);
vector<pair<int,int> > findPlacesToRemove(vector<vector<int> > board, bool isComp, int row, int col);
pair<int,int> getInputFromHuma(vector<pair<int,int> >);

int main() {
	// intialize the board
	vector<vector<int> > board(BOARD_SIZE, vector<int>(BOARD_SIZE));
	
	for (int i=0; i<BOARD_SIZE; i++) 
		for (int j=0; j<BOARD_SIZE; j++) 
			board[i][j]=0;
	board[BOARD_SIZE/2-1][BOARD_SIZE/2-1] =  1; 
	board[BOARD_SIZE/2][BOARD_SIZE/2]     =  1;
	board[BOARD_SIZE/2-1][BOARD_SIZE/2]   = -1;
	board[BOARD_SIZE/2][BOARD_SIZE/2-1]   = -1;
	printBoard(board);

	// play the game
	while (true) {
		if (isEndState(board))
			break;

        cout << "COMPUTER TURN" << endl;
		pair<int,int> bestMove = findBestMove(board);

		if (bestMove.first == -1) {
			cout << "The computer has no other moves...\n" << endl;
		} else {
			board = updateBoard(board,bestMove,true);
			printf("The computer has made a move at postion (%d%c)\nUpdating board...\n\n", bestMove.first+1, 'a'+bestMove.second);
			printBoard(board);
		}

        cout << "HUMAN TURN" << endl;
		vector<pair<int,int> > validMovesForHuma = searchValidMoves(board,false);
		if (validMovesForHuma.size() == 0) {
			cout << "The human has no other moves...\n" << endl;
		} else {
			// print valid moves for human
			cout << "The valid moves for human are: " << endl;
			for (int i=0; i<validMovesForHuma.size(); i++)
				printf("%d%c ", validMovesForHuma[i].first+1, 'a'+validMovesForHuma[i].second);
            cout << endl;
			// human enter the move
            pair<int,int> humanMove = getInputFromHuma(validMovesForHuma);
            if (humanMove.first == -1) {
	            cout << "You quit the game!" << endl;
	            return 0;
	        }
			// update the move
			board = updateBoard(board, humanMove, false);
            printBoard(board);
			cout << endl;
		}
	}

	vector<int> finalScore = evaluateBoard(board);
	printf("Final Score: Human - %d, Computer - %d\n", finalScore[0], finalScore[1]);
	if (finalScore[2]<0)
		printf("Human wins!\n");
	else if (finalScore[2]>0)
		printf("Computer wins!\n");
	else
		printf("It's a deuce\n");
}

int minimax(vector<vector<int> > board, int depth, bool isComp, int alpha, int beta){
	if(depth==MAX_DEPTH || isEndState(board))
		return evaluateBoard(board)[2];
	int bestVal;
	vector<pair<int,int> > validMoves = searchValidMoves(board, isComp);

	if(isComp){
		bestVal = -INF;
		for(int i = 0; i < validMoves.size(); i++){
			pair<int,int> move = validMoves[i];
			vector<vector<int> > newBoard = updateBoard(board,move,true);
			int value = minimax(newBoard, depth+1, false, alpha, beta);
			bestVal = max(bestVal, value);
			alpha = max(alpha,bestVal);
			if(beta <= alpha)
				break;
		}
		return bestVal;
	}
	else{
		bestVal = INF;
		for(int i = 0; i < validMoves.size(); i++){
			pair<int,int> move = validMoves[i];
            vector<vector<int> > newBoard = updateBoard(board,move,true);
			int value = minimax(newBoard, depth+1, true, alpha, beta);
			bestVal = min(bestVal, value);
			beta = min(beta,bestVal);
			if(beta <= alpha)
				break;
		}
		return bestVal;	
	}
}

pair<int,int> findBestMove(vector<vector<int> > board) {
	vector<pair<int,int> > validMoves = searchValidMoves(board,true);
	pair<int,int> bestMove = make_pair(-1,-1);

 	int bestVal = -INF;
	for (int i=0; i<validMoves.size(); i++) {
        int moveRow = validMoves[i].first;
        int moveCol = validMoves[i].second;
        board[moveRow][moveCol] = -1;
        int val = minimax(board,0,false,-INF,INF);
        board[moveRow][moveCol] = 0;
        if (val > bestVal) {
            bestMove.first = moveRow;
            bestMove.second = moveCol;
        }
	}
	return bestMove;
}

void printBoard(vector<vector<int> > board) {
	cout << endl; 
	cout << "    a b c d e f g h" << endl;
	for (int i=0; i<BOARD_SIZE; i++) {
		cout << "  " << i+1 << " ";
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

vector<pair<int,int> > searchValidMoves(vector<vector<int> > board, bool isComp){
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

vector<pair<int,int> > findPlacesToRemove(vector<vector<int> > board, bool isComp, int row, int col) {
	// row and col in inputs are row and col of a blank cell (a possible move)
	// output: vector of pairs, each pair (x,y) such that when we play the move on
	// cell (row, col) we will remove all the discs between (x,y) and (row,col)
	// vertically, horizontally, diagonally
	// return empty vector if the cell is not a move

	vector<pair<int,int> > places;

	if (board[row][col]!=0)
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
	if (row-index>=0 && col-index>=0 && board[row-index][col-index]==matchValue && index>1)
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

bool isEndState(vector<vector<int> > board) {
	vector<pair<int,int> > validMovesForComp = searchValidMoves(board, true);
	vector<pair<int,int> > validMovesForHuma = searchValidMoves(board, false);
	return validMovesForComp.size()==0 && validMovesForHuma.size()==0;
}

vector<int> evaluateBoard(vector<vector<int> > board){
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

vector<vector<int> > updateBoard(vector<vector<int> > board, pair<int,int> move, bool isComp){
	int matchValue = 1, oppositeValue;
	//pair<int,int> closeMove;
	
	if (isComp)
		matchValue = -1;
	oppositeValue = -matchValue;

	vector<pair<int,int> > places = findPlacesToRemove(board,isComp,move.first,move.second);

	//orientation: RIGHT 1, RIGHTUP 2, RIGHTDOWN 3, LEFT 4, LEFTUP 5, LEFTDOWN 6, DOWN 7, UP 8
	int orientation = 0;
	board[move.first][move.second]=matchValue;
	
	for(int k = 0; k < places.size(); k++){
		pair<int,int> closeMove = places[k];

		if(move.first==closeMove.first && (move.second - closeMove.second) < 0 )
			orientation = 1;
		else if((move.first - closeMove.first) < 0 && (move.second - closeMove.second) < 0 )
			orientation = 2;
		else if((move.first - closeMove.first) > 0 && (move.second - closeMove.second) < 0 )
			orientation = 3;
		else if(move.first==closeMove.first && (move.second - closeMove.second) > 0 )
			orientation = 4;
		else if((move.first - closeMove.first) < 0 && (move.second - closeMove.second) > 0 )
			orientation = 5;
		else if((move.first - closeMove.first) > 0 && (move.second - closeMove.second) > 0 )
			orientation = 6;
		else if((move.first - closeMove.first) > 0 && move.second==closeMove.second)
			orientation = 8;
		else if((move.first - closeMove.first) < 0 && move.second==closeMove.second)
			orientation = 7;

        //orientation: RIGHT 1, RIGHTUP 2, RIGHTDOWN 3, LEFT 4, LEFTUP 5, LEFTDOWN 6, DOWN 7, UP 8
        int index;
		switch ( orientation ) {
			case 1:        
				for(int i=move.second+1;i<closeMove.second;i++){
					board[move.first][i] = matchValue;
				}
				break;
			case 2:
				index = 1;
				while (move.first + index < closeMove.first) {
					board[move.first+index][move.second+index] = matchValue;
					index++;
				}
				break;
			case 3:
				index = 1;
				while (move.first - index > closeMove.first) {
					board[move.first-index][move.second+index] = matchValue;
					index++;
				}
				break;
			case 4:
				for(int i=move.second-1;i>closeMove.second;i--){
					board[move.first][i] = matchValue;
				}
				break;
			case 5:
				index = 1;
				while (move.first + index < closeMove.first) {
					board[move.first+index][move.second-index] = matchValue;
					index++;
				}
				break;	
			case 6:
				index = 1;
				while (move.first - index > closeMove.first) {
					board[move.first-index][move.second-index] = matchValue;
					index++;
				}
				break;	
			case 7:
				for(int i=move.first+1;i<closeMove.first;i++){
					board[i][move.second] = matchValue;
				}
				break;	
			case 8:
				for(int i=move.first-1;i>closeMove.first;i--){
					board[i][move.second] = matchValue;
				}
				break;	
			default:    
				cout<<"Error, bad input, quitting\n";
				break;
		}	
	}	
	return board;
}

pair<int,int> getInputFromHuma(vector<pair<int,int> > validMovesForHuma) {
	int humanMovRow, humanMovCol;
	string input;
    while (true) {
        bool inputValid = false;
        cout << "Please enter your move (enter -1 to quit)" << endl;
        cin >> input;
        if (input == "-1")
        	return make_pair(-1,-1);
        humanMovRow = input[0]-'1';
        humanMovCol = input[1]-'a';

        for (int i=0; i<validMovesForHuma.size(); i++)
            if (validMovesForHuma[i].first == humanMovRow && validMovesForHuma[i].second == humanMovCol)
                inputValid = true;
        if (inputValid)
            break;
    }
	printf("The human has made a move at postion (%d%c)\nUpdating board...\n\n", humanMovRow+1, 'a'+humanMovCol);
	return make_pair(humanMovRow, humanMovCol);
}