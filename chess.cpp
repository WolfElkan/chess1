#include <iostream>
#include <math.h>
#include <array>
#include <regex>
#include <stdio.h>
#include "./uint256_t.h"
#include "./uint128_t.h"
using namespace std;

const int Empty       =  0;
const int WhitePawn   =  1;
const int WhiteKnight =  2;
const int WhiteKing   =  3;

const int WhiteRook   =  5;
const int WhiteBishop =  6;
const int WhiteQueen  =  7;

const int BlackPawn   =  9;
const int BlackKnight = 10;
const int BlackKing   = 11;

const int BlackRook   = 13;
const int BlackBishop = 14;
const int BlackQueen  = 15;

int board[8][8];

int court[8] = {5,2,6,7,3,6,2,5};

string String(char* a, int size){ 
    int i; 
    string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
}

// char Chars(string)

char piece_symbol(int n){
	char pieces[16] = {' ','P','N','K','X','R','B','Q','*','p','n','k','x','r','b','q'};
	if (n > 16) {
		return '!';
	} else {
		return pieces[n];
	}
	return '0';
}

char Piece_symbol(int n){
	char pieces[16] = {' ','P','N','K','~','R','B','Q','*','P','N','K','~','R','B','Q'};
	if (n > 16) {
		return '!';
	} else {
		return pieces[n];
	}
	return '0';
}

int printboard(int board[8][8], bool grid) {
	for (int rank = 7; rank >= 0; rank--){
		if (grid) {
			cout << rank + 1 << ' ';
		}
		for (int file = 0; file < 8; file++){
			cout << piece_symbol(board[rank][file]) << ' ';
		}
		cout << endl;
	}
	if (grid) {
		cout << "  a b c d e f g h" << endl;
	}
	return 0;
}

// class Piece {
// public:
// 	Piece(int rank, int file, bool color){
// 		bool mov_range[8][8];
// 		bool cap_range[8][8];
// 	};
	
// };

// class Pawn : public Piece {
// public:
// 	Pawn(int rank, int file, bool color){
// 		Piece super(rank, file, color);
// 		// cout << super.mov_range << endl;
// 	}
// };

int parse_space(string space){
	int rank = int(space[1]) - 49;
	int file = int(space[0]) - 97;
	if (file < 0){
		file += 32;
	}
	return rank * 8 + file;
	return 0;
}

string pack_space(int rf){
	int r = rf / 8;
	int f = rf % 8;
	char rc = char(r + 49);
	char fc = char(f + 97);
	string result = "";
	result += fc;
	result += rc;
	return result;
}

string pack_space(int r, int f){
	return pack_space(r * 8 + f);
}

bool make_move(int board[8][8], bool turn, string move){
	regex re("((([RNBQKP]|)([a-h]?)([1-8]?)(x?)([a-h])([1-8]))|(O-O(-O|)))([+#]?)");
	smatch match;
	regex_match(move, match, re);
	if (match.length()) {
		string pieceS = match.str(3);
		if (pieceS.length() == 0) {
			pieceS = "P";
		}
		char piece = pieceS[0];
		string rank0S  = match.str(5);
		string file0S  = match.str(4);
		string capture = match.str(6);
		string rank1S  = match.str(8);
		string file1S  = match.str(7);
		string castle  = match.str(9);
		string check  = match.str(11);

		int rank0 = 56 - rank0S[0];
		int file0 = file0S[0] - 97;
		int rank1 = 56 - rank1S[0];
		int file1 = file1S[0] - 97;

		char current_piece = piece_symbol(board[rank0][file0]);
		current_piece -= 32 * turn;
		if (current_piece == piece) {
			int moving = board[rank0][file0];
			board[rank0][file0] = 0;
			board[rank1][file1] = moving;
			return true;
		}
		// cout << current_piece << endl;
	}
	return false;
}

uint256_t bitify(int board[8][8]){
	uint256_t board_bits = 0;
	for (int rank = 0; rank < 8; rank++){
		for (int file = 0; file < 8; file++)
		{
			board_bits *= 16;
			board_bits += board[rank][file];
		}
	}
	return board_bits;
}

int boardify(int board[8][8], uint256_t board_bits){
	return 0;
}

