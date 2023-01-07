#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "lab_work_3.hpp"
#include "utils/read_file.hpp"
#include "utils/random.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";

	LabWork3::Mesh LabWork3::_createCube()
	{
		LabWork3::Mesh mesh;

		mesh._sommets_Position
			= { Vec3f( .5, .5, .5 ),  Vec3f( .5, .5, -.5 ),	 Vec3f( .5, -.5, .5 ),	Vec3f( .5, -.5, -.5 ),
				Vec3f( -.5, .5, .5 ), Vec3f( -.5, .5, -.5 ), Vec3f( -.5, -.5, .5 ), Vec3f( -.5, -.5, -.5 ) };
		mesh._sommets_Couleur = { Vec3f( getRandomVec3f() ), Vec3f( getRandomVec3f() ), Vec3f( getRandomVec3f() ),
								  Vec3f( getRandomVec3f() ), Vec3f( getRandomVec3f() ), Vec3f( getRandomVec3f() ),
								  Vec3f( getRandomVec3f() ), Vec3f( getRandomVec3f() ) };
		mesh._sommets_Indices = { 0, 1, 2, 3, 1, 2, 1, 3, 5, 7, 3, 5, 5, 7, 4, 6, 7, 4,
								  4, 6, 0, 2, 6, 0, 5, 4, 1, 0, 4, 1, 6, 7, 2, 3, 7, 2 };
		mesh._objet_transformation = glm::scale( glm::mat4( 1.f ), Vec3f( .8 ) );
		return mesh;
	}

	LabWork3::~LabWork3()
	{
		// ##################################################################################
		// Destucteur de labWork3
		// ##################################################################################
		glDisableVertexArrayAttrib( _cube._vao, 0 );
		glDeleteVertexArrays( 1, &_cube._vao );
	}

	bool LabWork3::init()
	{
		_program				  = glCreateProgram();
		bool resultCreationShader = LabWork3::_initShader();
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		glEnable( GL_DEPTH_TEST );

		// ##################################################################################
		// GENERAL DATA
		// ##################################################################################
		_cube = LabWork3::_createCube();
		//_time				= 0.f;
		std::cout << "Initializing lab work 3..." << std::endl;
		LabWork3::_initBuffers();
		LabWork3::_initCamera();


		// ##################################################################################
		// UNIFORM SHADER DATA
		// ##################################################################################
		_ModelMatrix = glGetUniformLocation( _program, "uModelMatrix" );
		_updateModelMatrix();
		_ViewMatrix = glGetUniformLocation( _program, "uViewMatrix" );
		_updateViewMatrix();
		_ProjectionMatrix = glGetUniformLocation( _program, "uProjectionMatrix" );
		_updateProjectionMatrix();

		// ##################################################################################
		// BUFFER INIT & VBO EBO VAO
		// ##################################################################################
		
		glUseProgram( _program );
		// ##################################################################################
		// END
		// ##################################################################################
		std::cout << "Use program" << std::endl;

		return resultCreationShader;
	}

	void LabWork3::_initBuffers() {
		// ##################################################################################
		// VBO DATA
		// ##################################################################################

		// Création de la VBO vertice -------------------------------------------------------
		glCreateBuffers( 1, &_cube._vboVertice );
		glNamedBufferData( 
			_cube._vboVertice,
			sizeof( Vec3f ) * _cube._sommets_Position.size(),
			_cube._sommets_Position.data(),
			GL_STATIC_DRAW
		);

		std::cout << "VBO vertices created" << std::endl;

		// Création de la VBO color ---------------------------------------------------------
		glCreateBuffers( 1, &_cube._vboColor );
		glNamedBufferData( 
			_cube._vboColor,
			sizeof( Vec3f ) * _cube._sommets_Couleur.size(),
			_cube._sommets_Couleur.data(),
			GL_STATIC_DRAW
		);

		std::cout << "VBO colors created" << std::endl;

		// ##################################################################################
		// EBO DATA
		// ##################################################################################

		// Création de la EBO ---------------------------------------------------------------
		glCreateBuffers( 1, &_cube._ebo );
		glNamedBufferData(
			_cube._ebo, 
			sizeof( int ) * _cube._sommets_Indices.size(), 
			_cube._sommets_Indices.data(), 
			GL_STATIC_DRAW
		);

		std::cout << "EBO created" << std::endl;

		// ##################################################################################
		// VAO DATA
		// ##################################################################################

		// Creez le VAO ---------------------------------------------------------------------
		glCreateVertexArrays( 1, &_cube._vao );

		// Activez l’attribut 0 du VAO ------------------------------------------------------
		glEnableVertexArrayAttrib( _cube._vao, 0 ); // vertices
		glEnableVertexArrayAttrib( _cube._vao, 1 ); // color

		// Definissez le format de l’attribut avec la fonction ------------------------------
		glVertexArrayAttribFormat( _cube._vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribFormat( _cube._vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );

		// Liez le VBO et le VAO avec la fonction -------------------------------------------
		glVertexArrayVertexBuffer( _cube._vao, 0, _cube._vboVertice, 0, sizeof( Vec3f ) );
		glVertexArrayVertexBuffer( _cube._vao, 1, _cube._vboColor	, 0, sizeof( Vec3f));

		// Enfin, connectez le VAO avec le Vertex shader ------------------------------------
		glVertexArrayAttribBinding( _cube._vao, 0, 0 );
		glVertexArrayAttribBinding( _cube._vao, 1, 1 );

		// On relie l'EBO à la VAO ----------------------------------------------------------
		glVertexArrayElementBuffer( _cube._vao, _cube._ebo );

	}

	bool LabWork3::_initShader() {
		// ##################################################################################
		// Création de nos shader
		// ##################################################################################
		//
		// Lecture des shader ---------------------------------------------------------------
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw3.vert" );
		const std::string fragntShaderStr = readFile( _shaderFolder + "lw3.frag" );

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

	}

	void LabWork3::_initCamera() { 
		_camera.setPosition( Vec3f( 0, 1, 3 ) );
		_camera.setScreenSize( _windowWidth, _windowHeight );
	}

	void LabWork3::animate( const float p_deltaTime ){
		
		_cube._objet_transformation = glm::rotate(
			_cube._objet_transformation, 
			(float)glm::radians( p_deltaTime * 0.5 ), 
			Vec3f( 0.f, 1.f, 1.f ) 
		);

		glProgramUniformMatrix4fv( _program, _ModelMatrix, 1, GL_FALSE, glm::value_ptr( _cube._objet_transformation ) );
	}

	void LabWork3::render()
	{
		// Effacez le frame buffer avec la fonction -----------------------------------------
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Liez le VAO au programme avec ----------------------------------------------------
		glBindVertexArray( _cube._vao );
		//animate( 1.f );

		// Lancez le pipeline  --------------------------------------------------------------
		glDrawElements( GL_TRIANGLES, _cube._sommets_Indices.size(), GL_UNSIGNED_INT, 0 );

		// Deliez le VAO et le programme (il faut lui passer 0) -----------------------------
		glBindVertexArray( 0 );
	}

	void LabWork3::displayUI()
	{
		ImGui::Begin( "Settings lab work 3" );
		if ( ImGui::ColorEdit3( "couleur fond", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}
		if ( ImGui::SliderFloat( "Fovy", &_fovySlider, 60.f, 120.f ) )
		{
			_camera.setFovy( _fovySlider);
			_updateProjectionMatrix();
		}
		ImGui::End();
	}

	void LabWork3::_updateViewMatrix()
	{

		glProgramUniformMatrix4fv( 
			_program, 
			_ViewMatrix,
			1, 
			GL_FALSE, 
			glm::value_ptr( _camera.getViewMatrix() ) 
		);	
	}

	void LabWork3::_updateProjectionMatrix() 
	{
		glProgramUniformMatrix4fv( 
			_program, 
			_ProjectionMatrix, 
			1, 
			GL_FALSE, 
			glm::value_ptr( _camera.getProjectionMatrix() ) 
		);
	}

	void LabWork3::_updateModelMatrix()
	{
		glProgramUniformMatrix4fv( 
			_program, 
			_ModelMatrix, 
			1, 
			GL_FALSE, 
			glm::value_ptr( _cube._objet_transformation ) 
		);
	}

	void LabWork3::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}
	}

} // namespace M3D_ISICG
