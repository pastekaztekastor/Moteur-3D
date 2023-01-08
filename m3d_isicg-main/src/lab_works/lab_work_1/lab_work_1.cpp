#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";
	


	LabWork1::~LabWork1()
	{
		// ##################################################################################
		// Destucteur de labWork1
		// ##################################################################################
		glDisableVertexArrayAttrib( _vao, 0 );
		glDeleteVertexArrays( 1, &_vao );
	}


	bool LabWork1::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );


		// ##################################################################################
		// Creation de nos shader
		// ##################################################################################
		// 
		// Lecture des shader ---------------------------------------------------------------
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw1.vert" );
		const std::string fragntShaderStr = readFile( _shaderFolder + "lw1.frag" );

		//  Creation des Obj shader ---------------------------------------------------------
		GLuint vshader = glCreateShader( GL_VERTEX_SHADER );
		GLuint fshader = glCreateShader( GL_FRAGMENT_SHADER);

		// Liaison des chaine de caracteres au Obj Shader -----------------------------------
		const GLchar * vSrc = vertexShaderStr.c_str(); // On transforme le std::string en GLchar
		const GLchar * fSrc = fragntShaderStr.c_str(); // On transforme le std::string en GLchar
		
		glShaderSource( vshader,1,&vSrc,NULL ); // vertex_shader_source is a GLchar* containing glsl shader source code
		glShaderSource( fshader,1,&fSrc,NULL ); // vertex_shader_source is a GLchar* containing glsl shader source code

		// Compilation des vertex -----------------------------------------------------------
		glCompileShader( vshader );
		glCompileShader( fshader );

		// CODE COPIER COLLER DU TP MARIA ---------------------------------------------------
		// Check if compilation is ok . // VERTEX SHADER
		GLint veretex_compiled;
		glGetShaderiv( vshader, GL_COMPILE_STATUS, &veretex_compiled );
		if ( !veretex_compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vshader, sizeof( log ), NULL, log );
			glDeleteShader( vshader );
			glDeleteShader( fshader );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}
		// Check if compilation is ok . // FRAGMENT SHADER
		GLint fragment_compiled;
		glGetShaderiv( fshader, GL_COMPILE_STATUS, &fragment_compiled );
		if ( !fragment_compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fshader, sizeof( log ), NULL, log );
			glDeleteShader( vshader );
			glDeleteShader( fshader );
			std ::cerr << " Error compiling fragment shader : " << log << std ::endl;
			return false;
		}

		glShaderSource( vshader, 1, &vSrc, NULL );
		glShaderSource( fshader, 1, &fSrc, NULL );

		// Creation du programme ------------------------------------------------------------
		_program = glCreateProgram();

		// On link les Shader ---------------------------------------------------------------
		glAttachShader( _program, vshader );
		glAttachShader( _program, fshader );

		glLinkProgram( _program );

		// CODE COPIER COLLER DU TP MARIA ---------------------------------------------------
		// Check if link is ok .
		GLint linked;
		glGetProgramiv( _program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}

		// Destruction des Shader -----------------------------------------------------------
		glDeleteShader( vshader );
		glDeleteShader( fshader );

		// #############################################################S#####################
		// Creation de nos donnees - VBO
		// ##################################################################################
 
		// Creation de la VBO ---------------------------------------------------------------
		glCreateBuffers( 1, &_vbo );
		glNamedBufferData( _vbo, sizeof( Vec2f ) * _triangle.size(), _triangle.data(), GL_STATIC_DRAW );

		std::cout << "VBO created" << std::endl;

		// ##################################################################################
		// Creation de nos donnees - VAO
		// ##################################################################################


		// Creez le VAO ---------------------------------------------------------------------
		glCreateVertexArrays( 1, &_vao);

		// Activez leattribut 0 du VAO ------------------------------------------------------
		glEnableVertexArrayAttrib( _vao, 0 );

		// Definissez le format de leattribut avec la fonction ------------------------------
		glVertexArrayAttribFormat( _vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );

		// Liez le VBO et le VAO avec la fonction -------------------------------------------
		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, sizeof(Vec2f));

		// Enfin, connectez le VAO avec le Vertex shader ------------------------------------
		glVertexArrayAttribBinding( _vao, 0, 0 );
		glVertexArrayAttribBinding( _vao, 1, 1 );
		
		std::cout << "VAO created" << std::endl;


		std::cout << "VAO destructed" << std::endl;

		// Specifier le programme a utiliser ------------------------------------------------
		glUseProgram( _program );
		std::cout << "Use program" << std::endl;

	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render() {
		// Effacez le frame buffer avec la fonction -----------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Liez le VAO au programme avec ----------------------------------------------------
		glBindVertexArray( _vao );
		// Lancez le pipeline  --------------------------------------------------------------
		glDrawArrays( GL_TRIANGLES , 0 , 3);
		// Deliez le VAO et le programme (il faut lui passer 0) -----------------------------
		glBindVertexArray( 0 );
	}

	void LabWork1::handleEvents( const SDL_Event & p_event )
	{}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
