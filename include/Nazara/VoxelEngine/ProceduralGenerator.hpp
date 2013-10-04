// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_PROCEDURALGENERATOR_HPP
#define NAZARA_PROCEDURALGENERATOR_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Noise/Simplex3D.hpp>
#include <Nazara/VoxelEngine/Enums.hpp>

class NAZARA_API NzProceduralGenerator
{
	public:
		NzProceduralGenerator();
		~NzProceduralGenerator();

        nzVoxelBlockType GetBlock(NzVector3f location);
        nzVoxelBlockType GetBlock(float x, float y, float z);

    protected:

	private:
        NzSimplex3D simp3;
};

#endif // NAZARA_PROCEDURALGENERATOR_HPP
