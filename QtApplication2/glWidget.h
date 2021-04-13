#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "objReader.h"
#include "MassSpringSolver.h"

class ObjReader;

struct BoundingBox {
	float height, width, depth;
};

struct RigidBody
{
	glm::vec3 v;
	glm::vec3 a;
	float m;
};

struct Edge
{
	bool operator== (const Edge& rhs) const{
		return this->v1 == rhs.v1 && this->v2 == rhs.v2;
	}
	int v1, v2; // ids of two vertices
};

struct Floor
{
	glm::vec3 pos;
	RigidBody rb;
};

namespace std {
	template <>
	struct hash<Edge> {
		size_t operator()(const Edge& other) const {
			using std::size_t;
			using std::hash;

			return ((hash<int>()(other.v1)
				^ (hash<int>()(other.v2) << 1)));
		}
	};
}

class GlWidget : public QGLWidget {
	Q_OBJECT

public:
	GlWidget(QWidget* parent = 0);
	void keyPressEvent(QKeyEvent* event);
	void cameraMove(int index, int move);
	void saveFile(std::string name);
	void fixPoints(bool state);
	void ballChanges(bool state);
	void windChanges(bool state);
	void reset();

	void freefall();

	bool on_animation;

protected:
	void initializeGL();
	void drawMessage(int line_no, const char* message);
	void paintGL();
	void resizeGL(int width, int height);

	void idle();
	void timerEvent(QTimerEvent* event);

	void initVertices();


	// void mousePressEvent(QMouseEvent* event);
	// void mouseMoveEvent(QMouseEvent* event);

private:
	int win_width, win_height;

	float camera_yaw;
	float camera_pitch;
	float camera_distance;

	float animation_time;

	bool fix_points;
	bool enable_ball;
	bool enable_wind;

	std::vector<MS_Solver::Vertex> m_vertices;
	std::unordered_map<Edge, int> m_edges;
	std::vector<MS_Solver::Spring> m_springs;

	Crane_Graphics::ObjReader *obj;
	MassSpringSolver *solver;

};

#endif