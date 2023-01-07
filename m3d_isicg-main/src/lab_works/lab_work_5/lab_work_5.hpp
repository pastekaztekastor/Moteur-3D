#pragma once
#ifndef __LAB_WORK_5_HPP__
#define __LAB_WORK_5_HPP__

#include "GL/gl3w.h"
#include "common/models/triangle_mesh_model.hpp"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork5 : public BaseLabWork
	{
	  public:
		LabWork5() : BaseLabWork() {}
		~LabWork5();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		bool _initShader();
		void _initCamera();

		void _updateMVPMatrix();

		// ================ Scene data
		TriangleMeshModel _meshBunny, _meshSponza, _mesh;

		// ================

		// ================ GL data.
		Mat4f	_MVPMatrix, _MVMatrix, _NormalMatrix;
		GLuint	_program, _MVPMatrixUniform, _MVMatrixUniform, _NormalMatrixUniform;
		GLfloat _time		= 0;
		GLfloat _fovySlider = 60.f;
		GLfloat _zNearSlider = 0.1;
		GLfloat _zFareSlider = 1000.f;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		int	  _selectMesh = 0;
		// ================

		// ================ Dans le .hpp Caméra settings
		Camera _camera;
		float  _cameraSpeed		  = 0.1f;
		float  _cameraSensitivity = 0.1f;
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_5_HPP__
#pragma once
