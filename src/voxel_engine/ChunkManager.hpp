#pragma once

#include <vector>
#include <glimac/glm.hpp>
#include <json/json.h>
#include <glimac/PerlinNoise.hpp>

#include "Chunk.hpp"

typedef std::vector<Chunk*> ChunkList;

class ChunkManager
{
	// Vectors of Chunk
	ChunkList m_vpGlobalChunkList;

	ChunkList m_vpChunkLoadList;
	ChunkList m_vpChunkSetupList;
	ChunkList m_vpChunkRebuildList;
	ChunkList m_vpChunkUpdateFlagsList;
	ChunkList m_vpChunkUnloadList;
	ChunkList m_vpChunkVisibilityList;
	ChunkList m_vpChunkRenderList;

	// Perlin Noise for terrain generation
	PerlinNoise m_PerlinNoise;

	// Camera data
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraView;

	// If we need do some visibility tests
	bool m_forceVisibilityUpdate;

	// Json files folder path
	std::string m_pathJson;

public:
	// Constructor
	ChunkManager(){};

	// Getters
	float getNoiseValue(const int & x, const int & z) { return (float) m_PerlinNoise.GetHeight(x, z); }
	Chunk* getChunk(const int &x, const int &y, const int &z);
	Chunk* getChunk(const glm::vec3 &pos);
	bool chunkExist(const glm::vec3 &position);

	// Initialization
	void initialize(const std::string& saveFolder);

	// Update fonctions
	void updateAsyncChunker(glm::vec3 cameraPosition, glm::vec3 cameraView);
	void updateLoadList();
	void updateUnloadList();
	void updateSetupList();
	void updateRebuildList();
	void updateFlagsList();
	void updateVisibilityList(glm::vec3 cameraPosition);
	void updateRenderList();
	void update(glm::vec3 cameraPosition, glm::vec3 cameraView);

	// Rendering
	void render(LightsProgram &program, const glm::mat4 viewMatrix);

	// Rebuild Chunk if is modified
	void addChunkToRebuildList(Chunk * chunk);

	// Load and save terrain
	int loadTerrain(const std::string & saveFolder);
	void saveTerrain(const unsigned int & perlin_seed);

	// Json functions
	bool jsonChunkExist(const std::string &fileName);
	Json::Value loadAndParseJsonFile(const std::string& fileName);

	// Global unloading
	void unloadWorld();

	// Destructor
	~ChunkManager();
};