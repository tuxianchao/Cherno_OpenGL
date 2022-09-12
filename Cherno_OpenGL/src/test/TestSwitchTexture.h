#pragma once

#include "Test.h"
#include <string>

#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexArray.h"
#include "../Shader.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"
#include "../Renderer.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



/*
可通过imgUI切换混合颜色，纹理，坐标，以及clearColor
*/
namespace Test
{
	class TestSwitchTexture : public Test
	{
	private:
		float				m_Positions[16];
		unsigned int		m_Indices[6];
		VertexArray			m_VA;
		VertexBuffer		m_VB;
		VertexBufferLayout	m_Layout;
		IndexBuffer			m_Ib;
		Shader				m_Shader;
		Texture				m_TextureA;
		Texture				m_TextureB;

		glm::vec3			m_Trans;


		Renderer			m_Renderer;

		float				m_ClearColor[4];
		float				m_MixColor[4];
		bool				m_ActiveA;

	public:
		TestSwitchTexture(const std::string& textureFilePathA, const std::string& textureFilePathB);
		~TestSwitchTexture();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRenderer() override;

		inline VertexArray GetVA() const { return m_VA; }
		inline IndexBuffer  GetIB() const { return m_Ib; }
		inline Shader GetShader() const { return m_Shader; }

	};
}
