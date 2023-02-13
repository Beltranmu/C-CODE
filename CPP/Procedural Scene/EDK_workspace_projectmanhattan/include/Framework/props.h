#ifndef __PROPS_H__
#define __PROPS_H__ 1

#include <EDK3/referenced.h>
#include <EDK3/ref_ptr.h>
#include <EDK3/drawable.h>
#include <Material/buildingmaterial.h>
#include <EDK3/geometry.h>

class Props : public EDK3::Referenced
{
public:
  Props();
  ~Props();

  void init(EDK3::ref_ptr<EDK3::Node> node,
            EDK3::ref_ptr<EDK3::Geometry> geometry,
            EDK3::ref_ptr<Buildingmaterial> material,
            EDK3::ref_ptr<Buildingmaterial::SettingsBuildingMat> mat_settings);

private:

  EDK3::ref_ptr<EDK3::Drawable> prop_;

};

#endif // !__PROPS_H__