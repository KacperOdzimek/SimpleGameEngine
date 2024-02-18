function on_init(e)
    x, y = _e_get_location(e)
    _d_set_f("a", 0); _d_set_f("x", x); _d_set_f("y", y)
    _c_cl_set_collision_preset(e, "collider1", "body")

    config = {
        shader = "mod/shaders/cat_shader",
        mesh = "core/square_mesh",
        textures = {
            "mod/textures/cat_texture"
        }
    }

    _e_add_mesh(e, "mesh", config)
end

function on_update(e, dt)
    speed = 2

    x_change = _i_axis("move_right") * dt * speed
    y_change = _i_axis("move_forward") * dt * speed

    x, y = _e_get_location(e)
    _e_sweep(e, x + x_change, y + y_change)
end