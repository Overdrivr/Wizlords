// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/VoxelTerrain.hpp>
#include <Nazara/VoxelEngine/VoxelChunkMesh.hpp>
#include <Nazara/VoxelEngine/Debug.hpp>

NzVoxelTerrain::NzVoxelTerrain()
{

}

NzVoxelTerrain::~NzVoxelTerrain()
{

}

void NzVoxelTerrain::Draw() const
{

}

const NzBoundingVolumef& NzVoxelTerrain::GetBoundingVolume() const
{

}

nzVoxelBlockType NzVoxelTerrain::GetBlockType(NzVector3i location)
{

}

bool NzVoxelTerrain::GetVoxelArray(NzVector3i location, NzVoxelArray* voxelArray) const
{

}

bool NzVoxelTerrain::IsDrawable() const
{

}

bool NzVoxelTerrain::SetBlockType(NzVector3i location, nzVoxelBlockType newType)
{

}

void NzVoxelTerrain::AddToRenderQueue(NzAbstractRenderQueue* renderQueue) const
{

}

bool NzVoxelTerrain::FrustumCull(const NzFrustumf& frustum)
{

}
