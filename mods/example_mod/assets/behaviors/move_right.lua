function on_init(e)
    x, y = _e_get_location(e)
    _d_set_f("a", 0); _d_set_f("x", x); _d_set_f("y", y)
end

function on_update(e, dt)
    _d_set_e("e", e)
    ent = _d_get("e")
    --rotate
    a = _d_get("a"); x = _d_get("x"); y = _d_get("y")
    _e_set_location(ent, x + math.sin(a), y + math.cos(a))
    _d_set_f("a", a + 2 * dt)
end

function on_destroy(e)
end