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

	void TriangleMesh::render( const GLuint p_glProgram ) const
	{
		glBindVertexArray( _vao );
		glEnableVertexAttribArray(  (int)AttributeIndexConversion::POSITION );
		glEnableVertexAttribArray(  (int)AttributeIndexConversion::NORMAL );
		glEnableVertexAttribArray(  (int)AttributeIndexConversion::TEXCOORDS );
		glEnableVertexAttribArray(  (int)AttributeIndexConversion::TANGENT );
		glEnableVertexAttribArray(  (int)AttributeIndexConversion::BITANGENT );
		glUseProgram( p_glProgram );
		glUniform3f( glGetUniformLocation( p_glProgram, "u_ambient" ),
					 _material._ambient.x,
					 _material._ambient.y,
					 _material._ambient.z );
		glUniform3f( glGetUniformLocation( p_glProgram, "u_diffuse" ),
					 _material._diffuse.x,
					 _material._diffuse.y,
					 _material._diffuse.z );

		glUniform3f( glGetUniformLocation( p_glProgram, "u_specular" ),
					 _material._specular.x,
					 _material._specular.y,
					 _material._specular.z );

		glUniform1f( glGetUniformLocation( p_glProgram, "u_shininess" ), _material._shininess );

		glUniform1i( glGetUniformLocation( p_glProgram, "u_hasAmbientMap" ), _material._hasAmbientMap );
		if (_material._hasAmbientMap) {
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, _material._ambientMap._id );
			glUniform1i( glGetUniformLocation( p_glProgram, "u_ambientMap" ), 0 );
		}

		glUniform1i( glGetUniformLocation( p_glProgram, "u_hasDiffuseMap" ), _material._hasDiffuseMap );
		if (_material._hasDiffuseMap) {
			glActiveTexture( GL_TEXTURE1 );
			glBindTexture( GL_TEXTURE_2D, _material._diffuseMap._id );
			glUniform1i( glGetUniformLocation( p_glProgram, "u_diffuseMap" ), 1 );
		}

		glUniform1i( glGetUniformLocation( p_glProgram, "u_hasSpecularMap" ), _material._hasSpecularMap );
		if ( _material._hasSpecularMap )
		{
			glActiveTexture( GL_TEXTURE2 );
			glBindTexture( GL_TEXTURE_2D, _material._specularMap._id );
			glUniform1i( glGetUniformLocation( p_glProgram, "u_specularMap" ), 2 );
		}

		glUniform1i( glGetUniformLocation( p_glProgram, "u_hasShininessMap" ), _material._hasShininessMap );
		if ( _material._hasShininessMap )
		{
			glActiveTexture( GL_TEXTURE3 );
			glBindTexture( GL_TEXTURE_2D, _material._shininessMap._id );
			glUniform1i( glGetUniformLocation( p_glProgram, "u_shininessMap" ), 3 );
		}
		

		

		glDrawElements( GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0 );
		
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, 0 );

		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	void TriangleMesh::cleanGL()
	{
		glBindVertexArray( _vao );
		glDisableVertexAttribArray(  (int)AttributeIndexConversion::POSITION );
		glDisableVertexAttribArray( (int)AttributeIndexConversion::NORMAL );
		glDisableVertexAttribArray( (int)AttributeIndexConversion::TEXCOORDS );
		glDisableVertexAttribArray( (int)AttributeIndexConversion::TANGENT );
		glDisableVertexAttribArray( (int)AttributeIndexConversion::BITANGENT );
		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
	}

	void TriangleMesh::_setupGL()
	{	
		glGenBuffers( 1, &_vbo );
		glBindBuffer( GL_ARRAY_BUFFER, _vbo );
		glBufferData( GL_ARRAY_BUFFER, _vertices.size() * sizeof( Vertex ), _vertices.data(), GL_STATIC_DRAW );

		glGenVertexArrays( 1, &_vao );
		glBindVertexArray( _vao );

		glGenBuffers( 1, &_ebo );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _ebo );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof( int ), _indices.data(), GL_STATIC_DRAW );

		glEnableVertexAttribArray( 
			(int)AttributeIndexConversion::POSITION
		);
		glBindBuffer( GL_ARRAY_BUFFER, _vbo );
		glVertexAttribPointer( (int)AttributeIndexConversion::POSITION, // attribute index
							   3, // vec3 = x, y , z
							   GL_FLOAT,
							   GL_FALSE,
							   sizeof( Vertex ), // stride
							   (void*)0		// offset
		);

		glEnableVertexAttribArray( 
			(int)AttributeIndexConversion::NORMAL 
		);
		glBindBuffer( GL_ARRAY_BUFFER, _vbo );
		glVertexAttribPointer( (int)AttributeIndexConversion::NORMAL, // attribute index
							   3, // vec3 = x, y , z
							   GL_FLOAT,
							   GL_FALSE,
							   sizeof( Vertex ), // stride
							   (void *)
			offsetof( Vertex, _normal ) // offset
		);

		glEnableVertexAttribArray( 
			(int)AttributeIndexConversion::TEXCOORDS 
		);
		glBindBuffer( GL_ARRAY_BUFFER, _vbo );
		glVertexAttribPointer( (int)AttributeIndexConversion::TEXCOORDS, // attribute index
							   2, // vec2 = uv
							   GL_FLOAT,
							   GL_FALSE,
							   sizeof( Vertex ), // stride
							   (void *)
			offsetof( Vertex, _texCoords ) // offset
		);

		glEnableVertexAttribArray( 
			(int)AttributeIndexConversion::TANGENT 
		);
		glBindBuffer( GL_ARRAY_BUFFER, _vbo );
		glVertexAttribPointer( (int)AttributeIndexConversion::TANGENT, // attribute index
							   3, // vec3 = x, y , z
							   GL_FLOAT,
							   GL_FALSE,
							   sizeof( Vertex ),					 // stride
							   (void *)
			offsetof( Vertex, _tangent ) // offset
		);

		glEnableVertexAttribArray( 
			(int)AttributeIndexConversion::BITANGENT 
		);
		glBindBuffer( GL_ARRAY_BUFFER, _vbo );
		glVertexAttribPointer( (int)AttributeIndexConversion::BITANGENT, // attribute index
							   3, // vec3 = x, y , z
							   GL_FLOAT,
							   GL_FALSE,
							   sizeof( Vertex ),					 // stride
							   (void *)
			offsetof( Vertex, _bitangent ) // offset
		);



	}
} // namespace M3D_ISICG
