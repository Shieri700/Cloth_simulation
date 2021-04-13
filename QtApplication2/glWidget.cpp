#include "glWidget.h"
#include "objReader.h"
#include "math.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "GL/glut.h"

using namespace std;

GlWidget::GlWidget(QWidget* parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

	camera_yaw = 0.0f;
	camera_pitch = -20.0f;
	camera_distance = 10.0f;
	win_width = win_height = 400;
	
	obj = new Crane_Graphics::ObjReader();

	on_animation = false;
	fix_points = false;
	enable_ball = false;
	enable_wind = false;
	startTimer(10);

}

void GlWidget::initializeGL() {
	// set up the lighting
	float  light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
	float  light0_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float  light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float  light0_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.5, 0.8, 0.5, 0.0);

	int returnValueInit = obj->init("cloth2.obj");
	//int returnValueInit = obj->init("testFile.obj");
	switch (returnValueInit) {
	case -1:
		OutputDebugStringW(L"file load xxxx=============");
		break;

	case -2:
		OutputDebugStringW(L"face Normal xxxx=============");
		break;

	case -3:
		OutputDebugStringW(L"Vert Normal xxxx=============");
		break;

	case -4:
		OutputDebugStringW(L"init Buffer xxxx=============");
		break;

	default:
		break;

	}
	initVertices();
}

