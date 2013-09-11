// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_VOXELARRAY_HPP
#define NAZARA_VOXELARRAY_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/VoxelEngine/Config.hpp>
#include <Nazara/VoxelEngine/Enums.hpp>
#include <Nazara/Math/Vector3.hpp>
#include <array>

class NzAbstract3DNoise;

class NAZARA_API NzVoxelArray
{
    friend class NzVoxelChunkMesh;

	public:
		NzVoxelArray(const NzVector3f& position = NzVector3f(0.f,0.f,0.f));
		~NzVoxelArray();

        nzVoxelBlockType GetBlockType(NzVector3ui location) const;
        NzVector3f GetPosition() const;

        void Init(NzAbstract3DNoise& densitySource);

        bool SetBlockType(const NzVector3ui& location, nzVoxelBlockType newType);
        void SetPosition(const NzVector3f& position);

	private:
		std::array<nzVoxelBlockType, NAZARA_VOXELENGINE_CHUNKSIZE_X * NAZARA_VOXELENGINE_CHUNKSIZE_Y  * NAZARA_VOXELENGINE_CHUNKSIZE_Z> m_blocks;

        NzVector3f m_position;
};

#endif // NAZARA_VOXELARRAY_HPP
