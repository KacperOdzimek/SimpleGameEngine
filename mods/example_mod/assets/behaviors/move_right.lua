local angle_id = 2
local x_id = 3
local y_id = 3

function on_init(e, d)
    x, y = _e_get_location(e)
    _d_set_float(d, angle_id, 0); _d_set_float(d, x_id, x); _d_set_float(d, y_id, y)
end

function on_update(e, d, dt)
    a = _d_get_float(d, angle_id); x = _d_get_float(d, x_id); y = _d_get_float(d, y_id)
    _e_set_location(e, x + math.sin(a), y + math.cos(a))
    _d_set_float(d, angle_id, a + 2 * dt)
end

function on_destroy(e, d)
end