uint256_t mover(uint256_t board_bits, bool turn, string move){
	// Parse move
		// use RegEx
	// Convert bits to board array
	int board[8][8];
	cout << board_bits << endl;
	// Make move on board
	make_move(board, turn, move);
	// Validate move
	// Convert board to bits and return
	// return bitify(board);
	return 0;
}

int setboard(int board[8][8], uint256_t board_bits){
	for (int rank = 0; rank < 8; rank++){
		for (int file = 0; file < 8; file++)
		{
			board[rank][file] = 0;
		}
	}

	return 0;
}

unsigned long long board64(bool board[8][8]){
	unsigned long long result = 0;
	for (int rank = 7; rank >= 0; rank--){
		for (int file = 7; file >= 0; file--)
		{
			result *= 2;
			result += board[rank][file];
		}
	}
	return result;	
}

unsigned long long occupied(int board[8][8], bool color){
	int min = color * 8 + 1;
	int max = color * 8 + 7;
	bool boolboard[8][8];
	for (int rank = 0; rank < 8; rank++){
		for (int file = 0; file < 8; file++){
			int piece = board[rank][file];
			bool result = piece >= min and piece <= max;
			boolboard[rank][file] = result;
		}
	}
	return board64(boolboard);
}

unsigned long long rangeP(int piece, int board[8][8], bool boolboard[8][8], int r0, int f0, bool move, bool capture) {
	// En Passant not yet supported
	int color = piece / 8;
	int r1;
	int f1;
	int target;
	if (move) {	
		// One space forward
		r1 = r0 + 1 - (color * 2);
		f1 = f0;
		target = board[r1][f1];
		if (target == Empty) {
			boolboard[r1][f1] = 1;
			cout << pack_space(r1,f1) << endl;
		}
		// Two spaces forward
		if (r0 == 1 + color * 5) {
			r1 = r0 + 2 - (color * 4);
			target = board[r1][f1];
			if (target == Empty){
				boolboard[r1][f1] = 1;
				cout << pack_space(r1,f1) << endl;
			}
		}
	}
	if (capture) {
		r1 = r0 + 1 - (color * 2);
		// Capture West (toward A file)
		if (f0 - 1 >= 0) {
			f1 = f0 - 1;
			target = board[r1][f1];
			if (target and (target/8)^(piece/8)) {
				boolboard[r1][f1] = 1;
				cout << pack_space(r0,f0)[0] << 'x' << pack_space(r1,f1) << endl;
			}
		}
		// Capture East (toward H file)
		if (f0 + 1 < 8) {
			f1 = f0 + 1;
			target = board[r1][f1];
			if (target and (target/8)^(piece/8)) {
				boolboard[r1][f1] = 1;
				cout << pack_space(r0,f0)[0] << 'x' << pack_space(r1,f1) << endl;
			}
		}
	}
	return board64(boolboard);
}

unsigned long long rangeN(int piece, int board[8][8], bool boolboard[8][8], int r0, int f0, bool move, bool capture) {
	int knight_move[8][2] = {
		{ 2, 1},
		{ 1, 2},
		{-1, 2},
		{-2, 1},
		{-2,-1},
		{-1,-2},
		{ 1,-2},
		{ 2,-1}
	};
	for (int i = 0; i < 8; i++){
		int r1 = r0 + knight_move[i][0];
		int f1 = f0 + knight_move[i][1];
		if (r1 < 8 and r1 >= 0 and f1 < 8 and f1 >= 0){
			int target = board[r1][f1];
			if (move and (target == Empty)){
				boolboard[r1][f1] = 1;
				cout << Piece_symbol(piece) << pack_space(r1,f1) << endl;
			} else if (target and capture and (target/8)^(piece/8)){
				boolboard[r1][f1] = 1;
				cout << Piece_symbol(piece) << 'x' << pack_space(r1,f1) << endl;
			}
		}
	}
	return board64(boolboard);
}

