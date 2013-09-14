/*
	VoxelEngine

	Copyright (C) 2013 Rémi Bèges (remi[dot]beges[at]laposte[net]). All rights reserved.

	This is an authorized fork of the open-source NazaraEngine.
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
const unsigned int NAZARA_VOXELENGINE_CHUNKSIZE_X = 24;
const unsigned int NAZARA_VOXELENGINE_CHUNKSIZE_Y = 256;
const unsigned int NAZARA_VOXELENGINE_CHUNKSIZE_Z = 24;

const unsigned int NAZARA_VOXELENGINE_MAX_CUBE_AMOUNT = NAZARA_VOXELENGINE_CHUNKSIZE_X * NAZARA_VOXELENGINE_CHUNKSIZE_Y * NAZARA_VOXELENGINE_CHUNKSIZE_Z / 3;
const unsigned int NAZARA_VOXELENGINE_MAX_VERTEX_AMOUNT = NAZARA_VOXELENGINE_MAX_CUBE_AMOUNT * 6 * 4;

#endif // NAZARA_CONFIG_VOXELENGINE_HPP
