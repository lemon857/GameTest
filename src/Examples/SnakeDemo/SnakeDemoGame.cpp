#include "SnakeDemoGame.h"

#include <glm/vec4.hpp>

#include "../../Resources/ResourceManager.h"

static SnakeDemoGame* g_pThis;

SnakeDemoGame::SnakeDemoGame(const glm::ivec2& windowSize, const glm::ivec2& cellSize)
	: IGame(windowSize)
	, m_cellSize(cellSize)
	, m_cellPlaceSize(windowSize.x / cellSize.x + 1, windowSize.y / cellSize.y)
	, m_eDir(Physics::Up)
	, m_pTimer(std::make_shared<Timer>())
	, m_status(Active)
{
	std::shared_ptr<SnakePart> part1 = std::make_shared<SnakePart>(glm::ivec2(rand() % (m_cellPlaceSize.x - 1), rand() % (m_cellPlaceSize.y - 1)));
	m_posApple = glm::ivec2(rand() % (m_cellPlaceSize.x - 1), rand() % (m_cellPlaceSize.y - 1));
	m_snakeParts.push_back(std::move(part1));
	m_pTimer->setCallback(onTick);
	g_pThis = this;
}

void SnakeDemoGame::render() const
{
	if (m_status == Loose)
	{
		m_pSpriteEnd1->render(glm::vec2(m_cellSize.x * ((m_cellPlaceSize.x / 2) - 1), m_cellSize.y * ((m_cellPlaceSize.y / 2) - 1)), m_cellSize, 0, 2);
		m_pSpriteEnd2->render(glm::vec2(m_cellSize.x * ((m_cellPlaceSize.x / 2)), m_cellSize.y * ((m_cellPlaceSize.y / 2) - 1)), m_cellSize, 0, 2);
	}
	/*for (size_t i = 0; i < m_cellPlaceSize.x; i++)
	{
		for (size_t j = 0; j < m_cellPlaceSize.y; j++)
		{
			m_pLine->render(glm::vec2(), glm::vec2(i * m_cellSize.x, 0.f), 1, glm::vec4(1));
			m_pLine->render(glm::vec2(1), glm::vec2(0.f, j * m_cellSize.y), 1, glm::vec4(1));
			m_pLine->render(glm::vec2(i * m_cellSize.x, 0.f), glm::vec2(0.f, j * m_cellSize.y), 1, glm::vec4(1));
			m_pLine->render(glm::vec2(1.f, j * m_cellSize.y), glm::vec2(i * m_cellSize.x, 0.f), 1, glm::vec4(1));
		}
	}*/
	for (size_t i = 0; i < m_snakeParts.size(); i++)
	{
		glm::ivec2 pos = m_snakeParts[i]->getPosition();
		m_pSprite->render(glm::vec2(pos.x * m_cellSize.x, pos.y * m_cellSize.y), m_cellSize, 0, 1);
	}
	m_pSpriteApple->render(glm::vec2(m_posApple.x * m_cellSize.x, m_posApple.y * m_cellSize.y), m_cellSize, 0, 0);
}

void SnakeDemoGame::update(const double delta)
{
	if (m_status == Loose)
	{
		if (m_keys[GLFW_KEY_ENTER])
		{
			m_snakeParts.clear();
			std::shared_ptr<SnakePart> part1 = std::make_shared<SnakePart>(glm::ivec2(rand() % (m_cellPlaceSize.x - 1), rand() % (m_cellPlaceSize.y - 1)));

			m_posApple = glm::ivec2(rand() % (m_cellPlaceSize.x - 1), rand() % (m_cellPlaceSize.y - 1));

			m_snakeParts.push_back(std::move(part1));
			m_status = Active;
		}
		return;
	}
	if (m_keys[GLFW_KEY_W])
	{
		if (g_pThis->m_eDir != Physics::Down) m_eDir = Physics::Up;
	}
	else if (m_keys[GLFW_KEY_A])
	{
		if (g_pThis->m_eDir != Physics::Right) m_eDir = Physics::Left;
	}
	else if (m_keys[GLFW_KEY_S])
	{
		if (g_pThis->m_eDir != Physics::Up) m_eDir = Physics::Down;
	}
	else if (m_keys[GLFW_KEY_D])
	{
		if (g_pThis->m_eDir != Physics::Left) m_eDir = Physics::Right;
	}
	m_pTimer->update(delta);
}

