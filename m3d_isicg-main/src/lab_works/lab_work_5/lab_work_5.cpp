#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "lab_work_5.hpp"
#include "common/models/triangle_mesh_model.hpp"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork5::_shaderFolder = "src/lab_works/lab_work_5/shaders/";

	LabWork5::~LabWork5()
	{
	}

	bool LabWork5::init()
	{
		_program				  = glCreateProgram();
		bool resultCreationShader = LabWork5::_initShader();
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		glEnable( GL_DEPTH_TEST );
		
		_meshBunny.load( "bunny1", _meshBunny._dirPath + "bunny2/bunny_2.obj" );
		_meshSponza.load( "sponza1", _meshSponza._dirPath + "sponza/sponza.obj" );
		_meshSponza._transformation = glm::scale( _meshSponza._transformation, glm::vec3( 0.0003 ) );

		_mesh = _meshBunny;

		std::cout << "Initializing lab work 4..." << std::endl;
		LabWork5::_initCamera();

		_MVPMatrixUniform		= glGetUniformLocation( _program, "uMVPMatrix" );
		_MVMatrixUniform		= glGetUniformLocation( _program, "uMVMatrix" );
		_NormalMatrixUniform	= glGetUniformLocation( _program, "uNormalMatrix" );

		LabWork5::_updateMVPMatrix();

		glUseProgram( _program );

		std::cout << "Use program" << std::endl;

		return resultCreationShader;
	}

	bool LabWork5::_initShader()
	{
		// ##################################################################################
		// Création de nos shader
		// ##################################################################################
		//
		// Lecture des shader ---------------------------------------------------------------
		const std::string vertexShaderStr = readFile( _shaderFolder + "mesh_texture.vert" );
		const std::string fragntShaderStr = readFile( _shaderFolder + "mesh_texture.frag" );

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

	void LabWork5::_initCamera()
	{
		_camera.setPosition( Vec3f( 0, 0, 0.2 ) );
		_camera.setScreenSize( _windowWidth, _windowHeight );
	}

	void LabWork5::animate( const float p_deltaTime )
	{
	}

	void LabWork5::render()
	{
		glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
		
		_MVMatrix = _camera.getViewMatrix() * _mesh._transformation;
		glProgramUniformMatrix4fv( _program, _MVMatrixUniform, 1, GL_FALSE, glm::value_ptr( _MVMatrix ) );

		_NormalMatrix = glm::transpose( glm::inverse( _MVMatrix ) ); //  cours 4 P 44
		glProgramUniformMatrix4fv( _program, _NormalMatrixUniform, 1, GL_FALSE, glm::value_ptr( _NormalMatrix ) );

		_mesh.render(_program);
	}

	void LabWork5::displayUI()
	{
		ImGui::Begin( "Settings lab work 5" );
		if ( ImGui::ColorEdit3( "couleur fond", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}
		if ( ImGui::SliderFloat( "Fovy", &_fovySlider, 60.f, 120.f ) )
		{
			_camera.setFovy( _fovySlider );
			_updateMVPMatrix();
		}
		if ( ImGui::Button( _mesh._name.c_str() ) )
		{
			_selectMesh = ( _selectMesh + 1 ) % 2;
			
			if ( _selectMesh )
				_mesh = _meshBunny;
			else
				_mesh = _meshSponza;
		
		}
		ImGui::End();
	}

	void LabWork5::_updateMVPMatrix(){
		_MVPMatrix = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _mesh._transformation;
		glProgramUniformMatrix4fv( _program, _MVPMatrixUniform, 1, GL_FALSE, glm::value_ptr( _MVPMatrix ) );
	}

	void LabWork5::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateMVPMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateMVPMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateMVPMatrix();
		}
	}

} // namespace M3D_ISICG
