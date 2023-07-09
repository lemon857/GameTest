#include "Game.h"

Game::Game() 
	: m_eCurrentGameState(EGameState::Active)
{
	m_keys.fill(false);
}
Game::~Game()
{

}

void Game::render()
{

}
void Game::update(const uint64_t delta)
{

}
void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}
bool Game::init()
{
	return false;
}