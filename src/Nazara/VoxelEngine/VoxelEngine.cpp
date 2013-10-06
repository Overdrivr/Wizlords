// Copyright (C) 2013 Rémi Bèges
// This file is part of the "Nazara Engine - Voxel Engine module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/VoxelEngine/VoxelEngine.hpp>
#include <Nazara/Core/Core.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/Log.hpp>
#include <Nazara/VoxelEngine/VoxelChunkMesh.hpp>
#include <Nazara/VoxelEngine/Config.hpp>
#include <Nazara/Graphics/Graphics.hpp>
#include <Nazara/Renderer/Renderer.hpp>
#include <iostream>
#include <Nazara/VoxelEngine/Debug.hpp>

namespace
{
  static std::array<float,32> m_topFace;
  static std::array<float,32> m_leftFace;
  static std::array<float,32> m_rightFace;
  static std::array<float,32> m_frontFace;
  static std::array<float,32> m_backFace;
  static std::array<float,32> m_bottomFace;
  static NzRenderStates m_renderStates;
  static NzIndexBuffer m_indexBuffer;
  static NzShaderProgram m_shader;
}

void NzVoxelEngine::DrawChunk(const NzVoxelChunkMesh& chunk)
{
    NzRenderer::SetMatrix(nzMatrixType_World, chunk.m_transformationMatrix);
    NzRenderer::SetRenderStates(m_renderStates);
    NzRenderer::SetFaceFilling(nzFaceFilling_Fill);

    NzRenderer::SetVertexBuffer(&(chunk.m_vertexBuffer));
    NzRenderer::SetIndexBuffer(&m_indexBuffer);
    NzRenderer::DrawIndexedPrimitives(nzPrimitiveMode_TriangleList,0,chunk.m_faceCount*6);
}

std::array<float,32> NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation face, NzVector3f offset, unsigned int textureIndex)
{
    std::array<float,32> data;

    switch(face)
    {
        case nzVoxelFaceOrientation_top :
            data = m_topFace;
        break;

        case nzVoxelFaceOrientation_left :
            data = m_leftFace;
        break;

        case nzVoxelFaceOrientation_right :
            data = m_rightFace;
        break;

        case nzVoxelFaceOrientation_front :
            data = m_frontFace;
        break;

        case nzVoxelFaceOrientation_back :
            data = m_backFace;
        break;

        case nzVoxelFaceOrientation_bottom :
        default :
            data = m_bottomFace;
        break;
    }

	data[0] += offset.x;
	data[1] += offset.y;
	data[2] += offset.z;

	data[8] += offset.x;
	data[9] += offset.y;
	data[10] += offset.z;

	data[16] += offset.x;
	data[17] += offset.y;
	data[18] += offset.z;

	data[24] += offset.x;
	data[25] += offset.y;
	data[26] += offset.z;

    return data;
}

