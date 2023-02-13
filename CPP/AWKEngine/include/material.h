
/************************************************
 * Material Struct
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/

#ifndef __MATERIAL_H__
#define __MATERIAL_H__ 1

#include <stdint.h>
namespace AWK {
  //! Material struct.
  /*! Represent a material for the engine. A material will be its MaterialType and its Data.*/
	struct Material
  {

    //! RGBA struct.
    /*! Represent a color with RGBA.*/
    struct RGBA {
      float r; /*!< Red component of the color.*/
      float g; /*!< Green component of the color.*/
      float b; /*!< Blue component of the color.*/
      float a; /*!< Alpha component of the color.*/
    }; 
    //! Material Parameters Struct.
    /*! Has all the different parameter and information that a material will need. Color, type, etc */
    struct MaterialParameters {

    //! Material Enum.
    /*! Different Types of the material */
      enum MaterialType {
        MaterialType_None = 0,       /*!< No material.*/      
        MaterialType_Blur ,          
        MaterialType_TextureColor=2,    /*!< Texture Material.*/
        MaterialType_RenderShadow,
        MaterialType_BlinnPhong,
        MaterialType_Atmosphere,
        MaterialType_Water,
        MaterialType_SolidColor ,  /*!< Solid Color material.*/
        MaterialType_PBR              /*!< PBR material.*/
        // etc...
      };

      //! SolidColor struct.
      /*! Store all the characteristic of the Solid Color type of a material */
      struct SolidColor {
        RGBA color; /*!< Color of the material.*/
      };

      //! TextureColor struct.
      /*! Store all the characteristic of the Texture Color type of a material */
      struct TextureColor {
        RGBA color; /*!< Color of the material.*/
        uint32_t texture; /*!< Textures of the material.*/
      };

      //! PBR struct.
      /*! Store all the characteristic of the PBR type of a material */
      struct PBR {
        RGBA BaseColor;           /*!< Base Color.*/
        RGBA emissive;            /*!< Emissive Color.*/
        float metallic;           /*!< Amount of metallic.*/
        float roughness;          /*!< Amount of roughness.*/
        float reflectance;        /*!< Amount of reflectance.*/
        float ambientOcclussion;  /*!< Amount of ambientOcclussion.*/
      };

      //! Data union.
      /*! Store all the possible information for each type of material.*/
      union Data {
        TextureColor textureColor; /*!< Data from the Texture color.*/
        SolidColor solidColor; /*!< Data from the Solid color.*/
        PBR pbr; /*!< Data from the pbr.*/
      };

      MaterialType type; /*!< Type of the material.*/
      Data data; /*!< The data of the material.*/
      MaterialParameters() {
        for(int i= 0; i< 8; ++i){
          data.textureColor.texture = 0;
        }
        data.textureColor.color.r = 0;
        data.textureColor.color.g = 0;
        data.textureColor.color.b = 0;
        data.textureColor.color.a = 0;

        data.solidColor.color.r = 0;
        data.solidColor.color.g = 0;
        data.solidColor.color.b = 0;
        data.solidColor.color.a = 0;

      }
    };
	

	};

 
}

#endif // !__MATERIAL_H__