// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/VoxelChunkMesh.hpp>
#include <Nazara/VoxelEngine/Config.hpp>
#include <Nazara/VoxelEngine/Functions.hpp>
#include <iostream>
#include <Nazara/VoxelEngine/Debug.hpp>

NzVoxelChunkMesh::NzVoxelChunkMesh()
{
    m_vertexCount = 0;
    m_faceCount = 0;
    m_location = NzVector3i(0,0,0);
}

NzVoxelChunkMesh::~NzVoxelChunkMesh()
{
    m_vertexBuffer.Reset();
}

NzVector3i NzVoxelChunkMesh::GetLocation() const
{
    return m_location;
}

void NzVoxelChunkMesh::GenerateMesh(NzVoxelArray& voxelArray)
{
    m_faceCount = 0;
    m_vertexCount = 0;
    m_vertexData.clear();

    for(unsigned int x(0) ; x < NAZARA_VOXELENGINE_CHUNKSIZE_X ; ++x)
        for(unsigned int y(0) ; y < NAZARA_VOXELENGINE_CHUNKSIZE_Y ; ++y)
            for(unsigned int z(0) ; z < NAZARA_VOXELENGINE_CHUNKSIZE_Z ; ++z)
            {
                GenerateCube(voxelArray,x,y,z);
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

    bool drawFace = false;

    //TOP
    if(Y + 1 < NAZARA_VOXELENGINE_CHUNKSIZE_Y)
    {
        if(!IsSolid(voxelArray.GetBlockType(NzVector3ui(X,Y + 1,Z))))
        {
            drawFace = true;
        }
    }
    else
    {
        //NEIGHBOR TOP CHUNK
        drawFace = true;
    }

    if(drawFace)
    {
        NzVector3f offset(static_cast<float>(X),
                          static_cast<float>(Y),
                          static_cast<float>(Z));

        std::array<float,36> data = NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation_top,offset,0);

        m_vertexData.reserve(m_vertexCount * 8 + 36);
        std::copy(data.data(),data.data() + 36,std::back_inserter(m_vertexData));
        ++m_faceCount;
        m_vertexCount += 4;
    }
    drawFace = false;

    //LEFT
    if(X + 1 < NAZARA_VOXELENGINE_CHUNKSIZE_X)
    {
        if(!IsSolid(voxelArray.GetBlockType(NzVector3ui(X + 1,Y,Z))))
        {
            drawFace = true;
        }
    }
    else
    {
        //NEIGHBOR LEFT CHUNK
        drawFace = true;
    }

    if(drawFace)
    {
        NzVector3f offset(static_cast<float>(X),
                          static_cast<float>(Y),
                          static_cast<float>(Z));

        std::array<float,36> data = NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation_left,offset,0);
        m_vertexData.reserve(m_vertexCount * 8 + 36);
        std::copy(data.data(),data.data() + 36,std::back_inserter(m_vertexData));
        ++m_faceCount;
        m_vertexCount += 4;
    }
    drawFace = false;

    //RIGHT
    if(X - 1 >= 0)
    {
        if(!IsSolid(voxelArray.GetBlockType(NzVector3ui(X - 1,Y,Z))))
        {
            drawFace = true;
        }
    }
    else
    {
        //NEIGHBOR RIGHT CHUNK
        drawFace = true;
    }

    if(drawFace)
    {
        NzVector3f offset(static_cast<float>(X),
                          static_cast<float>(Y),
                          static_cast<float>(Z));

        std::array<float,36> data = NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation_right,offset,0);
        m_vertexData.reserve(m_vertexCount * 8 + 36);
        std::copy(data.data(),data.data() + 36,std::back_inserter(m_vertexData));
        ++m_faceCount;
        m_vertexCount += 4;
    }
    drawFace = false;

    //FRONT
    if(Z + 1 < NAZARA_VOXELENGINE_CHUNKSIZE_Z)
    {
        if(!IsSolid(voxelArray.GetBlockType(NzVector3ui(X,Y,Z + 1))))
        {
            drawFace = true;
        }
    }
    else
    {
        //NEIGHBOR FRONT CHUNK
        drawFace = true;
    }

    if(drawFace)
    {
        NzVector3f offset(static_cast<float>(X),
                          static_cast<float>(Y),
                          static_cast<float>(Z));

        std::array<float,36> data = NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation_front,offset,0);
        m_vertexData.reserve(m_vertexCount * 8 + 36);
        std::copy(data.data(),data.data() + 36,std::back_inserter(m_vertexData));
        ++m_faceCount;
        m_vertexCount += 4;
    }
    drawFace = false;

    //BACK
    if(Z - 1 >= 0)
    {
        if(!IsSolid(voxelArray.GetBlockType(NzVector3ui(X,Y,Z - 1))))
        {
            drawFace = true;
        }
    }
    else
    {
        //NEIGHBOR FRONT CHUNK
        drawFace = true;
    }

    if(drawFace)
    {
        NzVector3f offset(static_cast<float>(X),
                          static_cast<float>(Y),
                          static_cast<float>(Z));

        std::array<float,36> data = NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation_back,offset,0);
        m_vertexData.reserve(m_vertexCount * 8 + 36);
        std::copy(data.data(),data.data() + 36,std::back_inserter(m_vertexData));
        ++m_faceCount;
        m_vertexCount += 4;
    }
    drawFace = false;

    //BOTTOM
    if(Y - 1 >= 0)
    {
        if(!IsSolid(voxelArray.GetBlockType(NzVector3ui(X,Y - 1,Z))))
        {
            drawFace = true;
        }
    }
    else
    {
        //NEIGHBOR BOTTOM CHUNK
        drawFace = true;
    }

    if(drawFace)
    {
        NzVector3f offset(static_cast<float>(X),
                          static_cast<float>(Y),
                          static_cast<float>(Z));

        std::array<float,36> data = NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation_bottom,offset,0);
        m_vertexData.reserve(m_vertexCount * 8 + 36);
        std::copy(data.data(),data.data() + 36,std::back_inserter(m_vertexData));
        ++m_faceCount;
        m_vertexCount += 4;
    }
}

bool NzVoxelChunkMesh::IsEmpty() const
{
    return m_vertexCount == 0;
}

void NzVoxelChunkMesh::SetLocation(const NzVector3i& location)
{
    m_location = location;
    m_transformationMatrix.MakeTranslation(NzVector3f(static_cast<float>(location.x * NAZARA_VOXELENGINE_CHUNKSIZE_X),
                                                      static_cast<float>(location.y * NAZARA_VOXELENGINE_CHUNKSIZE_Y),
                                                      static_cast<float>(location.z * NAZARA_VOXELENGINE_CHUNKSIZE_Z)));
}

void NzVoxelChunkMesh::UpdateMesh()
{
    if(m_vertexCount > 0)
    {
        //Réserver un peu en plus et ne pas resetter le buffer à chaque coup ?
        m_vertexBuffer.Reset(NzVertexDeclaration::Get(nzVertexLayout_XYZ_Normal_UVW),m_vertexCount,nzBufferStorage_Hardware);
        m_vertexBuffer.Fill(m_vertexData.data(),0,m_vertexCount);
    }
}
