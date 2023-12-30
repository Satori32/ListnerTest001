#include <iostream>
#include <memory>

#include <vector>

#include <conio.h>

#include "EscapeSequence.h"


//not complete.

#define Interface struct

class Game;

Interface ConsoleDisplay{
	virtual bool Draw(Game& In) { return false; };
};

typedef std::shared_ptr<ConsoleDisplay> ShardConsoleDisplay;

Interface ConsoleInput{
	virtual int Input() { return -1; };
};
typedef std::shared_ptr<ConsoleInput> ShardConsoleInput;
class Model {
public:

	bool Update() {
		if (G->GetLastInput() == 'w') {
			Y++;
		}
		if (G->GetLastInput() == 's') {
			Y--;
		}
		if (G->GetLastInput() == 'a') {
			X--;
		}
		if (G->GetLastInput() == 'd') {
			X++;
		}

		if (X >= W)X = W;
		if (X < 1)X = 1;
		if (Y >= H)Y = H;
		if (Y < 1)Y = 1;

		Field[Y][X] = 1;

		for (auto& oo : Field) {
			for (auto& o : oo) {
				if (o != 1) { return true; }
			}
		}

		IsEnd = true;
		return true;
	}

	bool Resize(int W_, int H_) {
		Field.clear();
		Field.resize(H_);
		for (auto& o : Field) {
			o.resize(W_,0);
		}
		W = W_;
		H = H_;
		return true;
	}

	int GetX() {
		return X;
	}

	int GetY() {
		return Y;
	}
	int GetWidth() {
		return W;
	}

	int GetHeight() {
		return H;
	}

	bool ToEnd() {
		return IsEnd;
	}

	bool SetGame(Game& In) {
		G = &In;
	}

	int Get(int X, int Y) {
		return Field[Y][X];
	}

protected:
	std::vector<std::vector<int>> Field;
	int X = 0;
	int Y = 0;
	int W = 0;
	int H = 0;
	bool IsEnd = false;
	Game* G = nullptr;
};

class CI : public ConsoleInput {
	int Input(){
		return _getch_nolock();
	}
};

class CD : public ConsoleDisplay {
	bool Draw(Game& In) {
		ESS::ClearShow();
		for (int i = 0; i < In.GetHeight(); i++) {
			for (int j = 0; j < In.GetWidth(); j++) {

				if (In.GetXY(j, i) != 0) { std::cout << '+'; }
				else {std::cout << 'A';}
				if (In.GetX() == j && In.GetY() == i) { ESS::Locate(j, i); std::cout << 'X'; }
			}
		}
		return false;
	}
};

class Game {
public:

	bool Initialize() {
		M.Resize(16, 16);
	}

	int GetXY(int X, int Y) {
		return M.Get(X, Y);
	}

	bool Update() {
		LastInput = -1;
		LastInput = SCI->Input();
		M.Update();
		SCD->Draw(*this);


		return true;
	}

	int GetLastInput() {
		return LastInput;
	}

	bool IsEnd() {
		return M.ToEnd();
	}

	bool SetConsoleInput(ShardConsoleInput In) {
		SCI = In;
		return true;
	}
	bool SetConsoleDisplay(ShardConsoleDisplay In) {
		SCD = In;
		return true;
	}

	bool Resize(int W, int H) {
		M.Resize(W, H);
		return true;
	}
	int GetX() {
		return M.GetX();
	}

	int GetY() {
		return M.GetY();
	}
	int GetWidth() {
		return M.GetWidth();
	}

	int GetHeight() {
		return M.GetHeight();
	}

protected:
	ShardConsoleDisplay SCD = std::make_shared<ConsoleDisplay>();
	ShardConsoleInput SCI = std::make_shared<ConsoleInput>();
	int LastInput = -1;
	Model M;
	};


int main() {


	Game G;
	G.SetConsoleInput(std::make_shared<CI>());
	G.SetConsoleDisplay(std::make_shared<CD>());

	while (!G.IsEnd()) {
		G.Update();
		if (G.GetLastInput() == 27) { break; }
	}

}