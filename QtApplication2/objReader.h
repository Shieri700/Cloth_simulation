#pragma once
#include <vector>
#include <qopenglfunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <QMatrix4x4>


namespace Crane_Graphics {

	struct Triangle
	{
		unsigned int x = -1;
		unsigned int y = -1;
		unsigned int z = -1;
	};

	struct Square
	{
		unsigned int v00 = -1;
		unsigned int v01 = -1;
		unsigned int v10 = -1;
		unsigned int v11 = -1;
	};

	class ObjReader
	{


	public:
		typedef boost::shared_ptr<ObjReader> ObjReader_Ptr;
	public:
		ObjReader();
		virtual ~ObjReader();

	public:
		virtual int init(std::string file, float scaleCo = 1.0f);
		virtual int update(std::vector<glm::vec3> newVertices);
		virtual int writeFile(std::string objFile);
		virtual int render();

		bool getShowWire() { return m_showWire; }
		bool getShowTexture() { return m_showTexture; }
		bool getShowColor() { return m_showColor; }
		std::vector<glm::vec3> getVertices() { return m_vertices; }
		std::vector<Triangle> getFaces() { return m_triangles; }
		std::vector<Square> getFacesSquare() { return m_squares; }

		void switchShowWire() { m_showWire = !m_showWire; }
		void switchShowTexture() { m_showTexture = !m_showTexture; }
		void switchShowColor() { m_showColor = !m_showColor; }

		QOpenGLVertexArrayObject& getVAO() { return m_vao; }

		int scale(float scale = 1);

		int normalise();
	protected:
		int loadObj(std::string file);
		int computeFaceNormals(bool recompute = false);
		int computeVertNormals();

		int initBuffer();


		//init colors
		int computeVertexColors();
	protected:
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_vertNormals;
		std::vector<glm::vec3> m_vertTexCoords;
		std::vector<glm::vec3> m_colors;
		std::vector<Triangle> m_triangles;
		std::vector<Square> m_squares;
		std::vector<glm::vec3> m_faceNormals;

		bool m_showWire = false;
		bool m_showTexture = false;
		bool m_showColor = true;
		bool m_showNormals = false;

		QOpenGLBuffer m_vbo;

		QOpenGLVertexArrayObject m_vao;


		std::vector<float> m_buffer;

		int m_unitSize = 0;

		QMatrix4x4 m_transMat;

		std::string m_file;

		std::string m_name;
	};

}

