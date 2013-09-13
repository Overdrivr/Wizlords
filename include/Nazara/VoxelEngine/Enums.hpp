// Copyright (C) 2012 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module".
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_ENUMS_VOXELENGINE_HPP
#define NAZARA_ENUMS_VOXELENGINE_HPP

enum nzVoxelBlockType
{
    nzVoxelBlockType_nonvalid = 0,
    nzVoxelBlockType_empty = 1,
    nzVoxelBlockType_dirt = 2,
    nzVoxelBlockType_rock = 3,
    nzVoxelBlockType_wood = 4
};

enum nzVoxelFaceOrientation
{
    nzVoxelFaceOrientation_top,
    nzVoxelFaceOrientation_bottom,
    nzVoxelFaceOrientation_left,
    nzVoxelFaceOrientation_right,
    nzVoxelFaceOrientation_front,
    nzVoxelFaceOrientation_back
};

#endif // NAZARA_ENUMS_VOXELENGINE_HPP
