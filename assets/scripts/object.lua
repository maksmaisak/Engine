--
-- User: maks
-- Date: 15/12/18
-- Time: 16:47
--

Object = {}

function Object:new(o)

    o = o or {}

    if (not rawget(self, '__index')) then
        self.__index = self
    end

    setmetatable(o, self)

    o:init()

    return o;
end

function Object:init() end

function Object:getPrototype()
    return getmetatable(self).__index
end

function Object:inheritsFrom(potentialPrototype)

    local prototype = self:getPrototype()
    if (prototype == potentialPrototype) then return true end

    return prototype ~= nil and prototype:inheritsFrom(potentialPrototype)
end
