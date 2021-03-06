#include "Sprite.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "../System/Utils/ShaderHelper.h"

namespace RenderEngine
{
	Sprite::Sprite( std::shared_ptr<Texture2D> pTexture_in,
					std::string initialSubTexture,
					std::shared_ptr<ShaderProgram> pShaderProgram ) :
		pTexture( std::move( pTexture_in ) ),
		pShaderProgram( std::move( pShaderProgram ) )
	{
		// =======================================================================
		// 2--3    3
		// | /   / |
		// 1    1--4
		// -----------------------------------------------------------------------
		constexpr GLfloat vertexCoords[] = {
			// X  Y
			0.f, 0.f,
			0.f, 1.f,
			1.f, 1.f,
			1.f, 0.f
		};

		constexpr GLuint vertexIndices[] = {
			0, 1, 2,
			0, 3, 2
		};

		const std::vector<GLfloat> textureCoords = std::move( GetSubTextureCoordinates( std::move( initialSubTexture ) ) );	

		vertexCoordsBuffer.Init( vertexCoords, 2 * 4 * sizeof( GLfloat ) );

		Utils::VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.AddElementLayoutFloat( 2, false );
		vertexArray.AddBuffer( vertexCoordsBuffer, vertexCoordsLayout );

		textureCoordsBuffer.Init( &textureCoords[0], 2 * 4 * sizeof( GLfloat ) );

		Utils::VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.AddElementLayoutFloat( 2, false );
		vertexArray.AddBuffer( textureCoordsBuffer, textureCoordsLayout );

		indicesBuffer.Init( vertexIndices, 6 );

		vertexArray.Unbind();
		indicesBuffer.Unbind();
	}

	void Sprite::Render( const glm::vec2& position, const glm::vec2& size, float rotation, float depthLayer, size_t frameId ) const
	{
		if( lastFrameId != frameId )
		{
			lastFrameId = frameId;
			const FrameDesc& currentFrameDescription = descFrames[frameId];

			const GLfloat textureCoords[] = {
				// U									V 
				currentFrameDescription.leftBottomUV.x, currentFrameDescription.leftBottomUV.y,
				currentFrameDescription.leftBottomUV.x, currentFrameDescription.rightTopUV.y,
				currentFrameDescription.rightTopUV.x,	currentFrameDescription.rightTopUV.y,
				currentFrameDescription.rightTopUV.x,	currentFrameDescription.leftBottomUV.y,
			};

			textureCoordsBuffer.Update( textureCoords, 2 * 4 * sizeof( GLfloat ) );
		}
		pShaderProgram->Use();
		
		glm::mat4 model( 1.f );

		model = glm::translate( model, glm::vec3( position, 0.f ) );							// /\ to top		
		model = glm::translate( model, glm::vec3( 0.5f * size.x, 0.5f * size.y, 0.f ) );		// ||
		model = glm::rotate( model, glm::radians( rotation ), glm::vec3( 0.f, 0.f, 1.f ) );		// ||		
		model = glm::translate( model, glm::vec3( -0.5f * size.x, -0.5f * size.y, 0.f ) );		// ||	
		model = glm::scale( model, glm::vec3( size, 1.f ) );									// || from bottom

		vertexArray.Bind();
		Utils::ShaderHelper::SetMat4( pShaderProgram->GetID(), "model", model );
		Utils::ShaderHelper::SetFloat( pShaderProgram->GetID(), "depth_layer", depthLayer );

		glActiveTexture( GL_TEXTURE0 );
		pTexture->Bind();

		Renderer::Draw( vertexArray, indicesBuffer, *pShaderProgram );
		glBindVertexArray( 0 );
		vertexArray.Unbind();
	}

	std::vector<GLfloat> Sprite::GetSubTextureCoordinates( std::string initialSubTexture ) const
	{
		auto subTexture = pTexture->GetSubTexture( std::move( initialSubTexture ) );
		GLfloat lbX = subTexture.leftBottomUV.x;
		GLfloat lbY = subTexture.leftBottomUV.y;
		GLfloat rtX = subTexture.rightTopUV.x;
		GLfloat rtY = subTexture.rightTopUV.y;
		std::vector<GLfloat> textureCoords = {
			// U  V
			lbX, lbY,
			lbX, rtY,
			rtX, rtY,
			rtX, lbY
		};

		return std::move( textureCoords );
	}
}
