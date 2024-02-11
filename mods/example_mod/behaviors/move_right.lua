function on_init(e)
    --save intial postition - center of the circle
    x, y = _e_get_location(e)
    _d_set_f("a", 0); _d_set_f("x", x); _d_set_f("y", y)
    _c_cl_set_collision_preset(e, "collider1", "body")
end

function on_update(e, dt)
    a = _d_get("a")

    --rotate
    x = _d_get("x"); y = _d_get("y")
    _e_teleport(e, x + math.sin(a), y + math.cos(a))
    _d_set_f("a", a + 4 * dt)
end

function on_destroy(e)
    --print some info
    x, y = _e_get_location(e)
    io.write("dead with x = "..x..", y ="..y.." and angle = ".._d_get("a").."[radians].\n")
end

function on_collide(e, otr)
    _e_call(otr, "hit", {15})
end

function on_overlap(e, otr)
end