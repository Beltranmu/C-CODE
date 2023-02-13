



#include <EDK3/node.h>
#include <EDK3/drawable.h>
#include <EDK3/dev/gpumanager.h>
#include "..\..\include\Mollete\camera.h"
#include <vector>



Mollete::Camera::Camera(){
  clear_color[0] = 0.0f;
  clear_color[1] = 0.0f;
  clear_color[2] = 0.0f;
  clear_color[3] = 0.0f;
}


void Mollete::Camera::doCullTR(const EDK3::Node * root_node, Matrix4x4 matrix) {

  for (int i = 0; i < root_node->num_children(); i++) {
    const EDK3::Drawable* is_drawable = dynamic_cast<const EDK3::Drawable*>(root_node->child(i));
    //const EDK3::Drawable* is_n = dynamic_cast<const EDK3::Drawable*>(root_node->child(i));
    Matrix4x4 child = Matrix4x4(root_node->child(i)->local_transform());
    Matrix4x4 parent_tr = matrix.Multiply(child);

    // This node has children
    if (nullptr != is_drawable) {

      drawable_elements.push_back(is_drawable);
      drawables_tr.push_back(Transformation(parent_tr.m));
    }
    if (root_node->child(i)->num_children() > 0) {
      doCullTR(root_node->child(i), parent_tr);
      //drawables_tr.push_back(Transformation(parent_tr.m));
    }
  }
  //ndrawcalls = drawable_elements.size();
}

void Mollete::Camera::doCull(const EDK3::Node * root_node){

  drawable_elements.clear();
  drawables_tr.clear();
 
  for (int i = 0; i < root_node->num_children(); i++) {
    const EDK3::Drawable* is_drawable = dynamic_cast<const EDK3::Drawable*>(root_node->child(i));
    Matrix4x4 parent_tr = Matrix4x4(root_node->child(i)->local_transform());
 
    // This node has children
    if (nullptr != is_drawable) {

      drawable_elements.push_back(is_drawable);
      drawables_tr.push_back(Transformation(parent_tr.m));
    }
    if (root_node->child(i)->num_children() > 0) {
     // drawables_tr.push_back(Transformation(parent_tr.m));
      doCullTR(root_node->child(i), parent_tr);
    }
  }
  ndrawcalls = drawable_elements.size();
}

void Mollete::Camera::doRender() const{

  EDK3::dev::GPUManager::Instance()->clearFrameBuffer(clear_color);
 
  for (int i = 0; i < drawable_elements.size(); ++i) {
    drawable_elements[i]->material()->enable(drawable_elements[i]->material_settings());
    drawable_elements[i]->material()->setupModel(drawables_tr[i].transform);
    drawable_elements[i]->material()->setupCamera(projection_matrix(), view_matrix());
    drawable_elements[i]->geometry()->bindAttributesAndRenderWithCurrentMaterial(drawable_elements[i]->material());
  }
}

void Mollete::Camera::setClearColor(float r, float g, float b, float a) {
  clear_color[0] = r;
  clear_color[1] = g;
  clear_color[2] = b;
  clear_color[3] = a;
}

Mollete::Camera::~Camera(){
}
