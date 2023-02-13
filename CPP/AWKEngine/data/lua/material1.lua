-- Creations of the materials
--Material Types -> TextureColor, BlinnPhong


print("Create Materials 1")
cubeMat = createMaterial(MaterialType.BlinnPhong)
local gropundColor =0.5
setColor(cubeMat,gropundColor,gropundColor,gropundColor,1)
rickyTexture = createTexture("../../data/textures/1.png")
octaneTexture = createTexture("../../data/textures/octane.png")
setTexture(cubeMat, rickyTexture)


cubeMat1 = createMaterial(MaterialType.BlinnPhong)
setColor(cubeMat1,1,1,1,1)
setTexture(cubeMat1, rickyTexture)


cubeMat2 = createMaterial(MaterialType.BlinnPhong)
setColor(cubeMat2,1,1,1,1)

octaneMat = createMaterial(MaterialType.BlinnPhong)
setColor(octaneMat,1,1,1,1)
setTexture(octaneMat, octaneTexture)

cCubeMat = createMaterial(MaterialType.BlinnPhong)
setColor(cCubeMat,1,1,0,1)

atmosphereMat = createMaterial(MaterialType.Atmosphere)

