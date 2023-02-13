-- Create Scene
print("Create Scene")


local mask = ComponentType.NODE | ComponentType.RENDER | ComponentType.TRS_AND_TRANSFORM


cube = createEntity(mask)
cube:setTransform(10, 10, 0,  0, 3.14, 0,  4, 4, 4)
cube:setMaterial(mat1)
--doplhin = createEntity(mask)
--dolphin1 = createEntity(ComponentType.NODE | ComponentType.RENDER | ComponentType.TRS_AND_TRANSFORM)

--doplhin:setTransform(2, 2, 0,  0, 3.14, 0,  1, 1, 1)
--doplhin:setMaterial(2)
--dolphin1:setTransform(-2, 2, 0,  0, 3.14, 0,  1, 1, 1)
--dolphin1:setMaterial(4)
--dolphinObj = doplhin:loadObj('d2.obj')
--dolphin1:setObj(dolphinObj);


island = createEntity(ComponentType.NODE | ComponentType.RENDER | ComponentType.TRS_AND_TRANSFORM)
floor = island:loadObj('platform_beach.obj')
island:setPosition(0, 0, 0)
island:setRotationInRadians(0, 0, 0)
island:setScale(50, 50, 50)
island:setMaterial(isalndMat)
----
palitroque = createEntity(mask)
--
palitroque:setPosition(0,0,0)
palitroque:setRotation(0, 0, 0)
palitroque:setScale(10,10,7)
palitroque:loadObj('tree_palm.obj')
palitroque:setMaterial(palmtreeMat)
--
--water = createEntity(ComponentType.NODE | ComponentType.RENDER | ComponentType.TRS_AND_TRANSFORM)
--water:setPosition(0, 0, 0)
--water:setRotation(90, 0, 0)
--water:setScale(1000000, 1000000, 1000000)
--water:setMaterial(5)

--scale = 0.0078125 * 1000
scale = 1

atmosphere = createEntity(mask);
----
atmosphere:setPosition(0, 0, 0)
atmosphere:setRotation(0, 0, 0)
atmosphere:setScale(scale, scale, scale)
atmosphere:loadObj('jlh_sphere.obj')
atmosphere:setMaterial(atmosphereMat)



--print(atmosphere)

function update(time)
--    palitroque:setRotation(0, 10 * time, 0)
--    palitroque:setScale(1,  10*time, 1)
r = time *0.05

setColor(mat1, 0, 0,r,1)
end