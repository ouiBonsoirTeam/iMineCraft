#include <glimac/Geometry.hpp>
#include <glimac/tiny_obj_loader.h>
#include <iostream>
#include <algorithm>

namespace glimac {

void Geometry::generateNormals(unsigned int meshIndex) {
    auto indexOffset = m_MeshBuffer[meshIndex].m_nIndexOffset;
    for (auto j = 0u; j < m_MeshBuffer[meshIndex].m_nIndexCount; j += 3) {
        auto i1 = m_IndexBuffer[indexOffset + j];
        auto i2 = m_IndexBuffer[indexOffset + j + 1];
        auto i3 = m_IndexBuffer[indexOffset + j + 2];

        auto n = glm::cross(glm::normalize(m_VertexBuffer[i2].m_Position - m_VertexBuffer[i1].m_Position),
                            glm::normalize(m_VertexBuffer[i3].m_Position - m_VertexBuffer[i1].m_Position));

        m_VertexBuffer[i1].m_Normal = n;
        m_VertexBuffer[i2].m_Normal = n;
        m_VertexBuffer[i3].m_Normal = n;
    }
}

bool Geometry::loadOBJ(const std::string& filepath, const std::string& mtlBasePath, bool loadTextures) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::cout << "" << std::endl;
    std::clog << "Load OBJ " << filepath << std::endl;
    std::string objErr = tinyobj::LoadObj(shapes, materials, filepath.c_str(), mtlBasePath.c_str());

    std::clog << "done." << std::endl;

    if (!objErr.empty()) {
        std::cerr << objErr << std::endl;
        return false;
    }

    std::clog << "Load materials" << std::endl;
    m_Materials.reserve(m_Materials.size() + materials.size());
    for(auto& material: materials) {
        m_Materials.emplace_back();
        auto& m = m_Materials.back();

        std::cout << "Material name : " << material.name.c_str() << std::endl;

        m.m_Ka = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
        m.m_Kd = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
        m.m_Ks = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
        m.m_Tr = glm::vec3(material.transmittance[0], material.transmittance[1], material.transmittance[2]);
        m.m_Le = glm::vec3(material.emission[0], material.emission[1], material.emission[2]);
        m.m_Shininess = material.shininess;
        m.m_RefractionIndex = material.ior;
        m.m_Dissolve = material.dissolve;

        std::cout << "m_Ka bien loadé : " << m.m_Ka << std::endl;
        std::cout << "m_Kd bien loadé : " << m.m_Kd << std::endl;
        std::cout << "m_Ks bien loadé : " << m.m_Ks << std::endl;

        if(loadTextures) {
            if(!material.ambient_texname.empty()) {
                //std::replace(material.ambient_texname.begin(), material.ambient_texname.end(), '\\', '/');
                FilePath texturePath = mtlBasePath + material.ambient_texname;
                std::clog << "load " << texturePath << std::endl;
                m.m_pKaMap = ImageManager::loadImage(texturePath);
                std::cout << "load map_Ka ---> OK" << std::endl;
            }

            if(!material.diffuse_texname.empty()) {
                //std::replace(material.diffuse_texname.begin(), material.diffuse_texname.end(), '\\', '/');
                FilePath texturePath = mtlBasePath + material.diffuse_texname;
                std::clog << "load " << texturePath << std::endl;
                m.m_pKdMap = ImageManager::loadImage(texturePath);
                std::cout << "load map_Kd ---> OK" << std::endl;
            }

            if(!material.specular_texname.empty()) {
                //std::replace(material.specular_texname.begin(), material.specular_texname.end(), '\\', '/');
                FilePath texturePath = mtlBasePath + material.specular_texname;
                std::clog << "load " << texturePath << std::endl;
                m.m_pKsMap = ImageManager::loadImage(texturePath);
                std::cout << "load map_Kd ---> OK" << std::endl;
            }

            if(!material.normal_texname.empty()) {
                //std::replace(material.normal_texname.begin(), material.normal_texname.end(), '\\', '/');
                FilePath texturePath = mtlBasePath + material.normal_texname;
                std::clog << "load " << texturePath << std::endl;
                m.m_pNormalMap = ImageManager::loadImage(texturePath);
                std::cout << "load map_N ---> OK" << std::endl;
            }
        }
        std::cout << "" << std::endl;
    }
    std::clog << "done." << std::endl;

    auto globalVertexOffset = m_VertexBuffer.size();
    auto globalIndexOffset = m_IndexBuffer.size();

    auto nbVertex = 0u;
    auto nbIndex = 0u;
    for (const auto& shape: shapes) {
        nbVertex += shape.mesh.positions.size();
        nbIndex += shape.mesh.indices.size();
    }

