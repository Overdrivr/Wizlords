// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/Functions.hpp>
#include <Nazara/VoxelEngine/Debug.hpp>

bool IsSolid(nzVoxelBlockType block)
{
    switch(block)
    {
        case nzVoxelBlockType_nonvalid :
        case nzVoxelBlockType_empty :
            return false;
        break;

        default:
            return true;
        break;
    }
}
