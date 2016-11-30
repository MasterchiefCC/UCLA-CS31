// rats.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;            // max number of rows in the arena
const int MAXCOLS = 20;            // max number of columns in the arena
const int MAXRATS = 100;           // max number of rats allowed

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
			  // type name, since it's mentioned in the Rat declaration.

class Rat
{
public:
	// Constructor
	Rat(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;
	int damage()const;
	bool ptMove();
	// Mutators
	void move();
	void addDamage();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	int    m_damage;
	int    pm;
	// TODO: You'll probably find that a rat object needs additional
	// data members to support your implementation of the behavior affected
	// by poison pellets.
};

class Player
{
public:
	// Constructor
	Player(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	string dropPoisonPellet();
	string move(int dir);
	void   setDead();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool   m_dead;
};

class Arena
{
public:
	// Constructor/destructor
	Arena(int nRows, int nCols);
	~Arena();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     ratCount() const;
	int     getCellStatus(int r, int c) const;
	int     numberOfRatsAt(int r, int c) const;
	void    display(string msg) const;

	// Mutators
	void setCellStatus(int r, int c, int status);
	bool addRat(int r, int c);
	bool addPlayer(int r, int c);
	void moveRats();

private:
	int     m_grid[MAXROWS][MAXCOLS];
	int     m_rows;
	int     m_cols;
	Player* m_player;
	Rat*    m_rats[MAXRATS];
	int     m_nRats;
	int     m_turns;
	int m_nOaR;

