#include <Framework/props.h>

Props::Props()
{
}

Props::~Props()
{
}

void Props::init(EDK3::ref_ptr<EDK3::Node> node,
                 EDK3::ref_ptr<EDK3::Geometry> geometry,
                 EDK3::ref_ptr<Buildingmaterial> material,
                 EDK3::ref_ptr<Buildingmaterial::SettingsBuildingMat> mat_settings) {

  prop_.alloc();

  node->addChild(prop_.get());

  prop_->set_geometry(geometry.get());
  prop_->set_material(material.get());
  prop_->set_material_settings(mat_settings.get());

}