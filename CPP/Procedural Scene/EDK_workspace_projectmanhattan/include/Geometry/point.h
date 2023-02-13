#pragma once
// ----------------------------------------------------------------------------
// Copyright (C) 2017 Lucas Gonzalez
// Test Material Implementation
// ----------------------------------------------------------------------------

#include <EDK3/geometry.h>

class Point : public EDK3::Geometry {
public:
	
  Point();
  
   bool bindAttribute(const EDK3::Attribute a,
                      unsigned int where_to_bind_attribute) const override;

   void render() const override;

   void Generate() ;

   EDK3::ref_ptr<EDK3::dev::Buffer> vertex_buffer;
   int intensity;
   int range;
   int minRainForce;
   int maxRainForce;
   float rain[100];

  
protected:
  virtual ~Point();
private:


}; /* end of EDK Namespace */
