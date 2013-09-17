// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/VoxelChunkMesh.hpp>
#include <Nazara/VoxelEngine/Config.hpp>
#include <Nazara/VoxelEngine/Functions.hpp>
#include <Nazara/VoxelEngine/Debug.hpp>

NzVoxelChunkMesh::NzVoxelChunkMesh()
{
    m_vertexCount = 0;
    m_faceCount = 0;
    m_vertexBuffer.Reset(NzVertexDeclaration::Get(nzVertexLayout_XYZ_Normal_UV),NAZARA_VOXELENGINE_MAX_VERTEX_AMOUNT, nzBufferStorage_Hardware);
}

NzVoxelChunkMesh::~NzVoxelChunkMesh()
{

}

NzVector3i NzVoxelChunkMesh::GetLocation() const
{
    return m_location;
}

void NzVoxelChunkMesh::GenerateMesh(NzVoxelTerrain& terrain)
{
    m_faceCount = 0;
    m_vertexCount = 0;

    NzVoxelArray* voxelArray = terrain.GetVoxelArray(m_location);

    if(voxelArray == nullptr)
    {
        #if NAZARA_VOXELENGINE_SAFE
        NazaraWarning("Could not generate mesh, chunk not found");
        #endif
        return;
    }

    for(unsigned int x(0) ; x < NAZARA_VOXELENGINE_CHUNKSIZE_X ; ++x)
        for(unsigned int y(0) ; y < NAZARA_VOXELENGINE_CHUNKSIZE_Y ; ++y)
            for(unsigned int z(0) ; z < NAZARA_VOXELENGINE_CHUNKSIZE_Z ; ++z)
            {
                GenerateCube(*voxelArray,x,y,z);
            }
}

unsigned int NzVoxelChunkMesh::GetFaceCount() const
{
    return m_faceCount;
}

unsigned int NzVoxelChunkMesh::GetVertexCount() const
{
    return m_vertexCount;
}

void NzVoxelChunkMesh::GenerateCube(const NzVoxelArray& voxelArray, unsigned int x, unsigned int y, unsigned int z)
{
    #if NAZARA_VOXELENGINE_SAFE
    if(x >= NAZARA_VOXELENGINE_CHUNKSIZE_X ||
       y >= NAZARA_VOXELENGINE_CHUNKSIZE_Y ||
       z >= NAZARA_VOXELENGINE_CHUNKSIZE_Z)
       {
           NazaraError("Block location outside boundaries");
           return;
       }

    #endif

    if(!IsSolid(voxelArray.GetBlockType(NzVector3ui(x,y,z))))
        return;

    int X = static_cast<int>(x);
    int Y = static_cast<int>(y);
    int Z = static_cast<int>(z);

    //TOP
    if(Y + 1 < NAZARA_VOXELENGINE_CHUNKSIZE_Y)
        if(!IsSolid(voxelArray.GetBlockType(NzVector3ui(X,Y + 1,Z))))
        {
            NzVector3f offset(static_cast<float>(X),static_cast<float>(Y),static_cast<float>(Z));
            m_vertexBuffer.Fill(NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation_top,offset,0).data(),m_faceCount * 4,4);
            ++m_faceCount;
            m_vertexCount += 4;
        }
    else
        {
            //NEIGHBOR TOP CHUNK
        }
}

void NzVoxelChunkMesh::SetLocation(const NzVector3i& location)
{
    m_location = location;
}
