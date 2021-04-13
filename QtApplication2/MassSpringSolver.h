#ifndef MASSSPRINGSOLVER_H
#define MASSSPRINGSOLVER_H

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace MS_Solver {
	struct Spring {
		std::pair<unsigned int, unsigned int> vertices;
		float ori_length;
		float current_length;
		float k;
	};

	struct Vertex {
		Eigen::Vector3f pos;
		std::vector<int> springs;
		float m;
		Eigen::Vector3f v;
		Eigen::Vector3f a;
	};
}

class MassSpringSolver
{
private:
	typedef Eigen::SparseMatrix<float> SparseMatrix;
	typedef Eigen::Vector3f Vector3f;
	typedef Eigen::VectorXf VectorXf;
	typedef MS_Solver::Spring Spring;
	typedef MS_Solver::Vertex Vertex;

	// data
	std::vector<Spring> spring_list;
	std::vector<Vertex> vertex_list;
	std::vector<Vector3f> vertex_spring_force_list;

	std::vector<Vector3f> a1, a2, b1, b2, c1, c2, d1, d2;
	std::vector<bool> lock_list;

	Vector3f ballPos;
	float ballR;

public:
	MassSpringSolver(std::vector<Vertex> given_list_vertex, std::vector<Spring> given_list_spring);
	void setVertex(std::vector<Vertex> given_list) { vertex_list = given_list; }
	std::vector<Vertex> getVertex() { return vertex_list; }
	void setSpring(std::vector<Spring> given_list) { spring_list = given_list; }
	std::vector<Spring> getSpring() { return spring_list; }

	void lockVertex(int id);
	void solveNext(float time);
	void computeSpringForce(Vector3f xI, Vector3f xJ, Vector3f vI, Vector3f vJ, int index, float l0);

	bool enable_ball;
	bool enable_wind;
	float wind_speed;
	float time_space;

};

#endif

