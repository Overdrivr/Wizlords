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
  NzVertexBuffer m_vertexBuffer;
}

void NzVoxelEngine::DrawChunk(const NzVoxelChunkMesh& chunk)
{
    NzRenderer::SetVertexBuffer(&m_vertexBuffer);
    NzRenderer::SetIndexBuffer(&(chunk.m_indexBuffer));
    NzRenderer::DrawIndexedPrimitives(nzPrimitiveMode_TriangleList,0,chunk.m_indexCount);
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
	// Construction du vertex buffer
	std::vector<float> static_vertices;
	unsigned int totalSize = (NAZARA_VOXELENGINE_CHUNKSIZE_X + 1) * (NAZARA_VOXELENGINE_CHUNKSIZE_Y + 1) * (NAZARA_VOXELENGINE_CHUNKSIZE_Z + 1);
    static_vertices.reserve(totalSize * 3);

    for(unsigned int j(0) ; j < NAZARA_VOXELENGINE_CHUNKSIZE_Y ; ++j)
        for(unsigned int k(0) ; k < NAZARA_VOXELENGINE_CHUNKSIZE_Z ; ++k)
            for(unsigned int i(0) ; i < NAZARA_VOXELENGINE_CHUNKSIZE_X ; ++i)
            {
                static_vertices.push_back(static_cast<float>(i));
                static_vertices.push_back(static_cast<float>(j));
                static_vertices.push_back(static_cast<float>(k));
            }

    try
    {
        m_vertexBuffer.Reset(NzVertexDeclaration::Get(nzVertexLayout_XYZ), totalSize, nzBufferStorage_Hardware, nzBufferUsage_Dynamic);
    }
    catch (const std::exception& e)
    {
        NazaraError("Failed to reset buffer: " + NzString(e.what()));

        Uninitialize();
        return false;
    }

    if (!m_vertexBuffer.Fill(static_vertices.data(), 0, totalSize))
	{
		NazaraError("Failed to initialize VoxelEngine : Failed to fill vertex buffer");
		m_vertexBuffer.Reset();
		return false;
	}

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
