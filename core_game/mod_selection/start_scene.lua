local thumbnail_p = require("mod/thumbnail")

local thumbnails = {}

function on_init()
   _i_set_mouse_visible(true)

   local camera = _e_create()
   _e_add_camera(camera, "cam", 56)
   _c_c_set_active(camera, "cam")
   _e_teleport(camera, 0, -0.75)

   local title = _e_create()
   _e_add_sprite(title, "title", "mod/text", 0, "ignore")
   _c_m_set_scale(title, "title", 0.25, 0.25)
   _e_teleport(title, 0, 13)

   for k, v in pairs(_m_get_all_mods()) do
      thumbnails[k] = thumbnail_p.create_thumbnail(k, v)
      _e_call(thumbnails[k], "assign_mod", {mod = v})
   end
end

function on_update()
   if not _i_action_just_pressed("click") then do return end end

   local mx, my = _i_get_mouse_position()

   for k, v in pairs(thumbnails) do
      _e_call(v, "check", {mx = mx, my = my})
   end
end