	// Helper functions
	void checkPos(int r, int c) const;
	bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

bool outOfGrid(const Arena& a, int r, int c);
bool isPoison(const Arena& a, int r, int c);
int numRatsAround(const Arena& a, int r, int c);
void zzz(int a, int&r, int &c);
///////////////////////////////////////////////////////////////////////////
//  Rat implementation
///////////////////////////////////////////////////////////////////////////

Rat::Rat(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** A rat must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "***** Rat created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_damage = 0;
	pm = 1;
}

int Rat::row() const
{
	return m_row;
}

int Rat::col() const
{	
	return m_col;  
}

int Rat::damage() const {
	return m_damage;
}

bool Rat::isDead() const
{
	return m_damage >= 2;
}

bool Rat::ptMove()  {
	int a = pm;
	pm = !pm;
	return a;
}

void Rat::move()
{
	if (!isDead()) //only functional robots can move
		attemptMove(*m_arena, randInt(0, NUMDIRS - 1), m_row, m_col);
}

void Rat::addDamage() {
	m_damage++;
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** The player must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_dead = false;
}

int Player::row() const
{
	return m_row; 
}

int Player::col() const
{
	return m_col;  
}

string Player::dropPoisonPellet()
{
	if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
		return "There's already a poison pellet at this spot.";
	m_arena->setCellStatus(m_row, m_col, HAS_POISON);
	return "A poison pellet has been dropped.";
}

string Player::move(int dir)
{
	bool moved = attemptMove(*m_arena, dir, m_row, m_col);

	if (!moved) //player did not move due to grid restrictions or a wall
		return "Player couldn't move; player stands.";
	else if (m_arena->numberOfRatsAt(m_row, m_col) > 0) //if player moves into a robot
	{
		this->setDead(); //set player to dead state
		return "Player walked into a rat and died.";
	}
	else
	{
		switch (dir) //player moved, so print out corresponding direction
		{
		case 0:
			return "Player moved north.";
			break;
		case 1:
			return "Player moved east.";
			break;
		case 2:
			return "Player moved south.";
			break;
		case 3:
			return "Player moved west.";
			break;
		default:
			return ""; //this case should never happen
			break;
		}
	}
}
	
bool Player::isDead() const
{
	return m_dead;
}

void Player::setDead()
{
	m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nRats = 0;
	m_turns = 0;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
	delete m_player;
	for (int a = 0; a < m_nRats; a++)
		if (m_rats[a] != NULL)
			delete m_rats[a];
}

int Arena::rows() const
{
	return m_rows; 
}

int Arena::cols() const
{
	return m_cols;  
}

Player* Arena::player() const
{
	return m_player;
}

int Arena::ratCount() const
{
	return m_nOaR;  
}

int Arena::getCellStatus(int r, int c) const
{
	checkPos(r, c);
	return m_grid[r - 1][c - 1];
}

int Arena::numberOfRatsAt(int r, int c) const
{
	int z = r;
	int count = 0;
	for (int a = 0; a < m_nRats; a++) {
		if (m_rats[a]->isDead())
			continue;
		Rat*r = m_rats[a];
		if (r->row() == z&& r->col() == c)
			count++;	
	}
	return count;
}

void Arena::display(string msg) const
{
	char displayGrid[MAXROWS][MAXCOLS];
	int r, c;

	for (int z = 0; z < MAXROWS; z++) {
		for (int k = 0; k < MAXCOLS; k++)
			displayGrid[z][k] = 0;
	}
	// Fill displayGrid with dots (empty) and stars (poison pellets)
	for (r = 1; r <= rows(); r++)
		for (c = 1; c <= cols(); c++)
			displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

	for (int a = 0; a < m_nRats; a++) {
		if (m_rats[a]->isDead())
			continue;
		int rows = m_rats[a]->row(), cols = m_rats[a]->col();
		if (displayGrid[rows - 1][cols - 1] == '9')continue;
		else if (displayGrid[rows-1][cols-1] == 'R')displayGrid[rows - 1][cols - 1] = '2';
		else if (displayGrid[rows-1][cols-1] < '9' && displayGrid[rows - 1][cols - 1] >= '2')
			displayGrid[rows-1][cols-1]++;
		else if (displayGrid[rows-1][cols-1] == '.' || displayGrid[rows - 1][cols - 1] == '*')
			displayGrid[rows - 1][cols - 1] = 'R';
	}
	// Indicate player's position
	if (m_player != nullptr)
		displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

	// Draw the grid
	clearScreen();
	for (r = 1; r <= rows(); r++)
	{
		for (c = 1; c <= cols(); c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;

	// Write message, rat, and player info
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << ratCount() << " rats remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player!" << endl;
	else if (m_player->isDead())
		cout << "The player is dead." << endl;
	cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
	checkPos(r, c);
	m_grid[r - 1][c - 1] = status;
}

bool Arena::addRat(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a rat on a spot with a poison pellet
	if (getCellStatus(r, c) != EMPTY)
		return false;

	// Don't add a rat on a spot with a player
	if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
		return false;
	
	if (m_nRats == MAXRATS)
		return 0;
	m_rats[m_nRats] = new Rat(this, r, c);
	m_nRats++;
	m_nOaR = m_nRats;
	// If there are MAXRATS existing rats, return false.  Otherwise,
	// dynamically allocate a new rat at coordinates (r,c).  Save the
	// pointer to the newly allocated rat and return true.

	// TODO:  Implement this.
	return 1;  // This implementation compiles, but is incorrect.
}

bool Arena::addPlayer(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Don't add a player on a spot with a rat
	if (numberOfRatsAt(r, c) > 0)
		return false;

	m_player = new Player(this, r, c);
	return true;
}

void Arena::moveRats()
{
	for (int a = 0; a < m_nRats; a++) {
		if (m_rats[a]->isDead())continue;
		if ((m_rats[a]->damage() == 1 && m_rats[a]->ptMove()) || m_rats[a]->damage() == 0)m_rats[a]->move();
		int r = m_rats[a]->row();
		int c = m_rats[a]->col();
		bool z = getCellStatus(r, c);
		if (z) {
			m_rats[a]->addDamage();
			m_rats[a]->ptMove();
			setCellStatus(r, c, EMPTY);
			if (m_rats[a]->isDead())m_nOaR--;
		}
		Player*p = player();
		
		if (r == p->row() && c == p->col()&&!m_rats[a]->isDead()) {p->setDead(); break;}
	}
	m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
	return (r >= 1 && r <= m_rows  &&  c >= 1 && c <= m_cols);
}

void Arena::checkPos(int r, int c) const
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
	{
		cout << "***** " << "Invalid arena position (" << r << ","
			<< c << ")" << endl;
		exit(1);
	}
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRats)
{
	if (nRats < 0)
	{
		cout << "***** Cannot create Game with negative number of rats!" << endl;
		exit(1);
	}
	if (nRats > MAXRATS)
	{
		cout << "***** Trying to create Game with " << nRats
			<< " rats; only " << MAXRATS << " are allowed!" << endl;
		exit(1);
	}
	if (rows == 1 && cols == 1 && nRats > 0)
	{
		cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
		exit(1);
	}

	// Create arena
	m_arena = new Arena(rows, cols);

	// Add player
	int rPlayer;
	int cPlayer;
	do
	{
		rPlayer = randInt(1, rows);
		cPlayer = randInt(1, cols);
	} while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
	m_arena->addPlayer(rPlayer, cPlayer);

	// Populate with rats
	while (nRats > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (r == rPlayer && c == cPlayer)
			continue;
		m_arena->addRat(r, c);
		nRats--;
	}
}

Game::~Game()
{
	delete m_arena;
}

string Game::takePlayerTurn()
{
	for (;;)
	{
		cout << "Your move (n/e/s/w/x or nothing): ";
		string playerMove;
		getline(cin, playerMove);

		Player* player = m_arena->player();
		int dir;

		if (playerMove.size() == 0)
		{
			if (recommendMove(*m_arena, player->row(), player->col(), dir))
				return player->move(dir);
			else
				return player->dropPoisonPellet();
		}
		else if (playerMove.size() == 1)
		{
			if (tolower(playerMove[0]) == 'x')
				return player->dropPoisonPellet();
			else if (decodeDirection(playerMove[0], dir))
				return player->move(dir);
		}
		cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
	}
}

void Game::play()
{
	m_arena->display("");
	int a = !m_arena->player()->isDead();
	int b = m_arena->ratCount();
	while (!m_arena->player()->isDead() && m_arena->ratCount() > 0)
	{
		string msg = takePlayerTurn();
		Player* player = m_arena->player();
		if (player->isDead())
		{
			cout << msg << endl;
			break;
		}
		m_arena->moveRats();
		m_arena->display(msg);
	}
	if (m_arena->player()->isDead())
		cout << "You lose." << endl;
	else
		cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
	if (max < min)
		swap(max, min);
	static random_device rd;
	static mt19937 generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
	switch (tolower(ch))
	{
	default:  return false;
	case 'n': dir = NORTH; break;
	case 'e': dir = EAST;  break;
	case 's': dir = SOUTH; break;
	case 'w': dir = WEST;  break;
	}
	return true;
}

bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	int tr = r;
	int tc = c;
	switch (dir){
	case 0:tr--; break;
	case 1:tc++; break;
	case 2:tr++; break;
	case 3:tc--; break;
	default:break;
	}
	if (outOfGrid(a, tr, tc))
		return 0;
	r = tr;
	c = tc;
	return 1;
}

bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	int z[4];

