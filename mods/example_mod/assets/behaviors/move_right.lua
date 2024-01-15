local a_id = 2

function on_init(e)
    x, y = _e_get_location(e)
    _d_set_f(a_id, 0); _d_set_f("x", x); _d_set_f("y", y)
end

function on_update(e, dt)
    a = _d_get(a_id); x = _d_get("x"); y = _d_get("y")
    _e_set_location(e, x + math.sin(a), y + math.cos(a))
    _d_set_f(a_id, a + 2 * dt)
end

function on_destroy(e)
end