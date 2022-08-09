-- Lua wrapper for C++ ipelet "DCEL"

label = "draw_DCEL"

about = "This ipelet is for draw_DCEL"

-- this variable will store the C++ ipelet when it has been loaded
ipelet = false

function run(ui)
  if not ipelet then ipelet = assert(ipe.Ipelet(dllname)) end
  filter_save = { "TXT (*.txt)", "*.txt"}
  file_name = ipeui.fileDialog(parent, "open","DCEL open", filter_save, nil, nil,nil)
  --ipeui.messageBox(parent, "information", file_name, nil, "ok")
  parameters = {f_name = file_name}
  ui:runIpelet(label, ipelet,1 ,parameters) 
end

-- shortcut
-- shortcuts.ipelet_graham= "Alt+Ctrl+G"