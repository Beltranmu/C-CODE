-- GENIe solution to build the project.
-- Algoritmos & Inteligencia Artificial
-- Ivan Sancho, Toni Barella
-- ESAT 2020 / 21

PROJ_DIR = path.getabsolute("./../")

solution("DS_ALG_AI1" .. _ACTION)
  location(PROJ_DIR .. "/build/")
  language "C"
  kind "ConsoleApp"
  startproject "PR0_MemoryNode"
  platforms {
    "x32",
    "x64"
  }

  defines {
    "_CRT_SECURE_NO_WARNINGS",
  }
  flags {
    "ExtraWarnings",
  }

  configurations {
    "Debug",
    "Release"
  }

  configuration "Debug"
    flags {
      "Symbols",
      "NoPCH",
    }
    targetsuffix "_d"
    defines {
      "DEBUG",
    }
    targetdir(path.join(PROJ_DIR, "./bin/Debug"))

  configuration "Release"
    flags {
      "Optimize",
      "NoPCH",
    }
    targetdir(path.join(PROJ_DIR, "./bin/Release"))
   --Array de Lua para los proyectos
  projects_names = {
    "PR0_MemoryNode",
    "PR1_Vector",
    "PR2_Circular_Vector",
    "PR3_Movable_Head_Vector",
    "PR4_List",
	  "PR5_Circular_List",
    "PR6_DL_List",
	  "PR7_Circular_DL_List",
    "PR8_Stack",
    "PR9_Queue",
    "PR10_Logger",
    "PR11_Comparative",
    "PR12_Sorting"
  }

  for i, prj in ipairs(projects_names) do
    project(prj)
    targetname(prj)
    location (PROJ_DIR .. "./build/" .. prj .. "/" .. _ACTION)

    includedirs {
      path.join(PROJ_DIR, "./include/"),
      path.join(PROJ_DIR, "./deps/include/"),
    }
    libdirs { path.join(PROJ_DIR, "./deps/lib/ABGS_MemoryManager") }

    files {
      path.join(PROJ_DIR, "./include/*.h"),
      path.join(PROJ_DIR, "./deps/include/common_def.h"),
      path.join(PROJ_DIR, "./deps/include/ABGS_MemoryManager/*.h"),
    }

    configuration "vs2017"
      windowstargetplatformversion "10.0.16299.0"
      --windowstargetplatformversion "10.0.17134.0"
      --windowstargetplatformversion "10.0.17134.471"

    configuration "vs2019"
      windowstargetplatformversion "10.0.18362.0"
      --windowstargetplatformversion "10.0.17763.0"
      --windowstargetplatformversion "10.0.17134.0"

    configuration "Debug"
      objdir(path.join(PROJ_DIR, "./build/" .. prj .. "/" .. "Debug"))
      links { "ABGS_MemoryManager_w32d" }
      --links { "ABGS_MemoryManager_w64d" }

	configuration "Release"
	  objdir(path.join(PROJ_DIR, "./build/" .. prj .. "/" .. "Release"))
	  links { "ABGS_MemoryManager_w32" }
	  --links { "ABGS_MemoryManager_w64" }
	end

  project "PR0_MemoryNode"
  	files {
  	  path.join(PROJ_DIR, "./include/adt_memory_node.h"),
  	  path.join(PROJ_DIR, "./src/adt_memory_node.c"),
  	  path.join(PROJ_DIR, "./tests/test_memory_node.c"),
    }
