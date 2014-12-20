#pragma once

#include <glimac/glm.hpp>

class ChunkManager
{
	std::vector<Chunk*> m_vpChunkLoadList;
	std::vector<Chunk*> m_vpChunkSetupList;
	std::vector<Chunk*> m_vpChunkRebuildList;
	std::vector<Chunk*> m_vpChunkUnloadList;
	std::vector<Chunk*> m_vpChunkVisibilityList;
	std::vector<Chunk*> m_vpChunkRenderList;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraView;
	bool m_forceVisibilityUpdate;

	const static int NUM_CHUNKS_PER_FRAME = 3;

public:
	ChunkManager();
	~ChunkManager();

	void update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView);
	void updateLoadList();
	void updateSetupList();
	void updateRebuildList();
};