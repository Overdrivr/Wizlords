// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/VoxelEngine.hpp>
#include <Nazara/Core/Core.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/Log.hpp>
#include <Nazara/VoxelEngine/VoxelChunkMesh.hpp>
#include <Nazara/VoxelEngine/Config.hpp>
#include <Nazara/Graphics/Graphics.hpp>
#include <Nazara/Renderer/Renderer.hpp>
#include <Nazara/VoxelEngine/Debug.hpp>

namespace
{
  static NzVertexBuffer m_vertexBuffer;
  static std::array<float,32> m_topFace;
}

void NzVoxelEngine::DrawChunk(const NzVoxelChunkMesh& chunk)
{
    NzRenderer::SetVertexBuffer(&(chunk.m_vertexBuffer));
    NzRenderer::SetIndexBuffer(&(chunk.m_indexBuffer));
    NzRenderer::DrawIndexedPrimitives(nzPrimitiveMode_TriangleList,0,chunk.m_indexCount);
}

std::array<float,32> NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation face, NzVector3f offset, unsigned int textureIndex)
{
    std::array<float,32> data;

    switch(face)
    {
        case nzVoxelFaceOrientation_top :
            data = m_topFace;
        break;
    }

	data[0] += offset.x;
	data[1] += offset.y;
	data[2] += offset.z;

	data[8] += offset.x;
	data[9] += offset.y;
	data[10] += offset.z;

	data[16] += offset.x;
	data[17] += offset.y;
	data[18] += offset.z;

	data[24] += offset.x;
	data[25] += offset.y;
	data[26] += offset.z;
}

bool NzVoxelEngine::Initialize()
{
	if (s_moduleReferenceCounter++ != 0)
		return true; // Déjà initialisé

	// Initialisation des dépendances
	if (!NzGraphics::Initialize())
	{
		NazaraError("Failed to initialize core module");
		return false;
	}

	// Initialisation du module
	//Vertex
	m_topFace[0] = 0.f;
	m_topFace[1] = 1.f;
	m_topFace[2] = 0.f;
	//Normal
	m_topFace[3] = 0.f;
	m_topFace[4] = 1.f;
	m_topFace[5] = 0.f;
	//UV
	m_topFace[6] = 0.f;
	m_topFace[7] = 0.f;
	//Vertex
	m_topFace[8] = 1.f;
	m_topFace[9] = 1.f;
	m_topFace[10] = 0.f;
	//Normal
	m_topFace[11] = 0.f;
	m_topFace[12] = 1.f;
	m_topFace[13] = 0.f;
	//UV
	m_topFace[14] = 1.f;
	m_topFace[15] = 0.f;
	//Vertex
	m_topFace[16] = 1.f;
	m_topFace[17] = 1.f;
	m_topFace[18] = 1.f;
	//Normal
	m_topFace[19] = 0.f;
	m_topFace[20] = 1.f;
	m_topFace[21] = 0.f;
	//UV
	m_topFace[22] = 1.f;
	m_topFace[23] = 1.f;
	//Vertex
	m_topFace[24] = 0.f;
	m_topFace[25] = 1.f;
	m_topFace[26] = 1.f;
	//Normal
	m_topFace[27] = 0.f;
	m_topFace[28] = 1.f;
	m_topFace[29] = 0.f;
	//UV
	m_topFace[30] = 0.f;
	m_topFace[31] = 1.f;

	NazaraNotice("Initialized: VoxelEngine module");

	return true;
}

bool NzVoxelEngine::IsInitialized()
{
	return s_moduleReferenceCounter != 0;
}

void NzVoxelEngine::Uninitialize()
{
	if (s_moduleReferenceCounter != 1)
	{
		// Le module est soit encore utilisé, soit pas initialisé
		if (s_moduleReferenceCounter > 1)
			s_moduleReferenceCounter--;

		return;
	}

	// Libération du module
	s_moduleReferenceCounter = 0;
    m_vertexBuffer.Reset();
	NazaraNotice("Uninitialized: VoxelEngine module");

	// Libération des dépendances
	NzGraphics::Uninitialize();
}

unsigned int NzVoxelEngine::s_moduleReferenceCounter = 0;
