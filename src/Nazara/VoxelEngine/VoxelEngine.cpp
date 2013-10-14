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
  static std::array<float,36> m_topFace;
  static std::array<float,36> m_leftFace;
  static std::array<float,36> m_rightFace;
  static std::array<float,36> m_frontFace;
  static std::array<float,36> m_backFace;
  static std::array<float,36> m_bottomFace;
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

std::array<float,36> NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation face, NzVector3f offset, unsigned int textureIndex)
{
    std::array<float,36> data;

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

	data[9] += offset.x;
	data[10] += offset.y;
	data[11] += offset.z;

	data[18] += offset.x;
	data[19] += offset.y;
	data[20] += offset.z;

	data[27] += offset.x;
	data[28] += offset.y;
	data[29] += offset.z;

	data[8]  += static_cast<float>(textureIndex);
	data[17] += static_cast<float>(textureIndex);
	data[26] += static_cast<float>(textureIndex);
	data[35] += static_cast<float>(textureIndex);

    return data;
}

NzShaderProgram* NzVoxelEngine::GetShader()
{
    return &m_shader;
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
    float uv[8] = {0.f, 0.f,
                   1.f, 0.f,
                   0.f, 1.f,
                   1.f, 1.f};


	// ---------TOP------------
	//Vertex 1              //Normal                //UV
	m_topFace[0] = 0.f;     m_topFace[3] = 0.f;     m_topFace[6] = uv[0];
	m_topFace[1] = 1.f;     m_topFace[4] = 1.f;     m_topFace[7] = uv[1];
	m_topFace[2] = 0.f;     m_topFace[5] = 0.f;     m_topFace[8] = 0.f; //Texture layer

	//Vertex 2              //Normal                //UV
	m_topFace[9] = 1.f;     m_topFace[12] = 0.f;    m_topFace[15] = uv[2];
	m_topFace[10] = 1.f;    m_topFace[13] = 1.f;    m_topFace[16] = uv[3];
	m_topFace[11] = 0.f;    m_topFace[14] = 0.f;    m_topFace[17] = 0.f;

	//Vertex 3              //Normal                //UV
	m_topFace[18] = 0.f;    m_topFace[21] = 0.f;    m_topFace[24] = uv[4];
	m_topFace[19] = 1.f;    m_topFace[22] = 1.f;    m_topFace[25] = uv[5];
	m_topFace[20] = 1.f;    m_topFace[23] = 0.f;    m_topFace[26] = 0.f;

	//Vertex 4              //Normal                //UV
	m_topFace[27] = 1.f;    m_topFace[30] = 0.f;    m_topFace[33] = uv[6];
	m_topFace[28] = 1.f;    m_topFace[31] = 1.f;    m_topFace[34] = uv[7];
	m_topFace[29] = 1.f;    m_topFace[32] = 0.f;    m_topFace[35] = 0.f;

	// ------LEFT-------
	//Vertex 1              //Normal                //UV
	m_leftFace[0] = 1.f;    m_leftFace[3] = 1.f;    m_leftFace[6] = uv[0];
	m_leftFace[1] = 0.f;    m_leftFace[4] = 0.f;    m_leftFace[7] = uv[1];
	m_leftFace[2] = 0.f;    m_leftFace[5] = 0.f;    m_leftFace[8] = 0.f;

	//Vertex 2              //Normal                //UV
	m_leftFace[9] = 1.f;    m_leftFace[12] = 1.f;   m_leftFace[15] = uv[2];
	m_leftFace[10] = 1.f;   m_leftFace[13] = 0.f;   m_leftFace[16] = uv[3];
	m_leftFace[11] = 0.f;   m_leftFace[14] = 0.f;   m_leftFace[17] = 0.f;

	//Vertex 3              //Normal                //UV
	m_leftFace[18] = 1.f;   m_leftFace[21] = 1.f;   m_leftFace[24] = uv[4];
	m_leftFace[19] = 0.f;   m_leftFace[22] = 0.f;   m_leftFace[25] = uv[5];
	m_leftFace[20] = 1.f;   m_leftFace[23] = 0.f;   m_leftFace[26] = 0.f;

	//Vertex 4              //Normal                //UV
	m_leftFace[27] = 1.f;   m_leftFace[30] = 1.f;   m_leftFace[33] = uv[6];
	m_leftFace[28] = 1.f;   m_leftFace[31] = 0.f;   m_leftFace[34] = uv[7];
	m_leftFace[29] = 1.f;   m_leftFace[32] = 0.f;   m_leftFace[35] = 0.f;

	// ------RIGHT-------
	//Vertex 1              //Normal                //UV
	m_rightFace[0] = 0.f;   m_rightFace[3] = -1.f;  m_rightFace[6] = uv[0];
	m_rightFace[1] = 0.f;   m_rightFace[4] = 0.f;   m_rightFace[7] = uv[1];
	m_rightFace[2] = 0.f;   m_rightFace[5] = 0.f;   m_rightFace[8] = 0.f;

	//Vertex 2              //Normal                //UV
	m_rightFace[9] = 0.f;   m_rightFace[12] = -1.f; m_rightFace[15] = uv[2];
	m_rightFace[10] = 1.f;  m_rightFace[13] = 0.f;  m_rightFace[16] = uv[3];
	m_rightFace[11] = 0.f;  m_rightFace[14] = 0.f;  m_rightFace[17] = 0.f;

	//Vertex 3              //Normal                //UV
	m_rightFace[18] = 0.f;  m_rightFace[21] = -1.f; m_rightFace[24] = uv[4];
	m_rightFace[19] = 0.f;  m_rightFace[22] = 0.f;  m_rightFace[25] = uv[5];
	m_rightFace[20] = 1.f;  m_rightFace[23] = 0.f;  m_rightFace[26] = 0.f;

	//Vertex 4              //Normal                //UV
	m_rightFace[27] = 0.f;  m_rightFace[30] = -1.f; m_rightFace[33] = uv[6];
	m_rightFace[28] = 1.f;  m_rightFace[31] = 0.f;  m_rightFace[34] = uv[7];
	m_rightFace[29] = 1.f;  m_rightFace[32] = 0.f;  m_rightFace[35] = 0.f;

    // ------FRONT-------
	//Vertex 1              //Normal                //UV
	m_frontFace[0] = 0.f;   m_frontFace[3] = 0.f;   m_frontFace[6] = uv[0];
	m_frontFace[1] = 0.f;   m_frontFace[4] = 0.f;   m_frontFace[7] = uv[1];
	m_frontFace[2] = 1.f;   m_frontFace[5] = 1.f;   m_frontFace[8] = 0.f;

	//Vertex 2              //Normal                //UV
	m_frontFace[9] = 0.f;   m_frontFace[12] = 0.f;  m_frontFace[15] = uv[2];
	m_frontFace[10] = 1.f;  m_frontFace[13] = 0.f;  m_frontFace[16] = uv[3];
	m_frontFace[11] = 1.f;  m_frontFace[14] = 1.f;  m_frontFace[17] = 0.f;

	//Vertex 3              //Normal                //UV
	m_frontFace[18] = 1.f;  m_frontFace[21] = 0.f;  m_frontFace[24] = uv[4];
	m_frontFace[19] = 0.f;  m_frontFace[22] = 0.f;  m_frontFace[25] = uv[5];
	m_frontFace[20] = 1.f;  m_frontFace[23] = 1.f;  m_frontFace[26] = 0.f;

	//Vertex 4              //Normal                //UV
	m_frontFace[27] = 1.f;  m_frontFace[30] = 0.f;  m_frontFace[33] = uv[6];
	m_frontFace[28] = 1.f;  m_frontFace[31] = 0.f;  m_frontFace[34] = uv[7];
	m_frontFace[29] = 1.f;  m_frontFace[32] = 1.f;  m_frontFace[35] = 0.f;

	// ------BACK-------
	//Vertex 1              //Normal                //UV
	m_backFace[0] = 0.f;    m_backFace[3] = 0.f;    m_backFace[6] = uv[0];
	m_backFace[1] = 0.f;    m_backFace[4] = 0.f;    m_backFace[7] = uv[1];
	m_backFace[2] = 0.f;    m_backFace[5] = -1.f;   m_backFace[8] = 0.f;

	//Vertex 2              //Normal                //UV
	m_backFace[9] = 0.f;    m_backFace[12] = 0.f;   m_backFace[15] = uv[2];
	m_backFace[10] = 1.f;   m_backFace[13] = 0.f;   m_backFace[16] = uv[3];
	m_backFace[11] = 0.f;   m_backFace[14] = -1.f;  m_backFace[17] = 0.f;

	//Vertex 3              //Normal                //UV
	m_backFace[18] = 1.f;   m_backFace[21] = 0.f;   m_backFace[24] = uv[4];
	m_backFace[19] = 0.f;   m_backFace[22] = 0.f;   m_backFace[25] = uv[5];
	m_backFace[20] = 0.f;   m_backFace[23] = -1.f;  m_backFace[26] = 0.f;

	//Vertex 4              //Normal                //UV
	m_backFace[27] = 1.f;   m_backFace[30] = 0.f;   m_backFace[33] = uv[6];
	m_backFace[28] = 1.f;   m_backFace[31] = 0.f;   m_backFace[34] = uv[7];
	m_backFace[29] = 0.f;   m_backFace[32] = -1.f;  m_backFace[35] = 0.f;

    // ---------BOTTOM------------
	//Vertex 1              //Normal                 //UV
	m_bottomFace[0] = 0.f;  m_bottomFace[3] = 0.f;   m_bottomFace[6] = uv[0];
	m_bottomFace[1] = 0.f;  m_bottomFace[4] = -1.f;  m_bottomFace[7] = uv[1];
	m_bottomFace[2] = 0.f;  m_bottomFace[5] = 0.f;   m_bottomFace[8] = 0.f;

	//Vertex 2              //Normal                 //UV
	m_bottomFace[9] = 1.f;  m_bottomFace[12] = 0.f;  m_bottomFace[15] = uv[2];
	m_bottomFace[10] = 0.f; m_bottomFace[13] = -1.f; m_bottomFace[16] = uv[3];
	m_bottomFace[11] = 0.f; m_bottomFace[14] = 0.f;  m_bottomFace[17] = 0.f;

	//Vertex 3              //Normal                 //UV
	m_bottomFace[18] = 0.f; m_bottomFace[21] = 0.f;  m_bottomFace[24] = uv[4];
	m_bottomFace[19] = 0.f; m_bottomFace[22] = -1.f; m_bottomFace[25] = uv[5];
	m_bottomFace[20] = 1.f; m_bottomFace[23] = 0.f;  m_bottomFace[26] = 0.f;

	//Vertex 4              //Normal                 //UV
	m_bottomFace[27] = 1.f; m_bottomFace[30] = 0.f;  m_bottomFace[33] = uv[6];
	m_bottomFace[28] = 0.f; m_bottomFace[31] = -1.f; m_bottomFace[34] = uv[7];
	m_bottomFace[29] = 1.f; m_bottomFace[32] = 0.f;  m_bottomFace[35] = 0.f;


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
