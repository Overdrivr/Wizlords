// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_VOXELENGINE_HPP
#define NAZARA_VOXELENGINE_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Core/Initializer.hpp>

class NzVoxelChunkMesh;

class NAZARA_API NzVoxelEngine
{
	public:
		NzVoxelEngine() = delete;
		~NzVoxelEngine() = delete;

        static void DrawChunk(const NzVoxelChunkMesh& chunk);

        static const std::array<float,24>& GetFaceData(nzVoxelFaceOrientation face);

		static bool Initialize();
		static bool IsInitialized();

		static void Uninitialize();

	private:
		static unsigned int s_moduleReferenceCounter;
};

#endif // NAZARA_VOXELENGINE_HPP
