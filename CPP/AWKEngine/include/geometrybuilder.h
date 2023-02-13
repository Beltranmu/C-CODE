/************************************************
 * GeometryBuilder Class
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/
#ifndef __GEOMETRYBUILDER_H__
#define __GEOMETRYBUILDER_H__ 1

#include <stdint.h>
#include <vector>

namespace AWK{
  class Engine;
  //! GeometryBuilder Class.
  /*! Have the attributes of a vertex and create all the structures to create one geometry*/
  class GeometryBuilder{
    
  public:
    //! VertexAttrub Struct.
    /*! Have the attributes of a vertex */
    struct VertexAtrrib{
      float x, y, z;
      float nx, ny, nz;
      float u, v;
    };
    GeometryBuilder();
    ~GeometryBuilder();


    //! Add a new vertex to the geometry.
    /*!
      \param vertex_attrib_info. Data for a new vertex to add to the geometry.
      \sa createGeometry().
    */
    void addVertex(VertexAtrrib* vertex_attrib_info); 

    //! Create the geometry.
    /*!
      \param engine. Pointer to the engine to add a new geometry to it.
      \sa addVertex().
    */
    uint32_t createGeometry(Engine& engine);

    static uint32_t nextGeoID;/*!< Keeps the id of the next geometry to create. */
    uint32_t NumberVertices;/*!< Number of vertices that the new geometry will have. */
    std::vector<VertexAtrrib> vertices;/*!< The attributes of all the vertices of the geometry to create. */
    std::vector<uint32_t> id_vertices;/*!< ID of the vertices of the geometry to create. */

  private:
    uint32_t geoID;/*!< ID of the geometry to create. */

  };

}

#endif // !__GEOMETRYBUILDER_H__