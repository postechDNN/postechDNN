-- Lua script.
p=tetview:new()
p:load_mesh("example1.face")
rnd=glvCreate(0, 0, 500, 500, "TetView")
p:plot(rnd)
glvWait()
