#pragma once 

#include <vector> 
#include <string> 
#include <memory> 

#include <glm/vec2.hpp>

class IGameObject;

class Level
{
public:
	Level( const std::vector<std::string>& levelDescription );
	void Render() const;
	void Update( double delta );

	inline size_t GetWidth() const { return( width + 3 ) * BLOCK_SIZE; }
	inline size_t GetHeight() const { return( height + 1 ) * BLOCK_SIZE; }

	inline const glm::ivec2& GetPlayerRespawn_1Pos() const { return playerRespawn_1Pos; }
	inline const glm::ivec2& GetPlayerRespawn_2Pos() const { return playerRespawn_2Pos; }
	inline const glm::ivec2& GetEnemyRespawn_1Pos() const { return enemyRespawn_1Pos; }
	inline const glm::ivec2& GetEnemyRespawn_2Pos() const { return enemyRespawn_2Pos; }
	inline const glm::ivec2& GetEnemyRespawn_3Pos() const { return enemyRespawn_3Pos; }

private:
	std::shared_ptr<IGameObject> CreateGameObjectFromDescription( char description, const glm::vec2& position, const glm::vec2& size, float rotation );

public:
	static constexpr uint32_t BLOCK_SIZE = 16u;

private:
	size_t width = 0;
	size_t height = 0;
	glm::ivec2 playerRespawn_1Pos;
	glm::ivec2 playerRespawn_2Pos;
	glm::ivec2 enemyRespawn_1Pos;
	glm::ivec2 enemyRespawn_2Pos;
	glm::ivec2 enemyRespawn_3Pos;	

	std::vector<std::shared_ptr<IGameObject>> levelObjects;
};