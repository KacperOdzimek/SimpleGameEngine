function on_init(e)
    _d_set_b("moving_up", true)
    _c_cl_set_collision_preset(e, "collider1", "body")
    io.write(_c_cl_get_collision_preset(e, "collider1"))
end

function on_update(e, dt)
    moving_up = _d_get("moving_up")
    x, y = _e_get_location(e)

    if (y > 3)  then moving_up = false end
    if (y < -3) then moving_up = true  end

    _d_set_b("moving_up", moving_up)

    if moving_up then
        _e_sweep(e, x, y + 3 * dt)
    else
        _e_sweep(e, x, y - 3 * dt)
    end

    _c_m_set_visible(e, "mesh", _c_m_get_visible(e, "mesh"))
end