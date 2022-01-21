----------------------------------------------------------------------
-- graph ipelet description
----------------------------------------------------------------------
label = "MST"
about = [[
Constructs a graph from selected points and segments
This ipelet is part of Ipe.
]]
-- this variable will store the C++ ipelet when it has been loaded
ipelet = false
function run(model)
  if not ipelet then ipelet = assert(ipe.Ipelet(dllname)) end
  model:runIpelet(label, ipelet) 
end
-- define a shortcut for this function
shortcuts.ipelet_1_kgon = "Alt+Ctrl+K"
----------------------------------------------------------------------