	for (int t = 0; t < 4; t++)z[t] = 0;

	if (outOfGrid(a, r, c+1))
		z[1] = 1; 
	if (!outOfGrid(a, r, c + 1) && a.numberOfRatsAt(r, c + 1))
		z[1] = 2;//E

	if (outOfGrid(a, r, c - 1))
		z[3] = 1;
	if (!outOfGrid(a, r, c - 1) && a.numberOfRatsAt(r, c - 1))
		z[3]=2;//W

	if (outOfGrid(a, r-1, c )) 
		z[0] = 1; 
	if (!outOfGrid(a, r-1, c ) && a.numberOfRatsAt(r-1, c ))
		z[0]=2; //N

	if (outOfGrid(a, r+1, c)) 
		z[2] = 1; 
	if (!outOfGrid(a, r+1, c ) && a.numberOfRatsAt(r+1, c))
		z[2]=2; //S

	int ok = 1;
	for (int i = 0; i < 4; i++) if (z[i]==2){ ok = 0; break; }
	if (ok) return 0;

	for (int i = 0; i < 4; i++) {
		if (z[i] == 0) {
			int rr = r, cc = c;
			zzz(i, rr, cc);
			if (!numRatsAround(a, rr, cc)) {bestDir = i; return 1;}
		}
	}

