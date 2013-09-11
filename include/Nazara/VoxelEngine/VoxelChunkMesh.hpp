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

		void GenerateMesh(const NzVoxelArray& voxelArray);

	private:
		NzIndexBuffer m_indexBuffer;
		unsigned int m_indexCount;
};

#endif // NAZARA_VOXELCHUNKMESH_HPP
