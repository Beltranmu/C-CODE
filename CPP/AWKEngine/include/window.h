/************************************************
 * Window Class
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/
#ifndef __WINDOW_H__
#define __WINDOW_H__ 1

#include <stdint.h>

struct GLFWwindow;


namespace AWK{
  class Engine;
  //! Window Class.
  /*! Represent the window for the engine. */
  class Window{
  
  public: 
      Window();
      ~Window();
      //! Create and initialize the window.
      /*!
      \param w. With of the window.
      \param h. Height of the window.
      \param w_name. Name of the screen.
      \return. True if the window is created correctly. Otherwise false.
      */
      bool initWindow(int32_t w, int32_t h, const char* w_name);
      //! Check if the window is opened.
      /*!
      \return. True if the window is opened. Otherwise false
      */
      bool isOpened();

      //! Make the swap of the buffers.
      /*!
      */
      void swap();      

      //! close the Window.
      /*!
      */
      void closeWindow();
      void drawImgui(Engine* engine);
      void moveCamera(Engine* engine);
      void drawCameraImgui(Engine* engine);

     

      bool showHierarchy = false;

      //! Data struct.
      /*! Forward declaration of the struct of the information for the window. */
      struct Data; 

    private:     
      double mouseX, mouseY;
      Data* _data = nullptr; /*!<Dark pointer to all the data of needed for the window. */

  };

};

#endif // !__WINDOW_H__