    std::clog << "Number of meshes: " << shapes.size() << std::endl;
    std::clog << "Number of vertices: " << nbVertex << std::endl;
    std::clog << "Number of triangles: " << (nbIndex) / 3 << std::endl;

    m_BBox = BBox3f(glm::vec3(shapes[0].mesh.positions[0], shapes[0].mesh.positions[1], shapes[0].mesh.positions[2]));

    m_VertexBuffer.resize(m_VertexBuffer.size() + nbVertex);
    m_IndexBuffer.resize(m_IndexBuffer.size() + nbIndex);

    auto pVertex = (Vertex*) m_VertexBuffer.data() + globalVertexOffset;
    auto pIndex = (unsigned int*) m_IndexBuffer.data() + globalIndexOffset;

    m_MeshBuffer.reserve(m_MeshBuffer.size() + shapes.size());

    auto vertexOffset = globalVertexOffset;
    auto indexOffset = globalIndexOffset;
    for (size_t i = 0; i < shapes.size(); i++) {
        auto pVertexTmp = pVertex;
        for (auto j = 0u; j < shapes[i].mesh.positions.size(); j += 3) {
            pVertexTmp->m_Position.x = shapes[i].mesh.positions[j];
            pVertexTmp->m_Position.y = shapes[i].mesh.positions[j + 1];
            pVertexTmp->m_Position.z = shapes[i].mesh.positions[j + 2];

            m_BBox.grow(pVertexTmp->m_Position);

            ++pVertexTmp;
        }
        pVertexTmp = pVertex;
        if(shapes[i].mesh.normals.size()) {
            for (auto j = 0u; j < shapes[i].mesh.normals.size(); j += 3) {
                pVertexTmp->m_Normal.x = shapes[i].mesh.normals[j];
                pVertexTmp->m_Normal.y = shapes[i].mesh.normals[j + 1];
                pVertexTmp->m_Normal.z = shapes[i].mesh.normals[j + 2];
                ++pVertexTmp;
            }
        } else {
            // should create normals from triangles...
        }
        pVertexTmp = pVertex;
        if(shapes[i].mesh.texcoords.size()) {
            for (auto j = 0u; j < shapes[i].mesh.texcoords.size(); j += 2) {
                pVertexTmp->m_TexCoords.x = shapes[i].mesh.texcoords[j];
                pVertexTmp->m_TexCoords.y = shapes[i].mesh.texcoords[j + 1];
                ++pVertexTmp;
            }
        } else {
            for (auto j = 0u; j < shapes[i].mesh.texcoords.size(); j += 2) {
                pVertexTmp->m_TexCoords.x =  0.f;
                pVertexTmp->m_TexCoords.x =  0.f;
                ++pVertexTmp;
            }
        }

        for (auto j = 0u; j < shapes[i].mesh.indices.size(); ++j) {
            pIndex[j] = vertexOffset + shapes[i].mesh.indices[j];
        }

        int materialIndex = -1;
        if(!shapes[i].mesh.material_ids.empty()) {
            materialIndex = shapes[i].mesh.material_ids[0];
        }

        m_MeshBuffer.emplace_back(shapes[i].name, indexOffset, shapes[i].mesh.indices.size(), materialIndex);

        if(shapes[i].mesh.normals.size() == 0u) {
            generateNormals(m_MeshBuffer.size() - 1);
        }

        pVertex += shapes[i].mesh.positions.size();
        vertexOffset += shapes[i].mesh.positions.size();
        pIndex += shapes[i].mesh.indices.size();
        indexOffset += shapes[i].mesh.indices.size();
    }

    return true;
}