--[[Comentario multilinea
  eieieieieieieieieieieiei la candea del poject tiene que coincidir  con la del
  array de arriba]]--
  project "PR1_Vector"
   files {
     path.join(PROJ_DIR, "./include/adt_memory_node.h"),
     path.join(PROJ_DIR, "./src/adt_memory_node.c"),
     path.join(PROJ_DIR, "./include/adt_vector.h"),
     path.join(PROJ_DIR, "./src/adt_vector.c"),
     path.join(PROJ_DIR, "./tests/test_vector.c"),
   }
   
  project "PR2_Circular_Vector"
   files {
     path.join(PROJ_DIR, "./include/adt_memory_node.h"),
     path.join(PROJ_DIR, "./src/adt_memory_node.c"),
     path.join(PROJ_DIR, "./include/adt_circular_vector.h"),
     path.join(PROJ_DIR, "./src/adt_circular_vector.c"),
     path.join(PROJ_DIR, "./tests/test_circular_vector.c"),
   }
   
  project "PR3_Movable_Head_Vector"
   files {
     path.join(PROJ_DIR, "./include/adt_memory_node.h"),
     path.join(PROJ_DIR, "./src/adt_memory_node.c"),
     path.join(PROJ_DIR, "./include/adt_movable_head_vector.h"),
     path.join(PROJ_DIR, "./src/adt_movable_head_vector.c"),
     path.join(PROJ_DIR, "./tests/test_movable_head_vector.c"),
   }
  project "PR4_List"
   files {
     path.join(PROJ_DIR, "./include/adt_memory_node.h"),
     path.join(PROJ_DIR, "./src/adt_memory_node.c"),
     path.join(PROJ_DIR, "./include/adt_list.h"),
     path.join(PROJ_DIR, "./src/adt_list.c"),
     path.join(PROJ_DIR, "./tests/test_list.c"),
   }
   
  project "PR5_Circular_List"
   files {
     path.join(PROJ_DIR, "./include/adt_memory_node.h"),
     path.join(PROJ_DIR, "./src/adt_memory_node.c"),
     path.join(PROJ_DIR, "./include/adt_circular_list.h"),
     path.join(PROJ_DIR, "./src/adt_circular_list.c"),
     path.join(PROJ_DIR, "./tests/test_circular_list.c"),
  }

  project "PR6_DL_List"
   files {
     path.join(PROJ_DIR, "./include/adt_memory_node.h"),
     path.join(PROJ_DIR, "./src/adt_memory_node.c"),
     path.join(PROJ_DIR, "./include/adt_dl_list.h"),
     path.join(PROJ_DIR, "./src/adt_dl_list.c"),
     path.join(PROJ_DIR, "./tests/test_dl_list.c"),
  }

  project "PR7_Circular_DL_List"
   files {
     path.join(PROJ_DIR, "./include/adt_memory_node.h"),
     path.join(PROJ_DIR, "./src/adt_memory_node.c"),
     path.join(PROJ_DIR, "./include/adt_circular_dl_list.h"),
     path.join(PROJ_DIR, "./src/adt_circular_dl_list.c"),
     path.join(PROJ_DIR, "./tests/test_circular_dl_list.c"),
  }
  
  project "PR8_Stack"
   files {
     path.join(PROJ_DIR, "./include/adt_vector.h"),
     path.join(PROJ_DIR, "./src/adt_vector.c"),
     path.join(PROJ_DIR, "./include/adt_stack.h"),
     path.join(PROJ_DIR, "./src/adt_stack.c"),
     path.join(PROJ_DIR, "./tests/test_stack.c"),
  }
  
  project "PR9_Queue"
   files {
     path.join(PROJ_DIR, "./include/adt_list.h"),
     path.join(PROJ_DIR, "./src/adt_list.c"),
     path.join(PROJ_DIR, "./include/adt_queue.h"),
     path.join(PROJ_DIR, "./src/adt_queue.c"),
     path.join(PROJ_DIR, "./tests/test_queue.c"),
  }
  
  project "PR10_Logger"
   files {
     path.join(PROJ_DIR, "./include/adt_queue.h"),
     path.join(PROJ_DIR, "./src/adt_queue.c"),
     path.join(PROJ_DIR, "./include/adt_list.h"),
     path.join(PROJ_DIR, "./src/adt_list.c"),
     path.join(PROJ_DIR, "./include/adt_logger.h"),
     path.join(PROJ_DIR, "./src/adt_logger.c"),
     path.join(PROJ_DIR, "./tests/test_logger.c"),
  }
  
  project "PR11_Comparative"
   files {
   
     path.join(PROJ_DIR, "./include/adt_memory_node.h"),
     path.join(PROJ_DIR, "./src/adt_memory_node.c"),
   
     path.join(PROJ_DIR, "./include/adt_vector.h"),
     path.join(PROJ_DIR, "./src/adt_vector.c"),
     
     path.join(PROJ_DIR, "./include/adt_list.h"),
     path.join(PROJ_DIR, "./src/adt_list.c"),
     
     path.join(PROJ_DIR, "./include/adt_dl_list.h"),
     path.join(PROJ_DIR, "./src/adt_dl_list.c"),
    
     
     path.join(PROJ_DIR, "./tests/test_comparatives.c"),
  }
  
  project "PR12_Sorting"
   files {
   
     path.join(PROJ_DIR, "./include/adt_memory_node.h"),
     path.join(PROJ_DIR, "./src/adt_memory_node.c"),
     
     path.join(PROJ_DIR, "./include/adt_list.h"),
     path.join(PROJ_DIR, "./src/adt_list.c"),
     
     path.join(PROJ_DIR, "./include/adt_dl_list.h"),
     path.join(PROJ_DIR, "./src/adt_dl_list.c"),
    
     
     path.join(PROJ_DIR, "./tests/test_sorting.c"),
  }
  
  