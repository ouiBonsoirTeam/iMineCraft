#include "ChunkManager.hpp"
#include <iostream>
#include <fstream>


void ChunkManager::initialize(const std::string& saveFolder)
{
    m_pathJson = saveFolder;
    int chunkLimit = 2;
    int chunkCount = 0;

    for (int i = 0; i < chunkLimit; ++i)
    {
        for (int j = 0; j < chunkLimit; ++j)
        {
            for (int k = 0; k < chunkLimit; ++k)
            {
                m_vpGlobalChunkList.push_back(new Chunk(glm::vec3(i, j, k)));
                m_vpChunkLoadList.push_back(m_vpGlobalChunkList.at(chunkCount));

                ++chunkCount;
            }
        }
    }
}

void ChunkManager::updateLoadList()
{
    int lNumOfChunksLoaded = 0;

    ChunkList::iterator iterator;   

    for(iterator = m_vpChunkLoadList.begin(); iterator != m_vpChunkLoadList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->isLoaded() == false)
        {
            // if(lNumOfChunksLoaded != NUM_CHUNKS_PER_FRAME)
            // {
                
                glm::vec3 p = pChunk->getPosition();
                std::string filePath = m_pathJson + "/chunk_" + std::to_string((int)p[0]) + "_" + std::to_string((int)p[1]) + "_" + std::to_string((int)p[2]) + ".json";

                if(jsonChunkExist(filePath))
                {
                    Json::Value data = loadAndParseJsonFile(filePath);
                    pChunk->load(data);
                }
                    
                // Increase the chunks loaded count
                lNumOfChunksLoaded++;

                // Ajouter un test de distance pour juger la nécessité du setup dans l'immédiat
                m_vpChunkSetupList.push_back(pChunk);

                m_forceVisibilityUpdate = true;
            // }
        }
    }

    // Clear the load list (every frame)
    m_vpChunkLoadList.clear();
}


void ChunkManager::update(/*float dt, */glm::vec3 cameraPosition, glm::vec3 cameraView)
{
    // updateAsyncChunker();

    updateLoadList();

    updateSetupList();

    updateRebuildList();

    updateFlagsList();

    // updateUnloadList();

    updateVisibilityList(cameraPosition);
	
    // Test if camera have moved
    if(m_cameraPosition != cameraPosition || m_cameraView != cameraView)
    {
        updateRenderList();
        
        m_cameraPosition = cameraPosition;
        m_cameraView = cameraView;
    }

    
}

void ChunkManager::updateSetupList()
{
    // Setup any chunks that have not already been setup
    ChunkList::iterator iterator;
    for(iterator = m_vpChunkSetupList.begin(); iterator != m_vpChunkSetupList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->isSetup() == false)
        {
            pChunk->setup();
            // Only force the visibility update if we actually setup the chunk, some chunks wait in the pre-setup stage...
            m_forceVisibilityUpdate = true;

            m_vpChunkRebuildList.push_back(pChunk);
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
    for(iterator = m_vpChunkRebuildList.begin(); iterator != m_vpChunkRebuildList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->isLoaded() && pChunk->isSetup())
        {
            // if(lNumRebuiltChunkThisFrame != NUM_CHUNKS_PER_FRAME)
            // {
                pChunk->buildMesh();

                m_vpChunkUpdateFlagsList.push_back(pChunk);

                // Also add our neighbours since they might now be surrounded too (If we have neighbours)
                Chunk* pChunkXMinus = getChunk(pChunk->getX()-1, pChunk->getY(), pChunk->getZ());
                Chunk* pChunkXPlus = getChunk(pChunk->getX()+1, pChunk->getY(), pChunk->getZ());
                Chunk* pChunkYMinus = getChunk(pChunk->getX(), pChunk->getY()-1, pChunk->getZ());
                Chunk* pChunkYPlus = getChunk(pChunk->getX(), pChunk->getY()+1, pChunk->getZ());
                Chunk* pChunkZMinus = getChunk(pChunk->getX(), pChunk->getY(), pChunk->getZ()-1);
                Chunk* pChunkZPlus = getChunk(pChunk->getX(), pChunk->getY(), pChunk->getZ()+1);

                if(pChunkXMinus != NULL) 
                    m_vpChunkUpdateFlagsList.push_back(pChunkXMinus);

                if(pChunkXPlus != NULL) 
                    m_vpChunkUpdateFlagsList.push_back(pChunkXPlus);

                if(pChunkYMinus != NULL) 
                    m_vpChunkUpdateFlagsList.push_back(pChunkYMinus);

                if(pChunkYPlus != NULL) 
                    m_vpChunkUpdateFlagsList.push_back(pChunkYPlus);

                if(pChunkZMinus != NULL) 
                    m_vpChunkUpdateFlagsList.push_back(pChunkZMinus);

                if(pChunkZPlus != NULL) 
                    m_vpChunkUpdateFlagsList.push_back(pChunkZPlus);
                
                // Only rebuild a certain number of chunks per frame
                lNumRebuiltChunkThisFrame++;

                m_forceVisibilityUpdate = true;
            // }
        }
    }

    // Clear the rebuild list
    m_vpChunkRebuildList.clear();
}

