#include "GameEngine.hpp"
#include <unistd.h>
#include <signal.h>

const timespec GameEngine::frameTime = {0, SEC(1) / GameEngine::FRAME_RATE};

static bool _init()
{
	initscr();
	// TODO need to be handled better
	if (COLS < GameEngine::MIN_WIDTH || LINES < GameEngine::MIN_HEIGHT)
	{
		std::cout << "Terminal minimum size is "
				  << GameEngine::MIN_WIDTH << " width & "
				  << GameEngine::MIN_HEIGHT << " height\n";
		return false;
	}

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_CYAN, COLOR_BLACK);

	cbreak();
	noecho();
	curs_set(0);
	timeout(0);
	refresh();
	srand(time(NULL));

	return true;
}

static WINDOW *createGameField()
{
	WINDOW *subwindow = newwin(GameEngine::FIELD_HEIGHT, GameEngine::FIELD_WIDTH, 0, 0);
	box(subwindow, 0, 0);

	return subwindow;
}

GameEngine::GameEngine() : _frameCount(0),
						   _running(false),
						   _didInit(_init()),
						   _gameField(createGameField()),
						   _em(this->_gameField, this->_scoreboard)
{
	this->_scoreboard.setLives(GameEngine::PLAYER_START_LIVES);
}

void GameEngine::start(void)
{
	if (this->_running == true)
		return;
	if (!this->_didInit)
		return;
	this->_running = true;
	this->_startMusic();
	while (this->_running)
		this->_mainLoop();
}

GameEngine::~GameEngine()
{
	this->_shutdown();
}

long GameEngine::getFrameCount(void) const
{
	return this->_frameCount;
}

void GameEngine::_shutdown()
{
	this->_didInit = false;
	delwin(this->_gameField);
	endwin();
}

void GameEngine::_mainLoop(void)
{
	clock_gettime(CLOCK_MONOTONIC, &this->_loopStart);

	this->_em.update(this->_frameCount);
	this->_scoreboard.update();

	if (this->_scoreboard.getLives() == 0)
	{
		this->_endMusic();
		this->_running = false;
	}

	clock_gettime(CLOCK_MONOTONIC, &this->_loopEnd);
	this->_diff = diff_ts(_loopEnd, _loopStart);
	this->_sleep = diff_ts(_diff, frameTime);
	if (!_sleep.tv_sec && _sleep.tv_nsec < SEC(1) / GameEngine::FRAME_RATE)
		nanosleep(&_sleep, NULL);
	this->_frameCount++;
}

void GameEngine::_startMusic(void)
{
	this->_soundPid = fork();

	if (!this->_soundPid)
	{
		execlp("afplay", "afplay", BGSOUND, "-v", "0.4", NULL);
		exit(0);
	}
}

void GameEngine::_endMusic(void)
{
	if (this->_soundPid)
	{
		kill(this->_soundPid, 9);
	}
}

void GameEngine::stop()
{
	this->_running = false;
}

/* Util Functions */
timespec diff_ts(const timespec &t1, const timespec &t2)
{
	int sec = std::abs(t1.tv_sec - t2.tv_sec);
	int nano = std::abs(t1.tv_nsec - t2.tv_nsec);
	timespec ret;
	ret.tv_sec = sec + nano - (nano % 1000000000);
	ret.tv_nsec = nano % 1000000000;

	return ret;
}

// Debug function to print out game frames
void printFrameCount(GameEngine &engine)
{
	printw("           ");
	move(0, 1);
	printw("#%ld", engine.getFrameCount() / 60);
	move(1, 1);
	printw("#%ld", engine.getFrameCount());
}
