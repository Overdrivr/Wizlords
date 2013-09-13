// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_VOXELTERRAIN_HPP
#define NAZARA_VOXELTERRAIN_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/VoxelEngine/VoxelArray.hpp>
#include <Nazara/Math/Vector3.hpp>
#include <Nazara/Graphics.hpp>
#include <Nazara/VoxelEngine/VoxelChunkMesh.hpp>
#include <map>

class NAZARA_API NzVoxelTerrain : public NzDrawable, public NzSceneNode, public NzUpdatable
{
	public:
		NzVoxelTerrain();
		~NzVoxelTerrain();

		 virtual void Draw() const;

        virtual const NzBoundingVolumef& GetBoundingVolume() const;
		//virtual nzSceneNodeType GetSceneNodeType() const;
		nzVoxelBlockType GetBlockType(NzVector3i location);
		bool GetVoxelArray(NzVector3i location, NzVoxelArray* voxelArray);

        virtual bool IsDrawable() const;

        bool SetBlockType(NzVector3i location, nzVoxelBlockType newType);

    protected:
        virtual void AddToRenderQueue(NzAbstractRenderQueue* renderQueue) const;
        virtual bool FrustumCull(const NzFrustumf& frustum);

        NzBoundingVolumef m_aabb;

	private:
		std::map<NzVector3i, NzVoxelArray> m_arrays;
		std::map<NzVector3i, NzVoxelChunkMesh> m_meshes;
};

#endif // NAZARA_VOXELTERRAIN_HPP