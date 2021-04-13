#include "MassSpringSolver.h"

#define GRAVITY -9.81

MassSpringSolver::MassSpringSolver(std::vector<Vertex> given_list_vertex, std::vector<Spring> given_list_spring) {
	vertex_list = given_list_vertex;
	spring_list = given_list_spring;
	for (int i = 0; i < vertex_list.size(); i++)
		lock_list.push_back(false);

	for (int i = 0; i < vertex_list.size(); i++) {
		vertex_spring_force_list.push_back(Vector3f(0.f, 0.f, 0.f));
		a1.push_back(Vector3f(0.f, 0.f, 0.f));
		a2.push_back(Vector3f(0.f, 0.f, 0.f));
	}
	ballPos = Vector3f(0.f, -6.f, 0.f);
	ballR = 4.6f;
	wind_speed = 5.f;
	time_space = 0.0001f;
}

void MassSpringSolver::lockVertex(int id) {
	lock_list[id] = !lock_list[id];
}

void MassSpringSolver::solveNext(float time) {
	// compute the force each spring

	for (int i = 0; i < vertex_list.size(); i++) {
		vertex_spring_force_list[i] = Vector3f(0.f, 0.f, 0.f);
		for (int j = 0; j < vertex_list[i].springs.size(); j++) {
			unsigned int otherIndex = -1;
			if (spring_list[vertex_list[i].springs[j]].vertices.first == i)
				otherIndex = spring_list[vertex_list[i].springs[j]].vertices.second;
			else
				otherIndex = spring_list[vertex_list[i].springs[j]].vertices.first;

			computeSpringForce(vertex_list[i].pos, vertex_list[otherIndex].pos, vertex_list[i].v, vertex_list[otherIndex].v, i, spring_list[vertex_list[i].springs[j]].ori_length);
		}
		vertex_spring_force_list[i] += Vector3f(0.f, vertex_list[i].m * (GRAVITY / 4.f), 0.f);
		//a1[i] = vertex_list[i].v;
		//a2[i] = (Vector3f(0.f, vertex_list[i].m * GRAVITY, 0.f) + vertex_spring_force_list[i]) / vertex_list[i].m;
	}	

	for (int i = 0; i < vertex_list.size(); i++) {
		//Vector3f total_force = Vector3f(0.f, 0.f, 0.f);
		if (lock_list[i])
			continue;

		Vector3f f = vertex_spring_force_list[i];
		if (enable_wind) {
			f += Vector3f( wind_speed * vertex_list[i].m, 0.f, 0.f );
		}

		vertex_list[i].v += time_space * f / vertex_list[i].m;
		vertex_list[i].pos += time_space * vertex_list[i].v;

		// hits the floor
		if (vertex_list[i].pos.y() <= -9.99f) {
			vertex_list[i].pos = Vector3f(vertex_list[i].pos.x(), -9.99f, vertex_list[i].pos.z());
			vertex_list[i].v = Vector3f(vertex_list[i].v.x(), -vertex_list[i].v.y() * 0.03f, vertex_list[i].v.z());
		}

		if (enable_ball) {
			Vector3f dis = vertex_list[i].pos - ballPos;
			if (dis.norm() < ballR) {
				dis.normalize();
				dis = ballR * dis;

				vertex_list[i].pos = Vector3f(dis.x() + ballPos.x(), dis.y() + ballPos.y(), dis.z() + ballPos.z());
			}
		}
		//7vertex_list[i].pos = Vector3f(vertex_list[i].pos.x() - 0.1f, vertex_list[i].pos.y(), vertex_list[i].pos.z());
		//vertex_list[i].pos -= Vector3f(0.1, 0.1, 0.1);;
	}
}

void MassSpringSolver::computeSpringForce(Vector3f xI, Vector3f xJ, Vector3f vI, Vector3f vJ, int index, float l0) {
	Vector3f pDiff = xJ - xI;
	Vector3f vDiff = vJ - vI;

	Vector3f vec = Vector3f(pDiff);
//	vec *= (2.f / pDiff.norm());
	float c_length = pDiff.norm();

	//Vector3f formula = -(Vector3f(20.f * (c_length - l0), 20.f * (c_length - l0), 20.f * (c_length - l0)) + Vector3f(6.f * (pDiff.dot(vDiff) / c_length), 6.f * (pDiff.dot(vDiff) / c_length), 6.f * (pDiff.dot(vDiff) / c_length)));
	Vector3f damping_force = -10.f * (pDiff / c_length) * (pDiff / c_length).dot(vDiff);

	Vector3f currentForce = 2.f * (pDiff / c_length) * (c_length - l0);

	vertex_spring_force_list[index] += currentForce;
	//vertex_spring_force_list[index] += damping_force;

//	Vector3f vDiff = vertex_list[otherIndex].v - vertex_list[i].v;
//	Vector3f dd((pDiff / c_length).x() * vDiff.x(), (pDiff / c_length).y() * vDiff.y(), (pDiff / c_length).z() * vDiff.z());
//	Vector3f damping_force = -2.f * Vector3f((pDiff / c_length).x() * dd.x(), (pDiff / c_length).y() * dd.y(), (pDiff / c_length).z() * dd.z());
//	vertex_spring_force_list[index] += damping_force;
}

