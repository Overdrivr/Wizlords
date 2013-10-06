// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/VoxelTerrain.hpp>
#include <Nazara/VoxelEngine/VoxelChunkMesh.hpp>
#include <Nazara/Renderer.hpp>
#include <iostream>
#include <Nazara/VoxelEngine/Debug.hpp>

NzVoxelTerrain::NzVoxelTerrain()
{
    //List of chunks to generate
    for(int i(0) ; i < 20 ; ++i)
        for(int j(0) ;  j < 20 ; ++j)
            for(int k(0) ; k < 20 ; ++k)
                m_generateList.push_back(NzVector3i(i,j,k));

    m_threadRun = true;
    //Start generation thread
    m_auxiliaryThread = NzThread(&NzVoxelTerrain::AuxiliaryThreadFunction,this);

    //Lights
    m_light = new NzLight(nzLightType_Directional);

    // Shader generation & Material
	m_material.EnableLighting(true);
	unsigned int width = 64;
	unsigned int height = 64;
	unsigned int texturesAmount = 2;

    m_terrainTextureArray.Create(nzImageType_2D_Array,nzPixelFormat_RGBA8,width,height,texturesAmount);

    m_terrainImageArray.LoadFromFile("resources/debug_texture_pack.png");
    m_terrainTextureArray.Update(m_terrainImageArray,NzRectui(64,64),0);
    m_terrainTextureArray.Update(m_terrainImageArray,NzRectui(64,64),1);

    m_material.SetDiffuseMap(&m_terrainTextureArray);
}

NzVoxelTerrain::~NzVoxelTerrain()
{
    delete m_light;
    //Arrêt du thread
    m_threadRun = false;
    m_generateList.clear();
    m_conditionVariable.Signal();
    //On attend que le thread ait fini avant de le supprimer
    m_auxiliaryThread.Join();
}

void NzVoxelTerrain::Draw() const
{
    NzRenderer::SetShaderProgram(m_material.GetShaderProgram(nzShaderTarget_Model,nzShaderFlags_None));

    m_material.Apply(m_material.GetShaderProgram(nzShaderTarget_Model,nzShaderFlags_None));

    m_material.GetShaderProgram(nzShaderTarget_Model,nzShaderFlags_None)->SendColor(
        m_material.GetShaderProgram(nzShaderTarget_Model,nzShaderFlags_None)->GetUniformLocation(nzShaderUniform_SceneAmbient), GetScene()->GetAmbientColor());

    m_material.GetShaderProgram(nzShaderTarget_Model,nzShaderFlags_None)->SendVector(
        m_material.GetShaderProgram(nzShaderTarget_Model,nzShaderFlags_None)->GetUniformLocation(nzShaderUniform_EyePosition), GetScene()->GetViewer()->GetEyePosition());

    m_light->Enable(m_material.GetShaderProgram(nzShaderTarget_Model,nzShaderFlags_None),1.0);
    //std::cout<<m_meshes.size()<<" : "<<m_arrays.size()<<std::endl;
    for(auto it = m_meshes.begin() ; it != m_meshes.end() ; ++it)
    {
        NzVoxelEngine::DrawChunk(*(it->second));
    }
}

const NzBoundingVolumef& NzVoxelTerrain::GetBoundingVolume() const
{
    return m_aabb;
}

nzVoxelBlockType NzVoxelTerrain::GetBlockType(NzVector3i location)
{
    NazaraError("A IMPLEMENTER");
    return nzVoxelBlockType_nonvalid;
}

nzSceneNodeType NzVoxelTerrain::GetSceneNodeType() const
{
    return nzSceneNodeType_User;
}

NzVoxelArray* NzVoxelTerrain::GetVoxelArray(NzVector3i location)
{
    std::map<NzVector3i,NzVoxelArray>::iterator it = m_arrays.find(location);

    if(it != m_arrays.end())
        return &(it->second);

    return nullptr;
}

bool NzVoxelTerrain::IsDrawable() const
{
    return true;
}

void NzVoxelTerrain::Init()
{
    //Update register
    GetScene()->RegisterForUpdate(this);
    //Tell the thread there are tasks to be done
    m_conditionVariable.Signal();
}

bool NzVoxelTerrain::SetBlockType(NzVector3i location, nzVoxelBlockType newType)
{
    NazaraError("A IMPLEMENTER");
}

void NzVoxelTerrain::Update()
{
    if (m_clock.GetMilliseconds() >= 1000/60)
    {
        m_light->Rotate(NzEulerAnglesf(0.01f * m_clock.GetMilliseconds(),0.f,0.f));
        m_clock.Restart();
    }

    int i(0);

    while(!m_meshesToUpdate.empty())
    {
        std::shared_ptr<NzVoxelChunkMesh> mesh;
        mesh = m_meshesToUpdate.front();
        m_meshesToUpdate.pop_front();
        (*mesh).UpdateMesh();
        m_meshes[mesh->GetLocation()] = mesh;
        ++i;
        if(i == 5)
            break;
    }

}

void NzVoxelTerrain::AddToRenderQueue(NzAbstractRenderQueue* renderQueue) const
{
    renderQueue->AddDrawable(this);
}

bool NzVoxelTerrain::FrustumCull(const NzFrustumf& frustum)
{
    return true;
}

void NzVoxelTerrain::AuxiliaryThreadFunction()
{
    while(m_threadRun)
    {
        //Wait for signal to start work
        m_conditionVariable.Wait(&m_mutex);

        while(!m_generateList.empty())
        {
            NzVector3i location = m_generateList.front();
            m_generateList.pop_front();

            NzVector3f offset(static_cast<float>(location.x * NAZARA_VOXELENGINE_CHUNKSIZE_X),
                              static_cast<float>(location.y * NAZARA_VOXELENGINE_CHUNKSIZE_Y),
                              static_cast<float>(location.z * NAZARA_VOXELENGINE_CHUNKSIZE_Z));

            m_arrays.emplace(location,NzVoxelArray(offset));
            m_arrays[location].Init(m_proceduralGenerator);

            std::shared_ptr<NzVoxelChunkMesh> p;
            p.reset(new NzVoxelChunkMesh());
            (*p).SetLocation(location);
            (*p).GenerateMesh(m_arrays[location]);
            m_meshesToUpdate.push_back(p);
        }
    }
}
