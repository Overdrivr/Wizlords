// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/VoxelArray.hpp>
#include <Nazara/Noise/Abstract3DNoise.hpp>
#include <iostream>
#include <Nazara/VoxelEngine/Debug.hpp>

NzVoxelArray::NzVoxelArray(const NzVector3f& position) : m_position(position)
{
    for(unsigned int i(0) ; i < m_blocks.size() ; ++i)
    {
        m_blocks[i] = nzVoxelBlockType_dirt;
    }
}

NzVoxelArray::~NzVoxelArray()
{

}

nzVoxelBlockType NzVoxelArray::GetBlockType(NzVector3ui location) const
{
    if(location.x >= 0 &&
       location.y >= 0 &&
       location.z >= 0 &&
       location.x < NAZARA_VOXELENGINE_CHUNKSIZE_X &&
       location.y < NAZARA_VOXELENGINE_CHUNKSIZE_Y &&
       location.z < NAZARA_VOXELENGINE_CHUNKSIZE_Z)
       {
           return m_blocks[location.x +
                           location.y * NAZARA_VOXELENGINE_CHUNKSIZE_X +
                           location.z * NAZARA_VOXELENGINE_CHUNKSIZE_X * NAZARA_VOXELENGINE_CHUNKSIZE_Y];
       }
       else
            return nzVoxelBlockType_nonvalid;
}

NzVector3f NzVoxelArray::GetPosition() const
{
    return m_position;
}

void NzVoxelArray::Init(NzAbstract3DNoise& densitySource)
{
    //Génération du terrain
    for(unsigned int i(0) ; i < NAZARA_VOXELENGINE_CHUNKSIZE_X ; ++i)
        for(unsigned int j(0) ; j < NAZARA_VOXELENGINE_CHUNKSIZE_Y ; ++j)
            for(unsigned int k(0) ; k < NAZARA_VOXELENGINE_CHUNKSIZE_Z ; ++k)
            {
                float density = densitySource.GetBasicValue(m_position.x + static_cast<float>(i),
                                                            m_position.y + static_cast<float>(j),
                                                            m_position.z + static_cast<float>(k));


                nzVoxelBlockType block;
                block = (density > 0.5f) ? nzVoxelBlockType_dirt : nzVoxelBlockType_empty;

                m_blocks[i +
                         j * NAZARA_VOXELENGINE_CHUNKSIZE_X +
                         k * NAZARA_VOXELENGINE_CHUNKSIZE_X * NAZARA_VOXELENGINE_CHUNKSIZE_Y] = block;
            }
}

bool NzVoxelArray::SetBlockType(const NzVector3ui& location, nzVoxelBlockType newType)
{
    if(location.x >= 0 &&
       location.y >= 0 &&
       location.z >= 0 &&
       location.x < static_cast<unsigned int>(NAZARA_VOXELENGINE_CHUNKSIZE_X) &&
       location.y < static_cast<unsigned int>(NAZARA_VOXELENGINE_CHUNKSIZE_Y) &&
       location.z < static_cast<unsigned int>(NAZARA_VOXELENGINE_CHUNKSIZE_Z))
       {
           m_blocks[location.x +
                    location.y * NAZARA_VOXELENGINE_CHUNKSIZE_X +
                    location.z * NAZARA_VOXELENGINE_CHUNKSIZE_X * NAZARA_VOXELENGINE_CHUNKSIZE_Y] = newType;
       }
}

void NzVoxelArray::SetPosition(const NzVector3f& position)
{
    m_position = position;
}
