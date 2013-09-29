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
    //Procedural generator
    simp3.SetNewSeed(123436789);
    simp3.ShufflePermutationTable();
    simp3.SetResolution(1/30.f);

    m_generateList.push_back(NzVector3i(0,0,0));
    m_generateList.push_back(NzVector3i(1,0,0));
    m_generateList.push_back(NzVector3i(2,0,0));
    m_generateList.push_back(NzVector3i(3,0,0));
    m_generateList.push_back(NzVector3i(0,1,0));
    m_generateList.push_back(NzVector3i(0,2,0));
    m_generateList.push_back(NzVector3i(0,3,0));
    m_generateList.push_back(NzVector3i(0,4,0));

    m_threadRun = true;
    //Start generation thread
    m_auxiliaryThread = NzThread(&NzVoxelTerrain::AuxiliaryThreadFunction,this);
    m_conditionVariable.Signal();

    //Lights
    m_light = new NzLight(nzLightType_Directional);

    // Shader generation & Material
	m_material.EnableLighting(true);

    m_material.SetDiffuseMap("resources/debug_texture_pack.png");
}

NzVoxelTerrain::~NzVoxelTerrain()
{
    delete m_light;
    m_threadRun = false;
    m_generateList.clear();
    m_conditionVariable.Signal();
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

    for(auto it = m_meshes.begin() ; it != m_meshes.end() ; ++it)
    {
        NzVoxelEngine::DrawChunk(it->second);
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
    std::cout<<"Thread started"<<std::endl;

    while(m_threadRun)
    {
        m_conditionVariable.Wait(&m_mutex);
        while(!m_generateList.empty())
        {
            NzVector3i location = m_generateList.front();
            m_generateList.pop_front();

            m_arrays.emplace(location,NzVoxelArray());
            m_meshes.emplace(location,NzVoxelChunkMesh());

            m_meshes[location].SetLocation(location);
            m_arrays[location].Init(simp3);
            m_meshes[location].GenerateMesh(*this);
        }
    }
    std::cout<<"Thread shut down"<<std::endl;
}
