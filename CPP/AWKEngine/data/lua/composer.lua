-- Composer LUA

renderShadowScene()
t0 = renderScene()

-- Post Proceso 1 t1 = blur(t0, 10, "Both")
-- Post Proceso 2
-- Post Proceso 3

displayScene(t0)