void ChunkManager::updateFlagsList(){
    ChunkList::iterator iterator;
    int lNumUpdateFlagsThisFrame = 0;
    for(iterator = m_vpChunkUpdateFlagsList.begin(); iterator != m_vpChunkUpdateFlagsList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->isLoaded() && pChunk->isSetup())
        {
            // if(lNumUpdateFlagsThisFrame != NUM_CHUNKS_PER_FRAME)
            // {
                pChunk->updateShouldRenderFlags();

                m_vpChunkVisibilityList.push_back(pChunk);

                // Only rebuild a certain number of chunks per frame
                lNumUpdateFlagsThisFrame++;

                m_forceVisibilityUpdate = true;
            // }
        }
    }
    // Clear the rebuild list
    m_vpChunkUpdateFlagsList.clear();
}


void ChunkManager::updateUnloadList()
{
    // Unload any chunks
    ChunkList::iterator iterator;
    for(iterator = m_vpChunkUnloadList.begin(); iterator != m_vpChunkUnloadList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->isLoaded())
        {
            pChunk->unload();

            m_forceVisibilityUpdate = true;
        }
    }
    // Clear the unload list (every frame)
    m_vpChunkUnloadList.clear();
}

void ChunkManager::updateVisibilityList(glm::vec3 cameraPosition)
{
    if(m_forceVisibilityUpdate)
    {
        ChunkList::iterator iterator;
        for(iterator = m_vpChunkVisibilityList.begin(); iterator != m_vpChunkVisibilityList.end(); ++iterator)
        {
            Chunk* pChunk = (*iterator);

            if(pChunk->isLoaded() && pChunk->isSetup())
            {
                if (pChunk->isEmpty()) 
                {
                    m_vpChunkVisibilityList.erase(iterator);
                    --iterator;
                }
                // A FAIRE: Tester les Chunks visibles avec la position de la caméra
            }
        }
    }
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
                //     float c_offset = (Chunk::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE) - Block::BLOCK_RENDER_SIZE;
                //     glm::vec3 chunkCenter = pChunk->getPosition() + glm::vec3(c_offset, c_offset, c_offset);

                //     float c_size = Chunk::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE;
                //     if(m_pRenderer->CubeInFrustum(m_pRenderer->GetActiveViewPort(), chunkCenter, c_size, c_size, c_size))
                //     {
                //         m_vpChunkRenderList.push_back(pChunk);
                //     }
                // }
                m_vpChunkRenderList.push_back(pChunk);
            }
        }
    } 
        
}

void ChunkManager::render(GeneralProgram &program, const glm::mat4 viewMatrix){
    ChunkList::iterator iterator;

    for(iterator = m_vpChunkRenderList.begin(); iterator != m_vpChunkRenderList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        pChunk->render(program, viewMatrix);
    }
}

bool ChunkManager::jsonChunkExist(const std::string &fileName)
{
    std::ifstream file;
    file.open(fileName);

    if (file.is_open())
    {
        file.close();
        return true;
    }    
    else
        return false;
}


Json::Value ChunkManager::loadAndParseJsonFile(const std::string& fileName){
    std::ifstream file;
    file.open(fileName);
    std::string str, contents;

    if (file.is_open())
    {
        while (std::getline(file, str))
        {
            contents += str;
        }  
        file.close();

        Json::Value root;
        Json::Reader reader;

        bool parsingSuccessful = reader.parse(contents, root);
        if ( !parsingSuccessful )
        {
            // report to the user the failure and their locations in the document.
            std::cerr  << "Failed to parse configuration\n"
                       << reader.getFormattedErrorMessages();
            exit(1);
        }
        else{
            std::cout << "Fichier chargé" << std::endl;
            return root;
        }

    }
    else std::cerr << "Unable to open file";
    exit(1);  
}

Chunk* ChunkManager::getChunk(const int &x, const int &y, const int &z){
    for (unsigned int i = 0; i < m_vpGlobalChunkList.size(); ++i)
    {
        if (m_vpGlobalChunkList.at(i)->getX() == x &&
            m_vpGlobalChunkList.at(i)->getY() == y &&
            m_vpGlobalChunkList.at(i)->getZ() == z)

            return m_vpGlobalChunkList.at(i);
    }

    return NULL;
}