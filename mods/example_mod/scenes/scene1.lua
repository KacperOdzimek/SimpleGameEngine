function on_init()
    tilemap = _e_create()
    _e_add_tilemap(tilemap, "tilemap", "mod/tilemaps/tilemap", "mod/textures/tileset")

    player = _e_create()
    _e_add_behavior(player, "behavior", "mod/behaviors/move_left")    
    _e_set_layer(player, 1)
    _e_add_camera(player, "cam", 16)
    _c_c_set_active(player, "cam")
end

function on_update(dt)
end

function on_destroy()
end