bool NzVoxelEngine::Initialize()
{
	if (s_moduleReferenceCounter++ != 0)
		return true; // Déjà initialisé

	// Initialisation des dépendances
	if (!NzGraphics::Initialize())
	{
		NazaraError("Failed to initialize core module");
		return false;
	}

    // Initialisation du module

    m_renderStates.parameters[nzRendererParameter_DepthBuffer] = true;

	// Initialisation des faces
	// UV (commun à toutes)
	float paddle = 0.005f;
	float uv[8] = {paddle,                                              paddle,
                   1.f / NAZARA_VOXELENGINE_TEXTURE_SIZE_X - paddle,    paddle,
                   paddle,                                              1.f / NAZARA_VOXELENGINE_TEXTURE_SIZE_Y - paddle,
                   1.f / NAZARA_VOXELENGINE_TEXTURE_SIZE_X - paddle,    1.f / NAZARA_VOXELENGINE_TEXTURE_SIZE_Y - paddle};


	// ---------TOP------------
	//Vertex 1              //Normal                //UV
	m_topFace[0] = 0.f;     m_topFace[3] = 0.f;     m_topFace[6] = uv[0];
	m_topFace[1] = 1.f;     m_topFace[4] = 1.f;     m_topFace[7] = uv[1];
	m_topFace[2] = 0.f;     m_topFace[5] = 0.f;

	//Vertex 2              //Normal                //UV
	m_topFace[8] = 1.f;     m_topFace[11] = 0.f;    m_topFace[14] = uv[2];
	m_topFace[9] = 1.f;     m_topFace[12] = 1.f;    m_topFace[15] = uv[3];
	m_topFace[10] = 0.f;    m_topFace[13] = 0.f;

	//Vertex 3              //Normal                //UV
	m_topFace[16] = 0.f;    m_topFace[19] = 0.f;    m_topFace[22] = uv[4];
	m_topFace[17] = 1.f;    m_topFace[20] = 1.f;    m_topFace[23] = uv[5];
	m_topFace[18] = 1.f;    m_topFace[21] = 0.f;

	//Vertex 4              //Normal                //UV
	m_topFace[24] = 1.f;    m_topFace[27] = 0.f;    m_topFace[30] = uv[6];
	m_topFace[25] = 1.f;    m_topFace[28] = 1.f;    m_topFace[31] = uv[7];
	m_topFace[26] = 1.f;    m_topFace[29] = 0.f;

	// ------LEFT-------
	//Vertex 1              //Normal                //UV
	m_leftFace[0] = 1.f;    m_leftFace[3] = 1.f;    m_leftFace[6] = uv[0];
	m_leftFace[1] = 0.f;    m_leftFace[4] = 0.f;    m_leftFace[7] = uv[1];
	m_leftFace[2] = 0.f;    m_leftFace[5] = 0.f;

	//Vertex 2              //Normal                //UV
	m_leftFace[8] = 1.f;    m_leftFace[11] = 1.f;   m_leftFace[14] = uv[2];
	m_leftFace[9] = 1.f;    m_leftFace[12] = 0.f;   m_leftFace[15] = uv[3];
	m_leftFace[10] = 0.f;   m_leftFace[13] = 0.f;

	//Vertex 3              //Normal                //UV
	m_leftFace[16] = 1.f;   m_leftFace[19] = 1.f;   m_leftFace[22] = uv[4];
	m_leftFace[17] = 0.f;   m_leftFace[20] = 0.f;   m_leftFace[23] = uv[5];
	m_leftFace[18] = 1.f;   m_leftFace[21] = 0.f;

	//Vertex 4              //Normal                //UV
	m_leftFace[24] = 1.f;   m_leftFace[27] = 1.f;   m_leftFace[30] = uv[6];
	m_leftFace[25] = 1.f;   m_leftFace[28] = 0.f;   m_leftFace[31] = uv[7];
	m_leftFace[26] = 1.f;   m_leftFace[29] = 0.f;

	// ------RIGHT-------
	//Vertex 1              //Normal                //UV
	m_rightFace[0] = 0.f;   m_rightFace[3] = -1.f;   m_rightFace[6] = uv[0];
	m_rightFace[1] = 0.f;   m_rightFace[4] = 0.f;   m_rightFace[7] = uv[1];
	m_rightFace[2] = 0.f;   m_rightFace[5] = 0.f;

	//Vertex 2              //Normal                //UV
	m_rightFace[8] = 0.f;   m_rightFace[11] = -1.f;  m_rightFace[14] = uv[2];
	m_rightFace[9] = 1.f;   m_rightFace[12] = 0.f;  m_rightFace[15] = uv[3];
	m_rightFace[10] = 0.f;  m_rightFace[13] = 0.f;

	//Vertex 3              //Normal                //UV
	m_rightFace[16] = 0.f;  m_rightFace[19] = -1.f;  m_rightFace[22] = uv[4];
	m_rightFace[17] = 0.f;  m_rightFace[20] = 0.f;  m_rightFace[23] = uv[5];
	m_rightFace[18] = 1.f;  m_rightFace[21] = 0.f;

	//Vertex 4              //Normal                //UV
	m_rightFace[24] = 0.f;  m_rightFace[27] = -1.f;  m_rightFace[30] = uv[6];
	m_rightFace[25] = 1.f;  m_rightFace[28] = 0.f;  m_rightFace[31] = uv[7];
	m_rightFace[26] = 1.f;  m_rightFace[29] = 0.f;

    // ------FRONT-------
	//Vertex 1              //Normal                //UV
	m_frontFace[0] = 0.f;    m_frontFace[3] = 0.f;    m_frontFace[6] = uv[0];
	m_frontFace[1] = 0.f;    m_frontFace[4] = 0.f;    m_frontFace[7] = uv[1];
	m_frontFace[2] = 1.f;    m_frontFace[5] = 1.f;

	//Vertex 2              //Normal                //UV
	m_frontFace[8] = 0.f;    m_frontFace[11] = 0.f;   m_frontFace[14] = uv[2];
	m_frontFace[9] = 1.f;    m_frontFace[12] = 0.f;   m_frontFace[15] = uv[3];
	m_frontFace[10] = 1.f;   m_frontFace[13] = 1.f;

	//Vertex 3              //Normal                //UV
	m_frontFace[16] = 1.f;   m_frontFace[19] = 0.f;   m_frontFace[22] = uv[4];
	m_frontFace[17] = 0.f;   m_frontFace[20] = 0.f;   m_frontFace[23] = uv[5];
	m_frontFace[18] = 1.f;   m_frontFace[21] = 1.f;

	//Vertex 4              //Normal                //UV
	m_frontFace[24] = 1.f;   m_frontFace[27] = 0.f;   m_frontFace[30] = uv[6];
	m_frontFace[25] = 1.f;   m_frontFace[28] = 0.f;   m_frontFace[31] = uv[7];
	m_frontFace[26] = 1.f;   m_frontFace[29] = 1.f;

	// ------BACK-------
	//Vertex 1              //Normal                //UV
	m_backFace[0] = 0.f;    m_backFace[3] = 0.f;    m_backFace[6] = uv[0];
	m_backFace[1] = 0.f;    m_backFace[4] = 0.f;    m_backFace[7] = uv[1];
	m_backFace[2] = 0.f;    m_backFace[5] = -1.f;

	//Vertex 2              //Normal                //UV
	m_backFace[8] = 0.f;    m_backFace[11] = 0.f;   m_backFace[14] = uv[2];
	m_backFace[9] = 1.f;    m_backFace[12] = 0.f;   m_backFace[15] = uv[3];
	m_backFace[10] = 0.f;   m_backFace[13] = -1.f;

	//Vertex 3              //Normal                //UV
	m_backFace[16] = 1.f;   m_backFace[19] = 0.f;   m_backFace[22] = uv[4];
	m_backFace[17] = 0.f;   m_backFace[20] = 0.f;   m_backFace[23] = uv[5];
	m_backFace[18] = 0.f;   m_backFace[21] = -1.f;

	//Vertex 4              //Normal                //UV
	m_backFace[24] = 1.f;   m_backFace[27] = 0.f;   m_backFace[30] = uv[6];
	m_backFace[25] = 1.f;   m_backFace[28] = 0.f;   m_backFace[31] = uv[7];
	m_backFace[26] = 0.f;   m_backFace[29] = -1.f;

    // ---------BOTTOM------------
	//Vertex 1                  //Normal                    //UV
	m_bottomFace[0] = 0.f;      m_bottomFace[3] = 0.f;      m_bottomFace[6] = uv[0];
	m_bottomFace[1] = 0.f;      m_bottomFace[4] = -1.f;     m_bottomFace[7] = uv[1];
	m_bottomFace[2] = 0.f;      m_bottomFace[5] = 0.f;

	//Vertex 2                  //Normal                    //UV
	m_bottomFace[8] = 1.f;      m_bottomFace[11] = 0.f;     m_bottomFace[14] = uv[2];
	m_bottomFace[9] = 0.f;      m_bottomFace[12] = -1.f;    m_bottomFace[15] = uv[3];
	m_bottomFace[10] = 0.f;     m_bottomFace[13] = 0.f;

	//Vertex 3                  //Normal                    //UV
	m_bottomFace[16] = 0.f;     m_bottomFace[19] = 0.f;     m_bottomFace[22] = uv[4];
	m_bottomFace[17] = 0.f;     m_bottomFace[20] = -1.f;    m_bottomFace[23] = uv[5];
	m_bottomFace[18] = 1.f;     m_bottomFace[21] = 0.f;

	//Vertex 4                  //Normal                    //UV
	m_bottomFace[24] = 1.f;     m_bottomFace[27] = 0.f;     m_bottomFace[30] = uv[6];
	m_bottomFace[25] = 0.f;     m_bottomFace[28] = -1.f;    m_bottomFace[31] = uv[7];
	m_bottomFace[26] = 1.f;     m_bottomFace[29] = 0.f;


	// Index buffer
	try
    {
        m_indexBuffer.Reset(false,NAZARA_VOXELENGINE_MAX_INDEX_AMOUNT,nzBufferStorage_Hardware);
    }
    catch (const std::exception& e)
    {
        NazaraError("Failed to create buffer: " + NzString(e.what()));
        Uninitialize();
        return false;
    }

	nzUInt16 indexes[NAZARA_VOXELENGINE_MAX_INDEX_AMOUNT] = {0};
	nzUInt16 index[6] = {0,1,2,1,2,3};

	for(unsigned int i(0) ; i < NAZARA_VOXELENGINE_MAX_FACE_AMOUNT ; ++i)
	{
        indexes[i * 6] = index[0] + i*4;
        indexes[i * 6 + 1] = index[1] + i*4;
        indexes[i * 6 + 2] = index[2] + i*4;
        indexes[i * 6 + 3] = index[3] + i*4;
        indexes[i * 6 + 4] = index[4] + i*4;
        indexes[i * 6 + 5] = index[5] + i*4;
	}

	if(!m_indexBuffer.Fill(indexes, 0, NAZARA_VOXELENGINE_MAX_INDEX_AMOUNT))
	{
		NazaraError("Failed to initialize voxel engine module : Failed to fill index buffer");
		Uninitialize();
		return false;
	}

	//Shader
	m_shader.Create(nzShaderLanguage_GLSL);

	if(!m_shader.LoadShaderFromFile(nzShaderType_Vertex, "resources/shader.vert"))
    {
        NazaraError("Failed to initialize voxel renderer module : Failed to load vertex shader");
        m_shader.Destroy();
        return false;
    }

    if(!m_shader.LoadShaderFromFile(nzShaderType_Fragment, "resources/shader.frag"))
    {
        NazaraError("Failed to initialize voxel renderer module : Failed to load fragment shader");
        m_shader.Destroy();
        return false;
    }

    if (!m_shader.Compile())
    {
        NazaraError("Failed to initialize voxel renderer module : Failed to compile shader");
        m_shader.Destroy();
        return false;
    }

	NazaraNotice("Initialized: VoxelEngine module");

	return true;
}

bool NzVoxelEngine::IsInitialized()
{
	return s_moduleReferenceCounter != 0;
}

void NzVoxelEngine::Uninitialize()
{
	if (s_moduleReferenceCounter != 1)
	{
		// Le module est soit encore utilisé, soit pas initialisé
		if (s_moduleReferenceCounter > 1)
			s_moduleReferenceCounter--;

		return;
	}

	// Libération du module
	s_moduleReferenceCounter = 0;
	m_indexBuffer.Reset();
	m_shader.Destroy();

	NazaraNotice("Uninitialized: VoxelEngine module");

	// Libération des dépendances
	NzGraphics::Uninitialize();
}

unsigned int NzVoxelEngine::s_moduleReferenceCounter = 0;
