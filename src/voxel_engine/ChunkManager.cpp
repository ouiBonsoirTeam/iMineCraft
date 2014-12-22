#include "ChunkManager.hpp"
#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>

void ChunkManager::updateLoadList()
{
    int lNumOfChunksLoaded = 0;

    ChunkList::iterator iterator;   
    for(iterator = m_vpChunkLoadList.begin(); iterator != m_vpChunkLoadList.end() &&
                  (lNumOfChunksLoaded != NUM_CHUNKS_PER_FRAME); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->isLoaded() == false)
        {
            if(lNumOfChunksLoaded != NUM_CHUNKS_PER_FRAME)
            {
                pChunk->load();

                // Increase the chunks loaded count
                lNumOfChunksLoaded++;

                m_forceVisibilityUpdate = true;
            }
        }
    }

    // Clear the load list (every frame)
    m_vpChunkLoadList.clear();
}


void ChunkManager::update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView)
{
    // updateAsyncChunker();

    // updateLoadList();

    // updateSetupList();

    // updateRebuildList();

    // updateFlagsList();

    // updateUnloadList();

    // updateVisibilityList(cameraPosition);
	
    // Test if camera have moved
    if(m_cameraPosition != cameraPosition || m_cameraView != cameraView)
    {
        //updateRenderList();
    }

    m_cameraPosition = cameraPosition;
    m_cameraView = cameraView;
}

void ChunkManager::updateSetupList()
{
    // Setup any chunks that have not already been setup
    ChunkList::iterator iterator;
    for(iterator = m_vpChunkSetupList.begin(); iterator != m_vpChunkSetupList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->isLoaded() && pChunk->isSetup() == false)
        {
            pChunk->setup();
            // Only force the visibility update if we actually setup the chunk, some chunks wait in the pre-setup stage...
            m_forceVisibilityUpdate = true;
        }
    }

    // Clear the setup list (every frame)
    m_vpChunkSetupList.clear();
}

void ChunkManager::updateRebuildList()
{
    // Rebuild any chunks that are in the rebuild chunk list
    ChunkList::iterator iterator;
    int lNumRebuiltChunkThisFrame = 0;
    for(iterator = m_vpChunkRebuildList.begin(); iterator != m_vpChunkRebuildList.end() &&
                  (lNumRebuiltChunkThisFrame != NUM_CHUNKS_PER_FRAME); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->isLoaded() && pChunk->isSetup())
        {
            if(lNumRebuiltChunkThisFrame != NUM_CHUNKS_PER_FRAME)
            {
                pChunk->rebuildMesh();

                // Only rebuild a certain number of chunks per frame
                lNumRebuiltChunkThisFrame++;

                m_forceVisibilityUpdate = true;
            }
        }
    }

    // Clear the rebuild list
    m_vpChunkRebuildList.clear();
}


void ChunkManager::updateUnloadList()
{}

void ChunkManager::updateVisibilityList()
{

}

void ChunkManager::updateRenderList()
{
    // Clear the render list each frame BEFORE we do our tests to see what chunks should be rendered
    m_vpChunkRenderList.clear();

    ChunkList::iterator iterator;
    for(iterator = m_vpChunkVisibilityList.begin(); iterator != m_vpChunkVisibilityList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk != NULL)
        {
            if(pChunk->isLoaded() && pChunk->isSetup())
            {
                // if(pChunk->ShouldRender()) // Early flags check so we don't always have to do the frustum check...
                // {
                //     // Check if this chunk is inside the camera frustum
                //     float c_offset = (Chunk::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE)-Block::BLOCK_RENDER_SIZE;
                //     glm::vec3 chunkCenter = pChunk->getPosition() + glm::vec3(c_offset, c_offset, c_offset);

                //     float c_size = Chunk::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE;
                //     if(m_pRenderer->CubeInFrustum(m_pRenderer->GetActiveViewPort(), chunkCenter, c_size, c_size, c_size))
                //     {
                //         m_vpChunkRenderList.push_back(pChunk);
                //     }
                // }
            }
        }
    }
        
}

void ChunkManager::loadJsonFile(const std::string& fileName ){
    std::ifstream file;
    file.open(fileName);
    std::string str, contents;

     Json::Value root;   // will contains the root value after parsing
    Json::Reader reader;

    if (file.is_open())
    {
        while (std::getline(file, str))
        {
            contents += str;
        }  
        file.close();
    }
    else std::cout << "Unable to open file";

    bool parsingSuccessful = reader.parse(contents, root);
    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "Failed to parse configuration\n"
                   << reader.getFormattedErrorMessages();
        return;
    }
    else
        std::cout << "Fichier chargé" << std::endl;
}

