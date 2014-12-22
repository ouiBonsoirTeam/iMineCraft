#pragma once

#include <vector>

#include "Chunk.hpp"
#include <glimac/glm.hpp>

typedef std::vector<Chunk *> ChunkList;

class ChunkManager
{
	ChunkList m_vpChunkLoadList;
	ChunkList m_vpChunkSetupList;
	ChunkList m_vpChunkRebuildList;
	ChunkList m_vpChunkUnloadList;
	ChunkList m_vpChunkVisibilityList;
	ChunkList m_vpChunkRenderList;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraView;
	bool m_forceVisibilityUpdate;

	const static int NUM_CHUNKS_PER_FRAME = 3;

public:
	ChunkManager(){};
	~ChunkManager(){};

	// Update fonctions
	void update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView);
	void updateLoadList();
	void updateunLoadList();
	void updateSetupList();
	void updateRebuildList();
	void updateVisibilityList();
	void updateUnloadList();
	void updateRenderList();

	void loadAndParseJsonFile(const std::string& fileName);
};