void Geometry::init(GeometryProgram &geoProgram, Geometry &obj, const std::string& filepath, bool loadTextures, const std::string& texture)
{
    //load obj
    if (!obj.loadOBJ("bin/assets/obj/"+filepath, "bin/assets/obj/mtl/", loadTextures))
        std::cerr << "Impossible de charger l'objet" << std::endl;

    //load texture
    std::unique_ptr<Image> texturePointer;
    texturePointer = loadImage("bin/assets/textures/"+texture);
    if(texturePointer == NULL)
    {
        std::cerr << "Error while charging texture." << std::endl;
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D,  m_texture);
    glTexImage2D(GL_TEXTURE_2D,  0,  GL_RGBA,  texturePointer->getWidth(),  
                    texturePointer->getHeight(),  0,  GL_RGBA,  GL_FLOAT,  texturePointer->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,  0);

    glGenBuffers(1, &m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, obj.getVertexCount() * sizeof(glm::vec3), obj.getVertexBuffer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenBuffers(1, &m_ibo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getIndexCount() * sizeof(uint32_t), obj.getIndexBuffer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    const static GLuint VERTEX_ATTR_POSITION = 0;
    const static GLuint VERTEX_ATTR_NORMAL = 1;
    const static GLuint VERTEX_ATTR_TEXTCOORD = 2;

    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);


        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glEnableVertexAttribArray(VERTEX_ATTR_TEXTCOORD);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Vertex), (const GLvoid*)(0* sizeof(GLfloat)));
            glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Vertex), (const GLvoid*)(3 * sizeof(GLfloat)));
            glVertexAttribPointer(VERTEX_ATTR_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Vertex), (const GLvoid*)(6 * sizeof(GLfloat)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Geometry::draw(GeometryProgram &geoProgram, 
                    Geometry &obj,
                    const glm::mat4 &viewMatrix, 
                    const glm::vec3 &trans, 
                    const glm::vec3 &scal, 
                    const float &angleR, 
                    const glm::vec3 &rot)
{
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), trans);
    modelMatrix = glm::rotate(modelMatrix, angleR, rot);
    modelMatrix = glm::scale(modelMatrix, scal);


    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

    // A sortir de la classe : Identique dans tout le programme
    glm::mat4 projMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

    glm::mat4 modelViewProjMatrix = projMatrix * modelViewMatrix;

    // Normale
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

    glUniformMatrix4fv(geoProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(geoProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjMatrix));
    glUniformMatrix4fv(geoProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));


    glBindVertexArray(m_vao);

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUniform1i(geoProgram.uTexture, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);


    glDrawElements(GL_TRIANGLES, obj.getIndexCount(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
}

void Geometry::drawCrowbar(GeometryProgram &geoProgram, Geometry &obj, const FreeFlyCamera &ffCam, float &breakCube)
{
    //render pour l'obj
    glm::mat4 modelMatrix = glm::mat4(1.0);

    glm::vec3 lookAtVector = glm::vec3(0,0,1);

    glm::vec3 posCam = ffCam.getPosition();
    glm::vec3 posObj = ffCam.getPosition() + glm::vec3(ffCam.getFrontVector().x * 0.2,ffCam.getFrontVector().y * 0.2,ffCam.getFrontVector().z * 0.2 );

    glm::vec3 objToCamProj = posCam - posObj;
    objToCamProj[1] = 0;

    objToCamProj = glm::normalize(objToCamProj);

    float angleCosine = glm::dot(objToCamProj, lookAtVector);

    float angle = glm::acos(angleCosine);

    glm::vec3 upAux = glm::cross(objToCamProj, lookAtVector);

    //glm::mat4 modelMatrix = glm::mat4(1.0);

    modelMatrix = glm::translate(modelMatrix, posObj);

    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.05, 0.05));


    if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
    {
        if (upAux[1] < 0)
            modelMatrix = glm::rotate(modelMatrix, angle, ffCam.getUpVector());
        else
            modelMatrix = glm::rotate(modelMatrix, angle, -ffCam.getUpVector());
    }


    glm::vec3 objToCam = posCam - posObj;

    objToCam = glm::normalize(objToCam);

    angleCosine = glm::dot(objToCamProj,objToCam);

    angle = glm::acos(angleCosine);

    if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
    {
        if (objToCam[1] < 0)
            modelMatrix = glm::rotate(modelMatrix, angle, -ffCam.getLeftVector());
        else
            modelMatrix = glm::rotate(modelMatrix, angle, ffCam.getLeftVector());
    }

    modelMatrix = glm::rotate(modelMatrix, 45.f, glm::vec3(-1,0,0));
    modelMatrix = glm::rotate(modelMatrix, 45.f, glm::vec3(0,-1,0));
    modelMatrix = glm::rotate(modelMatrix, 45.f, glm::vec3(0,0,-1));
    modelMatrix = glm::translate(modelMatrix, -glm::vec3(0,5,3));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8, 0.8, 0.8));

    modelMatrix = glm::rotate(modelMatrix, 10.f - abs(breakCube), glm::vec3(0,1,1));
    if (breakCube < 10)
        breakCube += 2;

    glm::mat4 modelViewMatrix = ffCam.getViewMatrix() * modelMatrix;

    // A sortir de la classe : Identique dans tout le programme
    glm::mat4 projMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

    glm::mat4 modelViewProjMatrix = projMatrix * modelViewMatrix;

    // Normale
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

    glUniformMatrix4fv(geoProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(geoProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjMatrix));
    glUniformMatrix4fv(geoProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));


    glBindVertexArray(m_vao);

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUniform1i(geoProgram.uTexture, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);


    glDrawElements(GL_TRIANGLES, obj.getIndexCount(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
}

void Geometry::destruct()
{
    // release vertex and index buffer object
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);

    // release texture
    glDeleteTextures(1, &m_texture);
}

}
