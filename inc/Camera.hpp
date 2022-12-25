#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
	private:
		const float MAX_STEP = 42.0f;
		const float MIN_STEP = 0.00042f;
		float		_step;
		glm::vec3	_eye;
		glm::vec3	_center;
		glm::vec3	_up;
		glm::mat4	_look_at;
		Camera() {}
		void	_move(glm::vec3 vec)
		{
			_eye += vec;
			_center += vec;
			//std::cout << "move: _eye : " << _eye.x << ", " << _eye.y << ", " << _eye.z << std::endl;
			_look_at = glm::lookAt(this->_eye, this->_center, this->_up);
		}
	public: 
		Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up, float step = 0.042)
		: _eye(eye), _center(center), _up(up), _step(step)
		{
			_look_at = glm::lookAt(this->_eye, this->_center, this->_up);
			std::cout << "camera set" << std::endl;
			std::cout << "_eye : " << _eye.x << ", " << _eye.y << ", " << _eye.z << std::endl;
			std::cout << "_center : " << _center.x << ", " << _center.y << ", " << _center.z << std::endl;
			std::cout << "Cx : " << _look_at[0].x << ", " << _look_at[0].y << ", " << _look_at[0].z << std::endl;
			std::cout << "Cy : " << _look_at[1].x << ", " << _look_at[1].y << ", " << _look_at[1].z << std::endl;
			std::cout << "Cz : " << _look_at[2].x << ", " << _look_at[2].y << ", " << _look_at[2].z << std::endl;
			std::cout << "Cw : " << _look_at[3].x << ", " << _look_at[3].y << ", " << _look_at[3].z << std::endl;
		}

		float getStep() const;
		void setStep(float step);

		const glm::mat4 & lookAt() const;

		void go_foward(float step);
		void go_backward(float step);
		void go_right(float step);
		void go_left(float step);
		void go_up(float step);
		void go_down(float step);

		void go_foward() { go_foward(_step); }
		void go_backward() { go_backward(_step); }
		void go_right() { go_right(_step); }
		void go_left() { go_left(_step); }
		void go_up() { go_up(_step); }
		void go_down() { go_down(_step); }


		void turn_left(float step);
		void turn_right(float step);
		void turn_up(float step);
		void turn_down(float step);
		void turn_cw(float step);
		void turn_ccw(float step);

		void turn_left() { turn_left(_step); }
		void turn_right() { turn_right(_step); }
		void turn_up() { turn_up(_step); }
		void turn_down() { turn_down(_step); }
		void turn_cw() { turn_cw(_step); }
		void turn_ccw() { turn_ccw(_step); }

		void rotate(float rad, glm::vec3 axis);
};

float Camera::getStep() const
{
	return (this->_step);
}

void Camera::setStep(float s)
{
	this->_step = std::min(std::max(s, MIN_STEP), MAX_STEP);
}


///*
//	Rotate the camera's orientation around the reference axis.
//*/
void Camera::rotate(float rad, glm::vec3 cam_axis)
{
	glm::vec3 eye_to_center = this->_center - this->_eye;

	glm::vec4 global_axis = glm::vec4(cam_axis, 0) * _look_at;
	glm::vec3 axis(global_axis.x, global_axis.y, global_axis.z);
	glm::mat4 rotate_mat = glm::rotate(glm::mat4(1.0f), rad, axis);
	eye_to_center = glm::vec3( rotate_mat * glm::vec4(eye_to_center, 0) );
	_up = glm::vec3( rotate_mat * glm::vec4(_up, 0.0f));

	this->_center = this->_eye + eye_to_center;
	_look_at = glm::lookAt(this->_eye, this->_center, this->_up);
}

const glm::mat4 & Camera::lookAt() const
{
	return (_look_at);
}


void Camera::go_foward(float step)
{
	glm::vec3 cam_foward = glm::vec3(_look_at[0].z, _look_at[1].z, _look_at[2].z) * -step;
	_move(cam_foward);
}

void Camera::go_backward(float step)
{
	glm::vec3 cam_backward = glm::vec3(_look_at[0].z, _look_at[1].z, _look_at[2].z) * step;
	_move(cam_backward);
}

void Camera::go_right(float step)
{
	glm::vec3 cam_right = glm::vec3(_look_at[0].x, _look_at[1].x, _look_at[2].x) * step;
	_move(cam_right);
}

void Camera::go_left(float step)
{
	glm::vec3 cam_left = glm::vec3(_look_at[0].x, _look_at[1].x, _look_at[2].x) * -step;
	_move(cam_left);
}

void Camera::go_up(float step)
{
	glm::vec3 cam_up = glm::vec3(_look_at[0].y, _look_at[1].y, _look_at[2].y) * step;
	_move(cam_up);
}

void Camera::go_down(float step)
{
	glm::vec3 cam_down = glm::vec3(_look_at[0].y, _look_at[1].y, _look_at[2].y)  * -step;
	_move(cam_down);
}

void Camera::turn_left(float step)
{

}

void Camera::turn_right(float step)
{

}

void Camera::turn_up(float step)
{

}

void Camera::turn_down(float step)
{

}

void Camera::turn_cw(float step)
{

}

void Camera::turn_ccw(float step)
{

}
