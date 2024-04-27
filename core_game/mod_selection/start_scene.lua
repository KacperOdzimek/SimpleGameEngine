function on_init()
   _i_set_mouse_visible(true)

   local camera = _e_create()
   _e_add_camera(camera, "cam", 56)
   _c_c_set_active(camera, "cam")
   _e_teleport(camera, 0, -0.75)

   print("MS")
end