bool SnakeDemoGame::init()
{
	ResourceManager::loadJSONresources("res/resources.json");

	auto pShapeShaderProgram = ResourceManager::getShaderProgram("shapeShader");
	auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");

	m_cam = std::make_shared<Camera>(glm::vec2(0) + (glm::vec2)(m_WindowSize / 2), m_WindowSize, -100.f, 100.f);

	m_cam->addShaderProgram(pSpriteShaderProgram);
	m_cam->addShaderProgram(pShapeShaderProgram);

	m_pLine = std::make_shared<RenderEngine::Line>(pShapeShaderProgram);

	m_pSprite = ResourceManager::getSprite("SnakePart");
	m_pSpriteApple = ResourceManager::getSprite("Apple");
	m_pSpriteEnd1 = ResourceManager::getSprite("SnakeEnd1");
	m_pSpriteEnd2 = ResourceManager::getSprite("SnakeEnd2");

	m_pTimer->start(400);

	return true;
}

void SnakeDemoGame::checkDead()
{
	glm::ivec2 pos = m_snakeParts[0]->getPosition();
	switch (m_eDir)
	{
	case Physics::Up:
		if (pos.y >= m_cellPlaceSize.y - 1) m_status = Loose;
		break;
	case Physics::Down:
		if (pos.y + 1 <= 0) m_status = Loose;
		break;
	case Physics::Left:
		if (pos.x + 1 <= 0) m_status = Loose;
		break;
	case Physics::Right:
		if (pos.x >= m_cellPlaceSize.x - 1) m_status = Loose;
		break;
	}
	for (size_t i = 1; i < m_snakeParts.size(); i++)
	{
		glm::ivec2 curPos = m_snakeParts[i]->getPosition();
		if (pos == curPos) m_status = Loose;
	}
}

void SnakeDemoGame::checkApple()
{
	glm::ivec2 pos = m_snakeParts[0]->getPosition();
	if (pos == m_posApple)
	{
		std::shared_ptr<SnakePart> part = std::make_shared<SnakePart>(pos);
		m_snakeParts[m_snakeParts.size() - 1]->addNext(part);
		m_snakeParts.push_back(std::move(part));
		m_posApple = glm::ivec2(rand() % (m_cellPlaceSize.x - 1), rand() % (m_cellPlaceSize.y - 1));

		for (size_t i = 0; i < m_snakeParts.size(); i++)
		{
			glm::ivec2 curPos = m_snakeParts[i]->getPosition();
			if (m_posApple == curPos) m_posApple = glm::ivec2(rand() % (m_cellPlaceSize.x - 1), rand() % (m_cellPlaceSize.y - 1));
		}
	}
}

void SnakeDemoGame::onTick()
{
	glm::ivec2 pos = g_pThis->m_snakeParts[0]->getPosition();
	switch (g_pThis->m_eDir)
	{
	case Physics::Up:
		g_pThis->m_snakeParts[0]->setPostion(glm::ivec2(pos.x, pos.y + 1));
		break;
	case Physics::Down:
		g_pThis->m_snakeParts[0]->setPostion(glm::ivec2(pos.x, pos.y - 1));
		break;
	case Physics::Left:
		g_pThis->m_snakeParts[0]->setPostion(glm::ivec2(pos.x - 1, pos.y));
		break;												  
	case Physics::Right:									 
		g_pThis->m_snakeParts[0]->setPostion(glm::ivec2(pos.x + 1, pos.y));
		break;
	}
	g_pThis->checkDead();
	g_pThis->checkApple();
	g_pThis->m_pTimer->start(400);
}
