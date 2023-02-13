#pragma once
// ----------------------------------------------------------------------------
// Copyright (C) 2017 Lucas Gonzalez
// Test Material Implementation
// ----------------------------------------------------------------------------



#include <EDK3/geometry.h>

class  Cube : public EDK3::Geometry {
public:
	
  Cube();
  
   bool bindAttribute(const EDK3::Attribute a,
                      unsigned int where_to_bind_attribute) const override;

   void render() const override;

   void Generate() ;

   void Update(float deformation, float deformation2);

   EDK3::ref_ptr<EDK3::dev::Buffer> vertex_buffer;
   EDK3::ref_ptr<EDK3::dev::Buffer> element_buffer;
   unsigned int size_points;
   unsigned int kNTriangles;
protected:
  virtual ~Cube();
private:


}; /* end of EDK Namespace */
