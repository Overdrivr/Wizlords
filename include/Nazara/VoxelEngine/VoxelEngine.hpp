// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_VOXELENGINE_HPP
#define NAZARA_VOXELENGINE_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Core/Initializer.hpp>
#include <Nazara/Math/Vector3.hpp>
#include <Nazara/VoxelEngine/Enums.hpp>

class NzVoxelChunkMesh;

class NAZARA_API NzVoxelEngine
{
	public:
		NzVoxelEngine() = delete;
		~NzVoxelEngine() = delete;

        static void DrawChunk(const NzVoxelChunkMesh& chunk);

        static std::array<float,24> GetFaceData(nzVoxelFaceOrientation face, NzVector3f offset);

		static bool Initialize();
		static bool IsInitialized();

		static void Uninitialize();

	private:
		static unsigned int s_moduleReferenceCounter;
};

#endif // NAZARA_VOXELENGINE_HPP
