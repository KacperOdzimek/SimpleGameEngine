local x_d_pos = 1
local y_d_pos = 2

function on_init(e, d)
    x, y = _e_get_location(e)
    
    _d_set_float(d, x_d_pos, x)
    _d_set_float(d, y_d_pos, y)
end

function on_update(e, d, dt)
    x = _d_get_float(d, x_d_pos)
    y = _d_get_float(d, y_d_pos)

    _e_set_location(e, x, y)

    _d_set_float(d, x_d_pos, x + 8 * dt)
    _d_set_float(d, y_d_pos, y + 8 * dt)
end

function on_destroy(e, d)
end