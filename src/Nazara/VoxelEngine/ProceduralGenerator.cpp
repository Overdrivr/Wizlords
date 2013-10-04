// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/ProceduralGenerator.hpp>
#include <Nazara/VoxelEngine/Debug.hpp>

NzProceduralGenerator::NzProceduralGenerator()
{
    //Procedural generator
    simp3.SetNewSeed(123436789);
    simp3.ShufflePermutationTable();
    simp3.SetResolution(1/30.f);
}

NzProceduralGenerator::~NzProceduralGenerator()
{

}

nzVoxelBlockType NzProceduralGenerator::GetBlock(NzVector3f location)
{
    return GetBlock(location.x,location.y,location.z);
}

nzVoxelBlockType NzProceduralGenerator::GetBlock(float x, float y, float z)
{
    float density = simp3.GetBasicValue(x,y,z);

    nzVoxelBlockType block;
    block = (density > 0.5f) ? nzVoxelBlockType_dirt : nzVoxelBlockType_empty;

    return block;
}
