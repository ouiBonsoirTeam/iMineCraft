#include "ChunkManager.hpp"
#include <iostream>
#include <fstream>

ChunkManager::~ChunkManager(){
    ChunkList::iterator iterator;   

    for(iterator = m_vpGlobalChunkList.begin(); iterator != m_vpGlobalChunkList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);
        delete pChunk;
    }

};

void ChunkManager::initialize(const std::string& saveFolder)
{
    double amplitude = 64;
    double persistence = 0.01;
    double frequency = 0.05;
    double octaves = 1 ;

    srand(time(NULL));
    double randomseed = rand() % 1990;

    if(jsonChunkExist(saveFolder + "terrain.json"))
        randomseed = loadTerrain(saveFolder);

    m_PerlinNoise = PerlinNoise(persistence, frequency, amplitude, octaves, randomseed);

    m_pathJson = saveFolder;

    saveTerrain(randomseed);
}

void ChunkManager::updateAsyncChunker(glm::vec3 cameraPosition, glm::vec3 cameraView)
{
    // Transform camera position into relative chunk position
    glm::vec3 chunkCameraPosition;
    for (int i = 0; i < 3; ++i)
    {
        chunkCameraPosition[i] = (int)cameraPosition[i] / Chunk::CHUNK_SIZE;
    }

    int chunkAreaLimit = 1;
    int unloadLimit = chunkAreaLimit + 1;

    for (int i = -unloadLimit; i <= unloadLimit; ++i)
    {
        for (int j = -unloadLimit; j <= unloadLimit; ++j)
        {
            for (int k = -unloadLimit; k <= unloadLimit; ++k)
            {
                glm::vec3 position(chunkCameraPosition[0] + i, chunkCameraPosition[1] + j, chunkCameraPosition[2] + k);

                if(     i >= -chunkAreaLimit && i <= chunkAreaLimit
                    &&  j >= -chunkAreaLimit && j <= chunkAreaLimit
                    &&  k >= -chunkAreaLimit && k <= chunkAreaLimit)
                {
                    
                    if(!chunkExist(position)){
                        m_vpChunkLoadList.push_back(new Chunk(position));
                    }
                }
                else
                {
                    if(chunkExist(position))
                        m_vpChunkUnloadList.push_back(getChunk(position));
                }
            }
        }
    }
}

bool ChunkManager::chunkExist(const glm::vec3 &position)
{
    ChunkList::iterator iterator;   

    for(iterator = m_vpGlobalChunkList.begin(); iterator != m_vpGlobalChunkList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);
        if(pChunk->getPosition() == position)
            return true;
    }

    return false;
}

void ChunkManager::updateLoadList()
{
    ChunkList::iterator iterator;   

    for(iterator = m_vpChunkLoadList.begin(); iterator != m_vpChunkLoadList.end(); ++iterator)
    {
        Chunk* pChunk = (*iterator);

        if(pChunk->isLoaded() == false)
        {                
                glm::vec3 p = pChunk->getPosition();
                std::string filePath = m_pathJson + "chunk_" + std::to_string((int)p[0]) + "_" + std::to_string((int)p[1]) + "_" + std::to_string((int)p[2]) + ".json";

                if(jsonChunkExist(filePath))
                {
                    Json::Value data = loadAndParseJsonFile(filePath);
                    pChunk->load(data);
                }
                m_vpGlobalChunkList.push_back(pChunk);
                m_vpChunkSetupList.push_back(pChunk);

                m_forceVisibilityUpdate = true;
        }
    }

    // Clear the load list (every frame)
    m_vpChunkLoadList.clear();
}

void ChunkManager::update(/*float dt, */glm::vec3 cameraPosition, glm::vec3 cameraView)
{
    updateAsyncChunker(cameraPosition, cameraView);

    updateLoadList();

    updateSetupList();

    updateRebuildList();

    updateFlagsList();

    updateUnloadList();

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
            pChunk->setup(&m_PerlinNoise);
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
            // Also add our neighbours since they might now be surrounded too (If we have neighbours)
            Chunk* pChunkXMinus = getChunk(pChunk->getX()-1, pChunk->getY(), pChunk->getZ());
            Chunk* pChunkXPlus = getChunk(pChunk->getX()+1, pChunk->getY(), pChunk->getZ());
            Chunk* pChunkYMinus = getChunk(pChunk->getX(), pChunk->getY()-1, pChunk->getZ());
            Chunk* pChunkYPlus = getChunk(pChunk->getX(), pChunk->getY()+1, pChunk->getZ());
            Chunk* pChunkZMinus = getChunk(pChunk->getX(), pChunk->getY(), pChunk->getZ()-1);
            Chunk* pChunkZPlus = getChunk(pChunk->getX(), pChunk->getY(), pChunk->getZ()+1);

            pChunk->buildMesh( pChunkXMinus, pChunkXPlus, pChunkYMinus, pChunkYPlus, pChunkZMinus, pChunkZPlus );

            m_vpChunkUpdateFlagsList.push_back(pChunk);

            

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
            pChunk->unload("bin/assets/saves/");
            delete pChunk;
            m_forceVisibilityUpdate = true;
        }
    }
    // Clear the unload list (every frame)
    m_vpChunkUnloadList.clear();
}

int ChunkManager::loadTerrain(const std::string & saveFolder)
{
    std::ifstream file;
    std::string filePath = saveFolder + "terrain.json";
    file.open(filePath);
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
        else
        {
            std::cout << "Fichier chargé" << std::endl;
            return root["perlin_seed"].asInt();
        }

    }
    else
    {
        std::cerr << "Unable to open file ( load of " + filePath + " )" << std::endl;
        exit(1);
    }
}

void ChunkManager::saveTerrain(const unsigned int & perlin_seed)
{
    std::ofstream file;
    std::string filePath = m_pathJson + "terrain.json";
    file.open(filePath);

    if (file.is_open())
    {
        Json::FastWriter l_writer;

        Json::Value l_val;
        l_val["perlin_seed"] = perlin_seed;
        file << l_writer.write(l_val);

        file.close();
    }
    else
    {
        std::cerr << "Unable to open file ( save in " + filePath + " )" << std::endl;
        exit(1);
    }
}

void ChunkManager::unloadWorld()
{
    m_vpChunkUnloadList = m_vpGlobalChunkList;
    updateUnloadList();
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

        pChunk->render(program, viewMatrix, 1);
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

Chunk* ChunkManager::getChunk(const glm::vec3 &pos)
{
    for (unsigned int i = 0; i < m_vpGlobalChunkList.size(); ++i)
    {
        if (m_vpGlobalChunkList.at(i)->getPosition() == pos)
            return m_vpGlobalChunkList.at(i);
    }

    return NULL;
}

void ChunkManager::addChunkToRebuildList(Chunk * chunk)
{
    m_vpChunkRebuildList.push_back(chunk);
}