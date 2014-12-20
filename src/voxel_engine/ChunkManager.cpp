#include "ChunkManager.hpp"


void ChunkManager::UpdateLoadList()
{
    int lNumOfChunksLoaded = 0;

    ChunkList::iterator iterator;   
    for(iterator = m_vpChunkLoadList.begin(); iterator != m_vpChunkLoadList.end() &&
                  (lNumOfChunksLoaded != ASYNC_NUM_CHUNKS_PER_FRAME); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->IsLoaded() == false)
        {
            if(lNumOfChunksLoaded != ASYNC_NUM_CHUNKS_PER_FRAME))
            {
                pChunk->Load();

                // Increase the chunks loaded count
                lNumOfChunksLoaded++;

                m_forceVisibilityUpdate = true;
            }
        }
    }

    // Clear the load list (every frame)
    m_vpChunkLoadList.clear();
}


void ChunkManager::Update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView)
{
    UpdateAsyncChunker();

    UpdateLoadList();

    UpdateSetupList();

    UpdateRebuildList();

    UpdateFlagsList();

    UpdateUnloadList();

    UpdateVisibilityList(cameraPosition);
	
    // Test if camera have moved
    if(m_cameraPosition != cameraPosition || m_cameraView != cameraView)
    {
        UpdateRenderList();
    }

    m_cameraPosition = cameraPosition;
    m_cameraView = cameraView;
}