function on_init(e)
    --save intial postition - center of the circle
    x, y = _e_get_location(e)
    _d_set_f("a", 0); _d_set_f("x", x); _d_set_f("y", y)
end

function on_update(e, dt)
    --check if should die
    a = _d_get("a")
    if a > 6.28 then
        _e_kill(e)
    end

    --rotate
    if _is_e_valid(e) then
        x = _d_get("x"); y = _d_get("y")
        _e_set_location(e, x + math.sin(a), y + math.cos(a))
        _d_set_f("a", a + 6 * dt)
    end
end

function on_destroy(e)
    --print some info
    x, y = _e_get_location(e)
    io.write("dead with x = "..x..", y ="..y.." and angle = ".._d_get("a").."[radians].\n")
end