#include "triangle_mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

namespace M3D_ISICG
{
	TriangleMesh::TriangleMesh( const std::string &				  p_name,
								const std::vector<Vertex> &		  p_vertices,
								const std::vector<unsigned int> & p_indices,
								const Material &				  p_material ) :
		_name( p_name ),
		_vertices( p_vertices ), _indices( p_indices ), _material( p_material )
	{
		_vertices.shrink_to_fit();
		_indices.shrink_to_fit();
		_setupGL();
	}

	void TriangleMesh::render( const GLuint p_glProgram ) const { 
		// DIFFUSE
		GLint diffuseColor	= glGetUniformLocation( p_glProgram, "uDiffuseColor" );
		GLint diffuseMap	= glGetUniformLocation( p_glProgram, "uHasDiffuseMap" );

		glProgramUniform3fv( p_glProgram, diffuseColor, 1, glm::value_ptr( _material._diffuse ) );
		glProgramUniform1i( p_glProgram, diffuseMap, _material._hasDiffuseMap );
		if ( _material._hasDiffuseMap )
			glBindTextureUnit( 1, _material._diffuseMap._id );

		// AMBIENT 
		GLint ambienteColor = glGetUniformLocation( p_glProgram, "uAmbientColor" );
		GLint ambienteMap   = glGetUniformLocation( p_glProgram, "uHasAmbientMap" );

		glProgramUniform3fv( p_glProgram, ambienteColor, 1, glm::value_ptr( _material._ambient ) );
		glProgramUniform1i( p_glProgram, ambienteMap, _material._hasAmbientMap );
		if ( _material._hasAmbientMap )
			glBindTextureUnit( 2, _material._ambientMap._id );
		
		// SPECULAIRE
		GLint specularColor = glGetUniformLocation( p_glProgram, "uSpecularColor" );
		GLint specularMap   = glGetUniformLocation( p_glProgram, "uHasSpecularMap" );

		glProgramUniform3fv( p_glProgram, specularColor, 1, glm::value_ptr( _material._specular ) );
		glProgramUniform1i( p_glProgram, specularMap, _material._hasSpecularMap );
		if ( _material._hasSpecularMap )
			glBindTextureUnit( 3, _material._specularMap._id );

		// SHININESS
		GLint shininess = glGetUniformLocation( p_glProgram, "uShininess" );
		glProgramUniform1f( p_glProgram, shininess, _material._shininess );

		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );

		glBindTextureUnit( 1, 0 );
		glBindTextureUnit( 2, 0 );
		glBindTextureUnit( 3, 0 );
	}

	void TriangleMesh::cleanGL()
	{
		glDisableVertexArrayAttrib( _vao, 0 );
		glDisableVertexArrayAttrib( _vao, 1 );
		glDisableVertexArrayAttrib( _vao, 2 );
		glDisableVertexArrayAttrib( _vao, 3 );
		glDisableVertexArrayAttrib( _vao, 4 );
		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
	}

	void TriangleMesh::_setupGL()
	{	
		// VBO
		glCreateBuffers( 1, &_vbo);
		glNamedBufferData( _vbo, _vertices.size() * sizeof( Vertex ), _vertices.data(), GL_STATIC_DRAW );
 
		// VAO
		unsigned int stride = sizeof( Vertex );
		glCreateVertexArrays( 1, &_vao );
		 
		// POSITION
		glEnableVertexArrayAttrib( _vao, 0 );
		glVertexArrayAttribFormat( _vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _vao, 0, _vbo, offsetof( Vertex, _position ), stride );
		glVertexArrayAttribBinding( _vao, 0, 0 );

		// NORMAL
		glEnableVertexArrayAttrib( _vao, 1 );
		glVertexArrayAttribFormat( _vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _vao, 1, _vbo, offsetof( Vertex, _normal ), stride );
		glVertexArrayAttribBinding( _vao, 1, 1 );
		 
		// TEXTURE
		glEnableVertexArrayAttrib( _vao, 2 );
		glVertexArrayAttribFormat( _vao, 2, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _vao, 2, _vbo, offsetof( Vertex, _texCoords ), stride );
		glVertexArrayAttribBinding( _vao, 2, 2 );
		 
		// TANGENT
		glEnableVertexArrayAttrib( _vao, 3 );
		glVertexArrayAttribFormat( _vao, 3, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _vao, 3, _vbo, offsetof( Vertex, _tangent ), stride );
		glVertexArrayAttribBinding( _vao, 3, 4 );
		 
		// BITANGENT
		glEnableVertexArrayAttrib( _vao, 4 );
		glVertexArrayAttribFormat( _vao, 4, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _vao, 4, _vbo, offsetof( Vertex, _bitangent ), stride );
		glVertexArrayAttribBinding( _vao, 4, 4 );
		 
		 
		// EBO
		glCreateBuffers( 1, &_ebo );
		glNamedBufferData( _ebo, _indices.size() * sizeof( unsigned int ), _indices.data(), GL_STATIC_DRAW );
		glVertexArrayElementBuffer( _vao, _ebo );
	}
} // namespace M3D_ISICG
