#pragma once
// ----------------------------------------------------------------------------
// Copyright (C) 2017 Lucas Gonzalez
// Test Material Implementation
// ----------------------------------------------------------------------------



#include <EDK3/geometry.h>

class  Sphere2 : public EDK3::Geometry {
public:
	
  Sphere2();
  
   bool bindAttribute(const EDK3::Attribute a,
                      unsigned int where_to_bind_attribute) const override;

   void render() const override;

   void Generate() ;

   EDK3::ref_ptr<EDK3::dev::Buffer> vertex_buffer;
   EDK3::ref_ptr<EDK3::dev::Buffer> element_buffer;
   unsigned int size_points;
   unsigned int kNTriangles;

protected:
  virtual ~Sphere2();
private:


}; /* end of EDK Namespace */