unsigned long long rangeK(int piece, int board[8][8], bool boolboard[8][8], int r0, int f0, bool move, bool capture) {
	int knight_move[8][2] = {
		{ 0, 1},
		{ 1, 1},
		{ 1, 0},
		{ 1,-1},
		{ 0,-1},
		{-1,-1},
		{-1, 0},
		{-1, 1},
	};
	for (int i = 0; i < 8; i++){
		int r1 = r0 + knight_move[i][0];
		int f1 = f0 + knight_move[i][1];
		if (r1 < 8 and r1 >= 0 and f1 < 8 and f1 >= 0){
			int target = board[r1][f1];
			if (move and (target == Empty)){
				boolboard[r1][f1] = 1;
				cout << Piece_symbol(piece) << pack_space(r1,f1) << endl;
			} else if (target and capture and (target/8)^(piece/8)){
				boolboard[r1][f1] = 1;
				cout << Piece_symbol(piece) << 'x' << pack_space(r1,f1) << endl;
			}
		}
	}
	return board64(boolboard);
}

unsigned long long rangeR(int piece, int board[8][8], bool boolboard[8][8], int r0, int f0, bool move, bool capture) {
	
	int r1;
	int f1;
	int target;
	int rook_direction[4][2] = {
		{ 0, 1}, // North
		{ 1, 0}, // East
		{ 0,-1}, // South
		{-1, 0}, // West
	};
	for (int i = 0; i < 4; ++i) {
		int rD = rook_direction[i][0];
		int fD = rook_direction[i][1];
		r1 = r0;
		f1 = f0;
		cout << rook_direction[i][0] << rook_direction[i][1] << endl;
		while (r1 < 7 and r1 >= 1 and f1 < 7 and f1 >= 1 and target == Empty) {
			r1 += rD;
			f1 += fD;
			target = board[r1][f1];
			if (target and capture and (target/8)^(piece/8)){
				boolboard[r1][f1] = 1;
				cout << Piece_symbol(piece) << 'x' << pack_space(r1,f1) << endl;
			}
			if (move and (target == Empty)) {
				boolboard[r1][f1] = 1;
				cout << Piece_symbol(piece) << pack_space(r1,f1) << endl;
			} 
		}
	}

	return board64(boolboard);
}

unsigned long long range(int board[8][8], string space, bool move, bool capture) {
	bool boolboard[8][8];
	for (int rank = 0; rank < 8; rank++){
		for (int file = 0; file < 8; file++){
			boolboard[rank][file] = 0;
		}
	}
	int rf = parse_space(space);
	int r = rf / 8;
	int f = rf % 8;
	int piece = board[r][f];
	int color = piece / 8;
	// cout << piece_symbol(piece) << endl;
	switch(piece) {
		case Empty:
			return 0;
			break;

		case WhitePawn:
		case BlackPawn:
			return rangeP(piece, board, boolboard, r, f, move, capture);
			break;

		case WhiteKnight:
		case BlackKnight:
			return rangeN(piece, board, boolboard, r, f, move, capture);
			break;

		case WhiteKing:
		case BlackKing:
			return rangeK(piece, board, boolboard, r, f, move, capture);
			break;

		case WhiteRook:
		case BlackRook:
			return rangeR(piece, board, boolboard, r, f, move, capture);
			break;

		case WhiteBishop:
		case BlackBishop:
			break;

		case WhiteQueen:
		case BlackQueen:
			break;

	}
	return 0;
}

int main(){
	bool turn = 0;
	int turn_number = 1;
	for (int file = 0; file < 8; file++){
		board[7][file] = court[file] + 8;
		board[6][file] = 9;
		board[1][file] = 1;
		board[0][file] = court[file];
	}
	board[4][3] = WhiteRook;
	bool go = true;
		printboard(board,1);
		string space;
		cin >> space;
		cout << range(board,space,1,1) << endl;
		// for (int rank = 0; rank < 8; ++rank) {
		// 	for (int file = 0; file < 8; ++file) {
		// 		space = pack_space(rank,file);
		// 		int piece = board[rank][file];
		// 		if (piece and piece / 8 == turn) {
		// 			range(board,space,1,1);
		// 		}
		// 	}
		// }

	// while (go) {
	// 	printboard(board,1);
	// 	// setboard(board,0);
	// 	// printboard(board,0);
	// 	cout << turn_number << " ";
	// 	if (turn == 0) {
	// 		cout << "White";
	// 	}
	// 	if (turn == 1) {
	// 		cout << "Black";
	// 	}
	// 	cout << ": ";
	// 	string move = "";
	// 	cin >> move;
	// 	int x = make_move(board, turn, move);
	// 	turn_number += x * turn;
	// 	turn = turn ^ x;
	// 	if (move == "quit"){
	// 		go = false;
	// 	}
	// }
	return 0;
}