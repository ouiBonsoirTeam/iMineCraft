#pragma once

#include <vector>

#include "Chunk.hpp"
#include <glimac/glm.hpp>
#include <json/json.h>
#include <glimac/PerlinNoise.hpp>

typedef std::vector<Chunk*> ChunkList;

class ChunkManager
{
	ChunkList m_vpGlobalChunkList;

	ChunkList m_vpChunkLoadList;
	ChunkList m_vpChunkSetupList;
	ChunkList m_vpChunkRebuildList;
	ChunkList m_vpChunkUpdateFlagsList;
	ChunkList m_vpChunkUnloadList;
	ChunkList m_vpChunkVisibilityList;
	ChunkList m_vpChunkRenderList;

	PerlinNoise m_PerlinNoise;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraView;

	bool m_forceVisibilityUpdate;

	std::string m_pathJson;

	const static int NUM_CHUNKS_PER_FRAME = 3;

public:
	ChunkManager(){};
	~ChunkManager(){};

	// Getters
	float getNoiseValue(const int & x, const int & z) { return (float) m_PerlinNoise.GetHeight(x, z); }

	// Update fonctions
	void initialize(const std::string& saveFolder);
	void updateAsyncChunker(glm::vec3 cameraPosition, glm::vec3 cameraView);
	bool chunkExist(const glm::vec3 &position);
	void update(/*float dt, */glm::vec3 cameraPosition, glm::vec3 cameraView);
	void updateLoadList();
	void updateUnLoadList();
	void updateSetupList();
	void updateRebuildList();
	void updateFlagsList();
	void updateVisibilityList(glm::vec3 cameraPosition);
	void updateUnloadList();
	void updateRenderList();
	void render(DirectionalLightProgram &program, const glm::mat4 viewMatrix);

	void addChunkToRebuildList(Chunk * chunk);


	bool jsonChunkExist(const std::string &fileName);
	Json::Value loadAndParseJsonFile(const std::string& fileName);

	Chunk* getChunk(const int &x, const int &y, const int &z);
};