function on_init(e)
    x, y = _e_get_location(e)
    _d_set_f("a", 0); _d_set_f("x", x); _d_set_f("y", y)
    _c_cl_set_collision_preset(e, "collider1", "body")
end

function on_update(e, dt)
    speed = 2

    x_change = _i_axis("move_right") * dt * speed
    y_change = _i_axis("move_forward") * dt * speed

    if (_i_action_just_pressed("action1")) then
        new = _e_create()
        _e_teleport(new, -2, -2)
        config = {
            shader = "mod/shaders/cat_shader",
            mesh = "core/square_mesh",
            textures = {
                "mod/textures/cat_texture"
            }
        }
        _e_add_mesh(new, "mesh", config)
        _d_set_e("obj", new)
        _e_set_layer(new, 1)
    elseif (_i_action_just_relased("action1")) then
        _e_kill(_d_get("obj"))
    end

    x, y = _e_get_location(e)
    _e_sweep(e, x + x_change, y + y_change)
end