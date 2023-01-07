#include "imgui.h"
#include "lab_work_2.hpp"
#include "utils/read_file.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2()
	{
		// ##################################################################################
		// Destucteur de labWork2
		// ##################################################################################
		glDisableVertexArrayAttrib( _vao, 0 );
		glDeleteVertexArrays( 1, &_vao );
	}

	bool LabWork2::init()
	{
		// ##################################################################################
		// Création des données
		// ##################################################################################
		//
		// vertices -------------------------------------------------------------------------
		_bufferVertices = { Vec2f( -.7, -.7 ), Vec2f( .7, -.7 ), Vec2f( -.7, .7 ), Vec2f( .7, .7 ) };
		_bufferColor	= {
			   1.f, 0.f, 0.f, // rouge
			   0.f, 1.f, 0.f, // vert
			   0.f, 0.f, 1.f, // bleu
			   1.f, 0.f, 1.f, // magenta
		};
		_bufferTriangle = { 0, 1, 2, 1, 2, 3};

		_time = 0.f;
		_alphaCanal = 1.f;

		std::cout << "Initializing lab work 2..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).


		// ##################################################################################
		// Création de nos shader
		// ##################################################################################
		//
		// Lecture des shader ---------------------------------------------------------------
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw2.vert" );
		const std::string fragntShaderStr = readFile( _shaderFolder + "lw2.frag" );

		//  Création des Obj shader ---------------------------------------------------------
		GLuint vshader = glCreateShader( GL_VERTEX_SHADER );
		GLuint fshader = glCreateShader( GL_FRAGMENT_SHADER );

		// Liaison des chaine de caractères au Obj Shader -----------------------------------
		const GLchar * vSrc = vertexShaderStr.c_str(); // On transforme le std::string en GLchar
		const GLchar * fSrc = fragntShaderStr.c_str(); // On transforme le std::string en GLchar

		glShaderSource(
			vshader, 1, &vSrc, NULL ); // vertex_shader_source is a GLchar* containing glsl shader source code
		glShaderSource(
			fshader, 1, &fSrc, NULL ); // vertex_shader_source is a GLchar* containing glsl shader source code

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

		// Création du programme ------------------------------------------------------------
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

		
		// ##################################################################################
		// Création de nos données uniforme de shader
		// ##################################################################################

		_translation = glGetUniformLocation( _program, "uTranslationX" );
		_transparenc = glGetUniformLocation( _program, "uAlphaCanal" );

		// ##################################################################################
		// Création de nos données - VBO
		// ##################################################################################

		// Création de la VBO vertice -------------------------------------------------------
		glCreateBuffers( 1, &_vboVertice );
		glNamedBufferData( _vboVertice, sizeof( Vec2f ) * _bufferVertices.size(), _bufferVertices.data(), GL_STATIC_DRAW );

		// Création de la VBO color ---------------------------------------------------------
		glCreateBuffers( 1, &_vboColor);
		glNamedBufferData( _vboColor, sizeof( float ) * _bufferColor.size(), _bufferColor.data(), GL_STATIC_DRAW );

		std::cout << "VBO created" << std::endl;

		// ##################################################################################
		// Création de nos données - EBO
		// ##################################################################################

		// Création de la EBO ---------------------------------------------------------------
		glCreateBuffers( 1, &_ebo );
		glNamedBufferData( _ebo, sizeof( int ) * _bufferTriangle.size(), _bufferTriangle.data(), GL_STATIC_DRAW );

		std::cout << "EBO created" << std::endl;

		// ##################################################################################
		// Création de nos données - VAO
		// ##################################################################################

		// Creez le VAO ---------------------------------------------------------------------
		glCreateVertexArrays( 1, &_vao );

		// Activez l’attribut 0 du VAO ------------------------------------------------------
		glEnableVertexArrayAttrib( _vao, 0 ); // vertices
		glEnableVertexArrayAttrib( _vao, 1 ); // color

		// Definissez le format de l’attribut avec la fonction ------------------------------
		glVertexArrayAttribFormat( _vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribFormat( _vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );

		// Liez le VBO et le VAO avec la fonction -------------------------------------------
		glVertexArrayVertexBuffer( _vao, 0, _vboVertice, 0, sizeof( Vec2f ) );
		glVertexArrayVertexBuffer( _vao, 1, _vboColor, 0, sizeof( float ) *3  );

		// Enfin, connectez le VAO avec le Vertex shader ------------------------------------
		glVertexArrayAttribBinding( _vao, 0, 0 );
		glVertexArrayAttribBinding( _vao, 1, 1 );

		// On relie l'EBO à la VAO ----------------------------------------------------------
		glVertexArrayElementBuffer( _vao, _ebo );

		// Specifier le programme a utiliser ------------------------------------------------
		glUseProgram( _program );
		std::cout << "Use program" << std::endl;
	}

	void LabWork2::animate( const float p_deltaTime ) {
		std::cout << p_deltaTime << _time << std::endl;
		_time += p_deltaTime*0.05;

		glProgramUniform1f( _program, _translation, glm::sin( _time ));
	}

	void LabWork2::render()
	{
		// Effacez le frame buffer avec la fonction -----------------------------------------
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Liez le VAO au programme avec ----------------------------------------------------
		glBindVertexArray( _vao );
		animate(1.f);
		
		// Lancez le pipeline  --------------------------------------------------------------
		glDrawElements( GL_TRIANGLES, _bufferTriangle.size(), GL_UNSIGNED_INT, 0 );

		// Deliez le VAO et le programme (il faut lui passer 0) -----------------------------
		glBindVertexArray( 0 );


	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		if (ImGui::SliderFloat("Transparence", &_transparencValue, 0.f, 1.f)) {
			glProgramUniform1f( _program, _transparenc, _transparencValue );
		}
		ImGui::Text( "No setting available!" );
		if (ImGui::ColorEdit3("couleur fond", glm::value_ptr(_bgColor))) {
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}
		ImGui::End();
	}

} // namespace M3D_ISICG