	return 0;

}
				   // Your replacement implementation should do something intelligent.
				   // You don't have to be any smarter than the following, although
				   // you can if you want to be:  If staying put runs the risk of a
				   // rat possibly moving onto the player's location when the rats
				   // move, yet moving in a particular direction puts the player in a
				   // position that is safe when the rats move, then the chosen
				   // action is to move to a safer location.  Similarly, if staying put
				   // is safe, but moving in certain directions puts the player in danger,
				   // then the chosen action should not be to move in one of the
				   // dangerous directions; instead, the player should stay put or move to
				   // another safe position.  In general, a position that may be moved to
				   // by many rats is more dangerous than one that may be moved to by
				   // few.
				   //
				   // Unless you want to, you do not have to take into account that a
				   // rat might be poisoned and thus sometimes less dangerous than one
				   // that is not.  That requires a more sophisticated analysis that
				   // we're not asking you to do.

////////////////////////////////////////////////////////////////////////////
//test()
////////////////////////////////////////////////////////////////////////////
#include <type_traits>
#include <cassert>

#define CHECKTYPE(c, f, r, a)  \
	static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
	   "FAILED: You changed the type of " #c "::" #f);  \
	[[gnu::unused]] r (c::* xxx##c##_##f) a = &c::f

void thisFunctionWillNeverBeCalled()
{
	// If the student deleted or changed the interfaces to the public
	// functions, this won't compile.  (This uses magic beyond the scope
	// of CS 31.)

	Rat r(static_cast<Arena*>(0), 1, 1);
	CHECKTYPE(Rat, row, int, () const);
	CHECKTYPE(Rat, col, int, () const);
	CHECKTYPE(Rat, isDead, bool, () const);
	CHECKTYPE(Rat, move, void, ());

	Player p(static_cast<Arena*>(0), 1, 1);
	CHECKTYPE(Player, row, int, () const);
	CHECKTYPE(Player, col, int, () const);
	CHECKTYPE(Player, isDead, bool, () const);
	CHECKTYPE(Player, dropPoisonPellet, string, ());
	CHECKTYPE(Player, move, string, (int));
	CHECKTYPE(Player, setDead, void, ());

	Arena a(1, 1);
	CHECKTYPE(Arena, rows, int, () const);
	CHECKTYPE(Arena, cols, int, () const);
	CHECKTYPE(Arena, player, Player*, () const);
	CHECKTYPE(Arena, ratCount, int, () const);
	CHECKTYPE(Arena, getCellStatus, int, (int, int) const);
	CHECKTYPE(Arena, numberOfRatsAt, int, (int, int) const);
	CHECKTYPE(Arena, display, void, (string) const);
	CHECKTYPE(Arena, setCellStatus, void, (int, int, int));
	CHECKTYPE(Arena, addRat, bool, (int, int));
	CHECKTYPE(Arena, addPlayer, bool, (int, int));
	CHECKTYPE(Arena, moveRats, void, ());

	Game g(1, 1, 1);
	CHECKTYPE(Game, play, void, ());
}

void findTheRat(const Arena& a, int& r, int& c)
{
	if (a.numberOfRatsAt(r - 1, c) == 1) r--;
	else if (a.numberOfRatsAt(r + 1, c) == 1) r++;
	else if (a.numberOfRatsAt(r, c - 1) == 1) c--;
	else if (a.numberOfRatsAt(r, c + 1) == 1) c++;
	else assert(false);
}

void doBasicTests()
{
	{
		Arena a(10, 20);
		a.addPlayer(2, 5);
		Player* pp = a.player();
		assert(pp->row() == 2 && pp->col() == 5 && !pp->isDead());
		assert(pp->move(NORTH) == "Player moved north.");
		assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
		assert(pp->move(NORTH) == "Player couldn't move; player stands.");
		assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
		pp->setDead();
		assert(pp->row() == 1 && pp->col() == 5 && pp->isDead());
	}
	{
		Arena a(10, 20);
		int r = 4;
		int c = 4;
		a.setCellStatus(r - 1, c, HAS_POISON);
		a.setCellStatus(r + 1, c, HAS_POISON);
		a.setCellStatus(r, c - 1, HAS_POISON);
		a.setCellStatus(r, c + 1, HAS_POISON);
		a.addRat(r, c);
		a.addPlayer(8, 18);
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 1);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 0);
		findTheRat(a, r, c);
		assert(a.getCellStatus(r, c) != HAS_POISON);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 1);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 0);
		findTheRat(a, r, c);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 1);
		a.setCellStatus(r - 1, c, HAS_POISON);
		a.setCellStatus(r + 1, c, HAS_POISON);
		a.setCellStatus(r, c - 1, HAS_POISON);
		a.setCellStatus(r, c + 1, HAS_POISON);
		a.moveRats();
		assert(a.ratCount() == 0 && a.numberOfRatsAt(r, c) == 0);
		assert(a.numberOfRatsAt(r - 1, c) == 0);
		assert(a.numberOfRatsAt(r + 1, c) == 0);
		assert(a.numberOfRatsAt(r, c - 1) == 0);
		assert(a.numberOfRatsAt(r, c + 1) == 0);

		for (int k = 0; k < MAXRATS / 4; k++)
		{
			a.addRat(7, 18);
			a.addRat(9, 18);
			a.addRat(8, 17);
			a.addRat(8, 19);
		}
		assert(!a.player()->isDead());
		a.moveRats();
		assert(a.player()->isDead());
	}
	cout << "Passed all basic tests" << endl;
}
///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////



int main()

{

	doBasicTests();
	return 0;
	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 5, 2);
	//Game g(3,5,100);

	// Play the game
	//g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif

bool outOfGrid(const Arena& a, int r, int c) {
	return (r<1 || r>a.rows() || c<1 || c>a.cols()); //return whether the position is out of grid
}

bool isPoison(const Arena& a, int r, int c)
{
	return (a.getCellStatus(r, c) == HAS_POISON); //return whether the position contains a poison
}

int numRatsAround(const Arena& a, int r, int c)
{
	int count = 0; //initiate counter for the amount of surrounding robots

				   //check all four positions and add up all robots (in all four directions) surrounding each position
	if (!outOfGrid(a, r - 1, c))
		count += a.numberOfRatsAt(r - 1, c);

	if (!outOfGrid(a, r + 1, c))
		count += a.numberOfRatsAt(r + 1, c);

	if (!outOfGrid(a, r, c - 1))
		count += a.numberOfRatsAt(r, c - 1);

	if (!outOfGrid(a, r, c + 1))
		count += a.numberOfRatsAt(r, c + 1);

	return count; //return the total number of robots surrounding the position
}

void zzz(int a, int&r, int &c) {
	if (a == 0)r = r - 1;
	if (a == 1)c = c + 1;
	if (a == 2)r = r - 1;
	if (a == 3)c = c - 1;
}
