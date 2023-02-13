#include <geometrybuilder.h>
#include <displayList.h>
#include <engine.h>

namespace AWK{

  uint32_t GeometryBuilder::nextGeoID = 0;

  GeometryBuilder::GeometryBuilder() {
    geoID = nextGeoID++;
    NumberVertices = 0;
  }

  GeometryBuilder::~GeometryBuilder() {
  }

  void GeometryBuilder::addVertex(VertexAtrrib* vertex_attrib_info){
    //IFFF 
    vertices.push_back(std::move(*vertex_attrib_info));
    id_vertices.push_back(NumberVertices);
    NumberVertices++;
  }

  uint32_t GeometryBuilder::createGeometry(Engine& engine) {   
    //Create the command and push  back to the DL
    DisplayList d_list;
    d_list.createGeometry(std::move(vertices), std::move(id_vertices));
    engine.submit(std::move(d_list));

    return geoID;
  }

}