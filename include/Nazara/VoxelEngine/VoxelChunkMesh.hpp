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
#include <Nazara/VoxelEngine/VoxelTerrain.hpp>
#include <Nazara/VoxelEngine/VoxelEngine.hpp>

class NAZARA_API NzVoxelChunkMesh
{
    friend class NzVoxelEngine;
	public:
		NzVoxelChunkMesh();
		~NzVoxelChunkMesh();

        NzVector3i GetLocation() const;
		void GenerateMesh(NzVoxelTerrain& terrain);
		unsigned int GetFaceCount() const;
		unsigned int GetVertexCount() const;

        void SetLocation(const NzVector3i& location);
	private:
        void GenerateCube(const NzVoxelArray& voxelArray, unsigned int x, unsigned int y, unsigned int z);

		NzVertexBuffer m_vertexBuffer;
		std::vector<float> m_vertexData;

		unsigned int m_vertexCount;
		unsigned int m_faceCount;

		NzVector3i m_location;
};

#endif // NAZARA_VOXELCHUNKMESH_HPP