void GlWidget::initVertices() {
	std::vector<glm::vec3> given_vertices = obj->getVertices();
	for (int i = 0; i < given_vertices.size(); i++) {
		Eigen::Vector3f pos(given_vertices[i].x, given_vertices[i].y, given_vertices[i].z);
		std::vector<int> connectedVertices;
		MS_Solver::Vertex v = { pos, // pos
								connectedVertices, // connected verteices
								0.005f, // m
								Eigen::Vector3f(0.f, 0.f, 0.f), // v
								Eigen::Vector3f(0.f, 0.f, 0.f) // a
								};
		m_vertices.push_back(v);
	}

	std::vector<Crane_Graphics::Square> m_faces = obj->getFacesSquare();
	int i = 0;
	float gap_between_v = (m_vertices[m_faces[i].v00].pos - m_vertices[m_faces[i].v01].pos).norm();
	for (i = 0; i < m_faces.size(); i++) {
		Edge e00 = { m_faces[i].v00,
						m_faces[i].v01
		};
		Edge e01 = { m_faces[i].v01,
						m_faces[i].v00
		};
		if (m_edges.find(e00) == m_edges.end() && m_edges.find(e01) == m_edges.end()) {
			m_edges.insert({ e00, m_edges.size() });
			float norm = (m_vertices[e00.v1].pos - m_vertices[e00.v2].pos).norm();
			MS_Solver::Spring s = {	{e00.v1, e00.v2}, // vertex
									norm,
									norm,
									1.f
			};
			m_springs.push_back(s);
			m_vertices[e00.v1].springs.push_back(m_edges.size() - 1);
			m_vertices[e00.v2].springs.push_back(m_edges.size() - 1);
		}
		Edge e10 = { m_faces[i].v01,
						m_faces[i].v11
		};
		Edge e11 = { m_faces[i].v11, 
						m_faces[i].v01 
		};
		if (m_edges.find(e10) == m_edges.end() && m_edges.find(e11) == m_edges.end()) {
			m_edges.insert({ e10, m_edges.size() });
			float norm = (m_vertices[e10.v1].pos - m_vertices[e10.v2].pos).norm();
			MS_Solver::Spring s = { {e10.v1, e10.v2}, // vertex
									norm,
									norm,
									1.f
			};
			m_springs.push_back(s);
			m_vertices[e10.v1].springs.push_back(m_edges.size() - 1);
			m_vertices[e10.v2].springs.push_back(m_edges.size() - 1);
		}
		Edge e20 = { m_faces[i].v11,
						m_faces[i].v10
		};
		Edge e21 = { m_faces[i].v10, // v1
						m_faces[i].v11 // v2
		};
		if (m_edges.find(e20) == m_edges.end() && m_edges.find(e21) == m_edges.end()) {
			m_edges.insert({ e20, m_edges.size() });
			float norm = (m_vertices[e20.v1].pos - m_vertices[e20.v2].pos).norm();
			MS_Solver::Spring s = { {e20.v1, e20.v2}, // vertex
									norm,
									norm,
									1.f
			};
			m_springs.push_back(s);
			m_vertices[e20.v1].springs.push_back(m_edges.size() - 1);
			m_vertices[e20.v2].springs.push_back(m_edges.size() - 1);
		}
		Edge e30 = { m_faces[i].v00,
						m_faces[i].v10
		};
		Edge e31 = { m_faces[i].v10, 
						m_faces[i].v00 
		};
		if (m_edges.find(e30) == m_edges.end() && m_edges.find(e31) == m_edges.end()) {
			m_edges.insert({ e30, m_edges.size() });
			float norm = (m_vertices[e30.v1].pos - m_vertices[e30.v2].pos).norm();
			MS_Solver::Spring s = { {e30.v1, e30.v2}, // vertex
									norm,
									norm,
									1.f
			};
			m_springs.push_back(s);
			m_vertices[e30.v1].springs.push_back(m_edges.size() - 1);
			m_vertices[e30.v2].springs.push_back(m_edges.size() - 1);
		}


		Edge e40 = { m_faces[i].v00,
						m_faces[i].v11
		};
		m_edges.insert({ e40, m_edges.size() });
		float norm = (m_vertices[e40.v1].pos - m_vertices[e40.v2].pos).norm();
		MS_Solver::Spring s = { {e40.v1, e40.v2}, // vertex
								norm,
								norm,
								1.f
		};
		m_springs.push_back(s);
		m_vertices[e40.v1].springs.push_back(m_edges.size() - 1);
		m_vertices[e40.v2].springs.push_back(m_edges.size() - 1);
		Edge e50 = { m_faces[i].v10,
						m_faces[i].v01
		};
		m_edges.insert({ e50, m_edges.size() });
		norm = (m_vertices[e50.v1].pos - m_vertices[e50.v2].pos).norm();
		MS_Solver::Spring s50 = { {e50.v1, e50.v2}, // vertex
								norm,
								norm,
								1.f
		};
		m_springs.push_back(s50);
		m_vertices[e50.v1].springs.push_back(m_edges.size() - 1);
		m_vertices[e50.v2].springs.push_back(m_edges.size() - 1);
	}

	// bend springs
	for (int v0 = 0; v0 < m_vertices.size(); v0++){
		int count = 0;
		for (int v1 = 0; v1 < m_vertices.size(); v1++) {
			if (count == 4)
				break;
			float dis = (m_vertices[v0].pos - m_vertices[v1].pos).norm();
			float m = gap_between_v * 0.2f;
			if (dis > gap_between_v + m && dis < gap_between_v * 2 + 2.f * m) {
				count++;

				Edge e0 = { v0, v1 };
				Edge e1 = { v1, v0 };

				if (m_edges.find(e0) == m_edges.end() && m_edges.find(e1) == m_edges.end()) {
					m_edges.insert({ e0, m_edges.size() });
					float norm = (m_vertices[e0.v1].pos - m_vertices[e0.v2].pos).norm();
					MS_Solver::Spring s = { {e0.v1, e0.v2}, // vertex
											norm,
											norm,
											1.f
					};
					m_springs.push_back(s);
					m_vertices[e0.v1].springs.push_back(m_edges.size() - 1);
					m_vertices[e0.v2].springs.push_back(m_edges.size() - 1);
				}
			}
		}
	}

	solver = new MassSpringSolver(m_vertices, m_springs);
	solver->enable_ball = enable_ball;
	solver->enable_wind = enable_wind;
	if (fix_points) {
		solver->lockVertex(0);
		solver->lockVertex(19);
	}
}

void GlWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -camera_distance);
	glRotatef(-camera_pitch, 1.0, 0.0, 0.0);
	glRotatef(-camera_yaw, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);

	float  light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	// paint the floor
	
	float  size = 1.5f;
	int  num_x = 10, num_z = 10;
	double  ox, oz;
	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	ox = -(num_x * size) / 2;
	for (int x = 0; x < num_x; x++, ox += size)
	{
		oz = -(num_z * size) / 2;
		for (int z = 0; z < num_z; z++, oz += size)
		{
			if (((x + z) % 2) == 0)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0.8, 0.8, 0.8);
			glVertex3d(ox, -10.0, oz);
			glVertex3d(ox, -10.0, oz + size);
			glVertex3d(ox + size, -10.0, oz + size);
			glVertex3d(ox + size, -10.0, oz);
		}
	}
	glEnd();

	if (enable_ball) {
		glColor3f(0.7f, 0.f, 0.f);

		GLUquadric* quadratic = gluNewQuadric();
		gluQuadricNormals(quadratic, GLU_SMOOTH);
		gluQuadricTexture(quadratic, GL_TRUE);

		glPushMatrix();
		glTranslatef(0.f, -6.f, 0.f);
		gluSphere(quadratic, 4.5f, 32, 32);
		glPopMatrix();
	}

	glColor3f(0.690f, 0.643f, 0.890f);
	obj->render();

	glFlush();
	//glutSwapBuffers();

}

void GlWidget::resizeGL(int width, int height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)width / height, 0.1f, 500);

	win_width = width;
	win_height = height;
}

void GlWidget::drawMessage(int line_no, const char* message)
{
	int i;
	if (message == NULL)
		return;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, win_width, win_height, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2i(8, 24 + 18 * line_no);
	for (i = 0; message[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GlWidget::idle()
{
	if (on_animation)
	{
		animation_time += 0.03f;
		//for (int i = 0; i < 5; i++) {
			solver->solveNext(0.03f);
		//}
		std::vector<MS_Solver::Vertex> currentVertices = solver->getVertex();
		std::vector<glm::vec3> result;
		for (int i = 0; i < currentVertices.size(); i++) {
			result.push_back(glm::vec3(currentVertices[i].pos.x(), currentVertices[i].pos.y(), currentVertices[i].pos.z()));
		}
		obj->update(result);
		updateGL();
	}
}

void GlWidget::timerEvent(QTimerEvent* event) {
	idle();
}

// free falling
void GlWidget::freefall() {

}

void GlWidget::fixPoints(bool state) {
	on_animation = false;
	fix_points = state;
	reset();
}

void GlWidget::ballChanges(bool state) {
	on_animation = false;
	enable_ball = state;
	reset();
}

void GlWidget::windChanges(bool state) {
	on_animation = false;
	enable_wind = state;
	reset();
}

void GlWidget::reset() {
	solver = new MassSpringSolver(m_vertices, m_springs);
	solver->enable_ball = enable_ball;
	solver->enable_wind = enable_wind;
	if (fix_points) {
		solver->lockVertex(0);
		solver->lockVertex(19);
	}
	std::vector<glm::vec3> result;
	for (int i = 0; i < m_vertices.size(); i++) {
		result.push_back(glm::vec3(m_vertices[i].pos.x(), m_vertices[i].pos.y(), m_vertices[i].pos.z()));
	}
	obj->update(result);
	updateGL();
}

// Save file
void GlWidget::saveFile(std::string name) {
	obj->writeFile(name);
}

// key press ==============
void GlWidget::cameraMove(int index, int move) {
	if (index == 0) { camera_distance += move; }
	else if (index == 1) { camera_pitch += move; }
	else if (index == 2) { camera_yaw += move; }
	updateGL();
}

void GlWidget::keyPressEvent(QKeyEvent* event) {
	switch (event->key()) {
	case Qt::Key_R: {
		// TO DO
		break;
	}
	case Qt::Key_N: {
		// TO DO
		//updateGL();
		break;
	}
	case Qt::Key_P: {
		// TO DO
		//updateGL();
		break;
	}
	case Qt::Key_A: {
		cameraMove(2, -10);
		break;
	}
	case Qt::Key_D: {
		cameraMove(2, 10);
		break;
	}
	case Qt::Key_W: {
		cameraMove(1, -10);
		break;
	}
	case Qt::Key_S: {
		cameraMove(1, 10);
		break;
	}
	case Qt::Key_Minus: {
		cameraMove(0, 2);
		break;
	}
	case Qt::Key_Equal: {
		cameraMove(0, -2);
		break;
	}
	default:
		break;
	}
}