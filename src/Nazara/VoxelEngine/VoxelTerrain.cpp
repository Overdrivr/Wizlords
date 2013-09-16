// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/VoxelTerrain.hpp>
#include <Nazara/VoxelEngine/VoxelChunkMesh.hpp>
#include <iostream>
#include <Nazara/Noise/Simplex3D.hpp>
#include <Nazara/VoxelEngine/Debug.hpp>

NzVoxelTerrain::NzVoxelTerrain()
{
    m_arrays.emplace(NzVector3i(0,0,0),NzVoxelArray());

    m_meshes.emplace(NzVector3i(0,0,0),NzVoxelChunkMesh());
    m_meshes[NzVector3i(0,0,0)].SetLocation(NzVector3i(0,0,0));

    NzSimplex3D simp3(123436789);
    simp3.SetResolution(1/50.f);
    m_arrays[NzVector3i(0,0,0)].Init(simp3);

    m_meshes[NzVector3i(0,0,0)].GenerateMesh(*this);
    std::cout<<"Face count : "<<m_meshes[NzVector3i(0,0,0)].GetFaceCount()<<std::endl;
    std::cout<<"Vertex count : "<<m_meshes[NzVector3i(0,0,0)].GetVertexCount()<<" / "<<NAZARA_VOXELENGINE_MAX_VERTEX_AMOUNT<<std::endl;
}

NzVoxelTerrain::~NzVoxelTerrain()
{

}

void NzVoxelTerrain::Draw() const
{
    NzVoxelEngine::DrawChunk(m_meshes.at(NzVector3i(0,0,0)));
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

bool NzVoxelTerrain::SetBlockType(NzVector3i location, nzVoxelBlockType newType)
{
    NazaraError("A IMPLEMENTER");
}

void NzVoxelTerrain::AddToRenderQueue(NzAbstractRenderQueue* renderQueue) const
{
    renderQueue->AddDrawable(this);
}

bool NzVoxelTerrain::FrustumCull(const NzFrustumf& frustum)
{
    return true;
}
