#pragma once

#include <vector>

#include "Chunk.hpp"
#include <glimac/glm.hpp>
#include <json/json.h>

typedef std::vector<Chunk*> ChunkList;

class ChunkManager
{
	ChunkList m_vpGlobalChunkList;

	ChunkList m_vpChunkLoadList;
	ChunkList m_vpChunkSetupList;
	ChunkList m_vpChunkRebuildList;
	ChunkList m_vpChunkUnloadList;
	ChunkList m_vpChunkVisibilityList;
	ChunkList m_vpChunkRenderList;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraView;
	bool m_forceVisibilityUpdate;

	std::string m_pathJson;

	const static int NUM_CHUNKS_PER_FRAME = 3;

public:
	ChunkManager(){};
	~ChunkManager(){};

	// Update fonctions
	void initialize(const std::string& saveFolder);
	void update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView);
	void updateLoadList();
	void updateUnLoadList();
	void updateSetupList();
	void updateRebuildList();
	void updateVisibilityList();
	void updateUnloadList();
	void updateRenderList();

	bool jsonChunkExist(const std::string &fileName);
	Json::Value loadAndParseJsonFile(const std::string& fileName);
};