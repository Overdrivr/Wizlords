// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/VoxelTerrain.hpp>
#include <Nazara/VoxelEngine/VoxelChunkMesh.hpp>
#include <Nazara/VoxelEngine/Debug.hpp>

NzVoxelTerrain::NzVoxelTerrain()
{
    m_arrays.emplace(NzVector3i(0,0,0),NzVoxelArray());

    m_meshes.emplace(NzVector3i(0,0,0),NzVoxelChunkMesh());
    m_meshes[NzVector3i(0,0,0)].SetLocation(NzVector3i(0,0,0));
    m_meshes[NzVector3i(0,0,0)].GenerateMesh(*this);
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

}

nzVoxelBlockType NzVoxelTerrain::GetBlockType(NzVector3i location)
{

}

nzSceneNodeType NzVoxelTerrain::GetSceneNodeType() const
{
    return nzSceneNodeType_User;
}

bool NzVoxelTerrain::GetVoxelArray(NzVector3i location, NzVoxelArray* voxelArray)
{
    std::map<NzVector3i,NzVoxelArray>::iterator it = m_arrays.find(location);

    if(it != m_arrays.end())
    {
        NazaraError("right");
        voxelArray = &(it->second);
        return true;
    }
    else
    {
        NazaraError("wrong");
        voxelArray = nullptr;
        return false;
    }
}

bool NzVoxelTerrain::IsDrawable() const
{
    return true;
}

bool NzVoxelTerrain::SetBlockType(NzVector3i location, nzVoxelBlockType newType)
{
    NazaraError("A IMPLEMENTER ? WHATFOR ?");
}

void NzVoxelTerrain::AddToRenderQueue(NzAbstractRenderQueue* renderQueue) const
{
    renderQueue->AddDrawable(this);
}

bool NzVoxelTerrain::FrustumCull(const NzFrustumf& frustum)
{
    return true;
}
