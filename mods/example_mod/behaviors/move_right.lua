function on_init(e)
    _d_set_b("moving_up", true)
    _c_cl_set_collision_preset(e, "collider1", "body")

    config = {
        shader = "mod/shaders/cat_shader",
        mesh = "core/square_mesh",
        textures = {
            "mod/textures/cat_texture"
        }
    }

    _e_add_mesh(e, "static_mesh", config)
end

function on_update(e, dt)
    moving_up = _d_get("moving_up")
    x, y = _e_get_location(e)

    if (y > 3)  then moving_up = false end
    if (y < -3) then moving_up = true  end

    io.write("V: "..tostring(moving_up).."\n")

    _d_set_b("moving_up", moving_up)

    if moving_up then
        _e_sweep(e, x, y + 3 * dt)
    else
        _e_sweep(e, x, y - 3 * dt)
    end
end

function on_destroy(e)
end