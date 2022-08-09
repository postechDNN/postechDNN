-- Lua wrapper for C++ ipelet "graham"

label = "L1_voronoi"

about = "This ipelet is for L1_voronoi"

-- this variable will store the C++ ipelet when it has been loaded
ipelet = false

function run(ui)
  if not ipelet then ipelet = assert(ipe.Ipelet(dllname)) end
  ui:runIpelet(label, ipelet) 
end

-- shortcut
-- shortcuts.ipelet_graham= "Alt+Ctrl+G"