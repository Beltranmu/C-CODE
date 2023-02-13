-- Create Scene
print("Create Scene 1")


local mask = ComponentType.NODE | ComponentType.RENDER | ComponentType.TRS_AND_TRANSFORM


cube = createEntity(mask)
cube:setTransform(0, 0, 0,  0, 0, 0,  100, 3, 100)
cube:setMaterial(cubeMat)
cubeObj = cube:loadObj('cube.obj')

cube1 = createEntity(mask)
cube1:setTransform(0, 10, 10,  0, 0, 0,  3, 3, 3)
cube1:setMaterial(cubeMat1)
--
--
--centerCube = createEntity(ComponentType.NODE | ComponentType.TRS_AND_TRANSFORM)
--centerCube:setTransform(0, 10, 3,  0, 0, 0,  1, 1, 1)
cube2 = createEntity(mask)
cube2:setTransform(0, 10, 3,  0, 0, 0,  4, 4, 4)
cube2:setRotation(45,0,0)
cube2:setMaterial(cubeMat2)
cube2:setObj(cubeObj)
--
octane = createEntity(mask)
octane:setTransform(0, 10, -20,  0, 90, 0,  0.1, 0.1, 0.1)
octane:setMaterial(octaneMat)
octane:loadObj('octane.obj')

cCube = createEntity(mask)
cCube:setTransform(0, 10, 20,  0, 90, 0,  0.01, 0.01, 0.01)
cCube:setMaterial(cCubeMat)
cCube:loadObj('cCube.obj')
--
--
--
--
--
atmosphere = createEntity(mask);
atmosphere:setPosition(0, 0, 0)
atmosphere:setRotation(0, 0, 0)
atmosphere:setScale(scale, scale, scale)
atmosphere:loadObj('jlh_sphere.obj')
atmosphere:setMaterial(atmosphereMat)



function update(time)
  local s1 = math.sin(time)
  local s2 = math.sin(time * 0.5)
  local s3 = math.sin(time * 0.25)
    cube2:setRotation(45,time*20,0)

  octane:setRotation(0,time*20, 0)
   setColor(cubeMat2,s1,s2,s3,1)
end