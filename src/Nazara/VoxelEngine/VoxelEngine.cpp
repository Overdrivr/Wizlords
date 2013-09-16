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
  static NzRenderStates m_renderStates;
  static NzShaderProgram* m_shader;
}

void NzVoxelEngine::DrawChunk(const NzVoxelChunkMesh& chunk)
{
    NzRenderer::SetMatrix(nzMatrixType_World, NzMatrix4f::Identity());
    NzRenderer::SetRenderStates(m_renderStates);
    NzRenderer::SetFaceFilling(nzFaceFilling_Line);
    NzRenderer::SetShaderProgram(m_shader);

    NzRenderer::SetVertexBuffer(&(chunk.m_vertexBuffer));
    NzRenderer::DrawPrimitives(nzPrimitiveMode_TriangleStrip,0,chunk.m_vertexCount);
}

std::array<float,32> NzVoxelEngine::GetFaceData(nzVoxelFaceOrientation face, NzVector3f offset, unsigned int textureIndex)
{
    std::array<float,32> data;

    switch(face)
    {
        case nzVoxelFaceOrientation_top :
            data = m_topFace;
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
/*
	for(unsigned int i(0) ; i < 4 ; ++i)
    {
        std::cout<<" P("<<data[i*8]<<" ; "<<data[i*8+1]<<" ; "<<data[i*8+2]<<std::endl;
        std::cout<<" N("<<data[i*8+3]<<" ; "<<data[i*8+4]<<" ; "<<data[i*8+5]<<std::endl;
        std::cout<<"UV("<<data[i*8+6]<<" ; "<<data[i*8+7]<<std::endl;
    }
    std::cout<<"------------------------"<<std::endl;*/

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
	//Vertex
	m_topFace[0] = 0.f;
	m_topFace[1] = 1.f;
	m_topFace[2] = 0.f;
	//Normal
	m_topFace[3] = 0.f;
	m_topFace[4] = 1.f;
	m_topFace[5] = 0.f;
	//UV
	m_topFace[6] = 0.f;
	m_topFace[7] = 0.f;
	//Vertex
	m_topFace[8] = 1.f;
	m_topFace[9] = 1.f;
	m_topFace[10] = 0.f;
	//Normal
	m_topFace[11] = 0.f;
	m_topFace[12] = 1.f;
	m_topFace[13] = 0.f;
	//UV
	m_topFace[14] = 1.f;
	m_topFace[15] = 0.f;
	//Vertex
	m_topFace[16] = 0.f;
	m_topFace[17] = 1.f;
	m_topFace[18] = 1.f;
	//Normal
	m_topFace[19] = 0.f;
	m_topFace[20] = 1.f;
	m_topFace[21] = 0.f;
	//UV
	m_topFace[22] = 0.f;
	m_topFace[23] = 1.f;
	//Vertex
	m_topFace[24] = 1.f;
	m_topFace[25] = 1.f;
	m_topFace[26] = 1.f;
	//Normal
	m_topFace[27] = 0.f;
	m_topFace[28] = 1.f;
	m_topFace[29] = 0.f;
	//UV
	m_topFace[30] = 1.f;
	m_topFace[31] = 1.f;

	// Shader
	const char* vertexSource =
    "#version 140\n"
    "in vec3 VertexPosition;\n"
    "in vec3 VertexNormal;\n"
    "uniform mat4 WorldViewProjMatrix;\n"
    "out vec3 normal;\n"
    "out vec3 position;\n"

    "void main()\n"
    "{\n"
	"normal = VertexNormal;\n"
	"position = VertexPosition;\n"
    "gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);\n"
    "}\n";

    const char* fragmentSource =
    "#version 140\n"
    "out vec4 out_Color;\n"
    "in vec3 normal;\n"
    "in vec3 position;\n"

    "void main()\n"
    "{\n"
    "out_Color = vec4(0.0,1.0,0.0,1.0);\n"
    "}\n";

    m_shader = new NzShaderProgram(nzShaderLanguage_GLSL);

    if (!m_shader->LoadShader(nzShaderType_Fragment, fragmentSource))
    {
        NazaraError("Failed to initialize voxel renderer module : Failed to load fragment shader");
        delete m_shader;
        return false;
    }

    if (!m_shader->LoadShader(nzShaderType_Vertex, vertexSource))
    {
        NazaraError("Failed to initialize voxel renderer module : Failed to load vertex shader");
        delete m_shader;
        return false;
    }

    if (!m_shader->Compile())
    {
        NazaraError("Failed to initialize voxel renderer module : Failed to compile shader");
        delete m_shader;
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
	delete m_shader;

	NazaraNotice("Uninitialized: VoxelEngine module");

	// Libération des dépendances
	NzGraphics::Uninitialize();
}

unsigned int NzVoxelEngine::s_moduleReferenceCounter = 0;
