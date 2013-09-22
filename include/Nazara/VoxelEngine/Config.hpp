/*
	VoxelEngine

	Copyright (C) 2013 Rémi Bèges (remi[dot]beges[at]laposte[net]). All rights reserved.
*/

#pragma once

#ifndef NAZARA_CONFIG_VOXELENGINE_HPP
#define NAZARA_CONFIG_VOXELENGINE_HPP

/// Chaque modification d'un paramètre du module nécessite une recompilation de celui-ci

// Utilise un tracker pour repérer les éventuels leaks (Ralentit l'exécution)
#define NAZARA_VOXELENGINE_MEMORYLEAKTRACKER 0

// Active les tests de sécurité basés sur le code (Conseillé pour le développement)
#define NAZARA_VOXELENGINE_SAFE 1

//Les dimensions d'un chunk
const unsigned int NAZARA_VOXELENGINE_CHUNKSIZE_X = 32;
const unsigned int NAZARA_VOXELENGINE_CHUNKSIZE_Y = 32;
const unsigned int NAZARA_VOXELENGINE_CHUNKSIZE_Z = 32;

const unsigned int NAZARA_VOXELENGINE_MAX_CUBE_AMOUNT = NAZARA_VOXELENGINE_CHUNKSIZE_X * NAZARA_VOXELENGINE_CHUNKSIZE_Y * NAZARA_VOXELENGINE_CHUNKSIZE_Z / 3;
const unsigned int NAZARA_VOXELENGINE_MAX_FACE_AMOUNT = NAZARA_VOXELENGINE_MAX_CUBE_AMOUNT * 6;
const unsigned int NAZARA_VOXELENGINE_MAX_INDEX_AMOUNT = NAZARA_VOXELENGINE_MAX_CUBE_AMOUNT * 6 * 6;
const unsigned int NAZARA_VOXELENGINE_MAX_VERTEX_AMOUNT = NAZARA_VOXELENGINE_MAX_CUBE_AMOUNT * 6 * 4;

const unsigned int NAZARA_VOXELENGINE_TEXTURE_SIZE_X = 2;
const unsigned int NAZARA_VOXELENGINE_TEXTURE_SIZE_Y = 1;


#endif // NAZARA_CONFIG_VOXELENGINE_HPP
