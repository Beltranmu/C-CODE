-- Creations of the materials
--Material Types -> TextureColor, BlinnPhong


print("Create Materials")
mat1 = createMaterial(MaterialType.TextureColor)
setColor(mat1,0,1,1,1)
rickyTexture = createTexture("../../data/textures/1.png")
setTexture(mat1, rickyTexture)
print(mat1)

atmosphereMat = createMaterial(MaterialType.Atmosphere)

isalndMat = createMaterial(MaterialType.BlinnPhong)
setColor(isalndMat,1,1,0,1)

palmtreeMat = createMaterial(MaterialType.BlinnPhong)
setColor(palmtreeMat,0,1,0,1)
