function on_init(e)
    --save intial postition - center of the circle
    x, y = _e_get_location(e)
    _d_set_f("a", 0); _d_set_f("x", x); _d_set_f("y", y)
    _c_cl_set_collision_preset(e, "collider1", "body")
end

function on_update(e, dt)
    x_change = _i_axis("move_right") * dt
    y_change = _i_axis("move_forward") * dt

    x, y = _e_get_location(e)
    _e_sweep(e, x + x_change, y)
    x, y = _e_get_location(e)
    _e_sweep(e, x, y + y_change)
end

function on_destroy(e)
    --print some info
    x, y = _e_get_location(e)
    io.write("dead with x = "..x..", y ="..y.." and angle = ".._d_get("a").."[radians].\n")
end

function event_hit(e, args)
    io.write("Some num: "..args[1].."\n")
    --_e_kill(e)
end