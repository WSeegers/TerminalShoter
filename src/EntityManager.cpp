#include "EntityManager.hpp"

PlayerEntity makeDefaultPlayer()
{
	std::string l0("#<x> ");
	std::string l1(" =x=>");
	std::string l2("#<x> ");

	std::string rawBody = (l0 + l1 + l2);
	Body playerBody(rawBody, 5, 3);
	return PlayerEntity(Vec2(3, 10), playerBody);
};

EntityManager::EntityManager() : _player(makeDefaultPlayer())
{
	int i;

	for (i = 0; i < EntityManager::PLAYER_PROJECTILE_MAX; i++)
	{
		this->_playerProjectilesPool[i] = new Projectile(
				Vec2(),
				Vec2(0.7, 0),
				Body(std::string("O"), 1, 1));
		this->_playerProjectilesPool[i]->kill();
	}
}

EntityManager::~EntityManager()
{
	int i;

	for (i = 0; i < EntityManager::PLAYER_PROJECTILE_MAX; i++)
	{
		delete this->_playerProjectilesPool[i];
	}
}

void EntityManager::update()
{
	this->updateProjectiles();
	this->updatePlayer();

	this->drawProjectiles();
	this->drawPlayer();
	refresh();
}

void EntityManager::_createPlayerShot()
{
	int i;
	Projectile *projectile;

	for (i = 0; i < EntityManager::PLAYER_PROJECTILE_MAX; i++)
	{
		projectile = _playerProjectilesPool[i];
		if (!projectile->isAlive())
		{
			projectile->revive();
			projectile->setPosition(this->_player.getPosition() + Vec2(5, 1));
			break;
		}
	}
}

void EntityManager::updateProjectiles()
{
	int i;
	Projectile *projectile;

	for (i = 0; i < EntityManager::PLAYER_PROJECTILE_MAX; i++)
	{
		projectile = _playerProjectilesPool[i];
		if (projectile->isAlive())
		{

			this->_removeBody(projectile->getPosition().y,
												projectile->getPosition().x,
												*projectile);
			projectile->update();
		}
	}
}

void EntityManager::drawProjectiles()
{
	int i;
	Projectile *projectile;

	for (i = 0; i < EntityManager::PLAYER_PROJECTILE_MAX; i++)
	{
		projectile = _playerProjectilesPool[i];
		if (projectile->isAlive())
		{

			this->_drawBody(projectile->getPosition().y,
											projectile->getPosition().x,
											*projectile);
		}
	}
}

void EntityManager::updatePlayer()
{
	this->_removeBody(this->_player.getPosition().y,
										this->_player.getPosition().x,
										this->_player);

	switch (getch())
	{
	case 'w':
		this->_player.moveUP();
		break;
	case 'a':
		this->_player.moveLEFT();
		break;
	case 's':
		this->_player.moveDOWN();
		break;
	case 'd':
		this->_player.moveRIGHT();
		break;
	case ' ':
		this->_createPlayerShot();
	}
}

void EntityManager::drawPlayer()
{
	this->_drawBody(this->_player.getPosition().y,
									this->_player.getPosition().x,
									this->_player);
}

void EntityManager::_drawBody(int y, int x, const Body &body)
{
	int _cy = -1;
	int _cx;
	char bodyPart;

	int width = body.getWidth();
	int height = body.getHeight();
	const std::string &_body = body.getBody();

	while (++_cy < height)
	{
		_cx = -1;
		while (++_cx < width)
		{
			bodyPart = _body[_cx + _cy * width];
			if (bodyPart != ' ')
				mvaddch(
						_cy + y,
						_cx + x,
						bodyPart);
		}
	}
}

void EntityManager::_removeBody(int y, int x, const Body &body)
{
	int _cy = -1;
	int _cx;
	char bodyPart;

	int width = body.getWidth();
	int height = body.getHeight();
	const std::string &_body = body.getBody();

	while (++_cy < height)
	{
		_cx = -1;
		while (++_cx < width)
		{
			bodyPart = _body[_cx + _cy * width];
			if (bodyPart != ' ')
				mvaddch(
						_cy + y,
						_cx + x,
						' ');
		}
	}
}
