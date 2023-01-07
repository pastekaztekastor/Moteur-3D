#pragma once
#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "define.hpp"
#include <vector>



namespace M3D_ISICG
{
	class LabWork3 : public BaseLabWork
	{
	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		bool creationShader();

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		void _updateViewMatrix();

		struct Mesh
		{
			std::vector<Vec3f>		  _sommets_Position;
			std::vector<Vec3f>		  _sommets_Couleur;
			std::vector<unsigned int> _sommets_Indices;
			Mat4f					  _objet_transformation;
			GLuint					  _vboColor, _vboPosition, _vao, _ebo;
		};

	  private:

		Mesh _createCube();
		void _initBuffers();

		// ================ Scene data.
		Mesh			   _cube;
		// ================

		// ================ GL data.
		GLuint	_program;
		GLfloat _time;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================

		// ================ Dans le .hpp Caméra settings
		Camera _camera;
		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_3_HPP__
#pragma once
