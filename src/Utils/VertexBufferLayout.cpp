#include "VertexBufferLayout.h"

namespace Utils
{
	void VertexBufferLayout::AddElementLayoutFloat( GLuint count, bool normalized )
	{
		layoutElments.push_back( { (GLint)count, GL_FLOAT, normalized, count * sizeof( GLfloat ) } );
		stride += layoutElments.back().size;
	}
}
