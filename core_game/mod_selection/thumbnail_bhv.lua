local function in_range(value, min, max)
    return value >= min and value <= max
end

local dst = 2

function event_assign_mod(e, args)
    self.mod = args.mod
end

function event_check(e, args)
    local x, y = _e_get_location(e)

    if in_range(args.mx, x - dst, x + dst) and in_range(args.mx, x - dst, x + dst) then
        _m_load_mod(self.mod)
    end
end