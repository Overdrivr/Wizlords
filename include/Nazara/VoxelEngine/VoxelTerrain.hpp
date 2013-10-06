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
#include <Nazara/Core/Clock.hpp>
#include <Nazara/Core/Thread.hpp>
#include <Nazara/Core/ConditionVariable.hpp>
#include <Nazara/VoxelEngine/ProceduralGenerator.hpp>
#include <memory>
#include <map>
#include <list>

class NzVoxelChunkMesh;

class NAZARA_API NzVoxelTerrain : public NzDrawable, public NzSceneNode, public NzUpdatable
{
	public:
		NzVoxelTerrain();
		~NzVoxelTerrain();

		virtual void Draw() const;

        virtual const NzBoundingVolumef& GetBoundingVolume() const;
		virtual nzSceneNodeType GetSceneNodeType() const;
		nzVoxelBlockType GetBlockType(NzVector3i location);
		NzVoxelArray* GetVoxelArray(NzVector3i location);

        virtual bool IsDrawable() const;
        void Init();

        bool SetBlockType(NzVector3i location, nzVoxelBlockType newType);

        virtual void Update();

        void AuxiliaryThreadFunction();
    protected:
        virtual void AddToRenderQueue(NzAbstractRenderQueue* renderQueue) const;
        virtual bool FrustumCull(const NzFrustumf& frustum);

        NzBoundingVolumef m_aabb;

	private:
		std::map<NzVector3i, NzVoxelArray> m_arrays;
		std::map<NzVector3i, std::shared_ptr<NzVoxelChunkMesh>> m_meshes;

		//Les maillages générés sont placées dans cette liste, puis récupérés par le thread principal lors de Update
		//Afin que les appels à OpenGl soient faits dans le thread principal (le thread secondaire n'a pas accés au contexte)
		std::list<std::shared_ptr<NzVoxelChunkMesh>> m_meshesToUpdate;

        //Les liste de maillage à générer/détruire par le thread secondaire
		std::list<NzVector3i> m_generateList;
		std::list<NzVector3i> m_deleteList;

		NzThread m_auxiliaryThread;
		NzMutex m_mutex;
		NzConditionVariable m_conditionVariable;
		bool m_threadRun;

		NzProceduralGenerator m_proceduralGenerator;

		NzMaterial m_material;
		NzTexture m_terrainTextureArray;
		NzImage m_terrainImageArray;

        NzLight* m_light;
        NzClock m_clock;
};

#endif // NAZARA_VOXELTERRAIN_HPP
