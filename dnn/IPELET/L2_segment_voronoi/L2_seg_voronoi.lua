-- Lua wrapper for C++ ipelet "graham"

label = "L2SegVoronoi"

about = "This ipelet is for finding convex hull"

-- this variable will store the C++ ipelet when it has been loaded
ipelet = false

function run(ui)
  if not ipelet then ipelet = assert(ipe.Ipelet(dllname)) end
  ui:runIpelet(label, ipelet) 
end

-- shortcut
shortcuts.ipelet_graham= "Alt+Ctrl+G"
