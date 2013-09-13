// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_VOXELCHUNKMESH_HPP
#define NAZARA_VOXELCHUNKMESH_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Utility/IndexBuffer.hpp>
#include <Nazara/Utility/VertexBuffer.hpp>
#include <Nazara/VoxelEngine/VoxelArray.hpp>
#include <Nazara/VoxelEngine/VoxelEngine.hpp>

class NAZARA_API NzVoxelChunkMesh
{
    friend class NzVoxelEngine;
	public:
		NzVoxelChunkMesh();
		~NzVoxelChunkMesh();

		void GenerateMesh(const NzVoxelTerrain& terrain, NzVector3ui chunkLocation);

	private:
        void GenerateCube(const NzVoxelArray& voxelArray, unsigned int x, unsigned int y, unsigned int z);

		NzIndexBuffer m_indexBuffer;
		NzVertexBuffer m_vertexBuffer;

		unsigned int m_indexCount;
		unsigned int m_faceCount;
};

#endif // NAZARA_VOXELCHUNKMESH_HPP
