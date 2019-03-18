--
-- User: maks
-- Date: 15/12/18
-- Time: 17:54
--

require('math')
require('assets/scripts/object')

Vector = Object:new { x = 0, y = 0, z = 0 }

function Vector.from(source)
    return Vector:new():set(source)
end

function Vector:set(source)

    self.x = source.x or source.r or source[1] or 0
    self.y = source.y or source.g or source[2] or 0
    self.z = source.z or source.b or source[3] or 0

    return self
end

function Vector:reset()

    self.x = 0
    self.y = 0
    self.z = 0

    return self
end

function Vector:__tostring()
    return "("..self.x..", "..self.y..", "..self.z..")";
end

function Vector:add(other)

    self.x = self.x + other.x
    self.y = self.y + other.y
    self.z = self.z + other.z

    return self
end

function Vector:sub(other)

    self.x = self.x - other.x
    self.y = self.y - other.y
    self.z = self.z - other.z

    return self
end

function Vector:mul(scalar)

    self.x = self.x * scalar
    self.y = self.y * scalar
    self.z = self.z * scalar

    return self
end

function Vector:div(scalar)

    self.x = self.x / scalar
    self.y = self.y / scalar
    self.z = self.z / scalar

    return self
end

function Vector:normalize()

    local magnitude = Vector.magnitude(self)
    if (magnitude > 0.0001) then
        Vector.div(self, magnitude)
    end

    return self
end

function Vector:setMagnitude(magnitude)

    Vector.normalize(self)
    Vector.mul(self, magnitude)
    return self
end

function Vector:truncate(maxMagnitude)

    if Vector.sqrMagnitude(self) > maxMagnitude * maxMagnitude then
        Vector.setMagnitude(self, maxMagnitude)
    end

    return self
end

function Vector:reflect(normal, bounciness)

    bounciness = bounciness or 1

    Vector.sub(self, Vector.__mul(normal, Vector.dot(self, normal)):mul(1 + bounciness))
end

function Vector:doCross(other)

    local x = self.y * other.z - self.z * other.y
    local y = self.z * other.x - self.x * other.z
    local z = self.x * other.y - self.y * other.x

    self.x = x
    self.y = y
    self.z = z

    return self
end

function Vector.__add(a, b)

    return Vector:new {
        x = a.x + b.x,
        y = a.y + b.y,
        z = a.z + b.z
    }
end

function Vector.__sub(a, b)

    return Vector:new {
        x = a.x - b.x,
        y = a.y - b.y,
        z = a.z - b.z
    }
end

function Vector.__div(vec, scalar)

    return Vector:new {
        x = vec.x / scalar,
        y = vec.y / scalar,
        z = vec.z / scalar
    }
end

function Vector.__mul(vec, scalar)

    return Vector:new {
        x = vec.x * scalar,
        y = vec.y * scalar,
        z = vec.z * scalar
    }
end

function Vector.__unm(vec)

    return Vector:new {
        x = -vec.x,
        y = -vec.y,
        z = -vec.z
    }
end

function Vector:dot(other)
    return self.x * other.x + self.y * other.y + self.z * other.z
end

function Vector.cross(a, b)
    return Vector.from(a):doCross(b)
end

function Vector:sqrMagnitude()
    return (self.x * self.x) + (self.y * self.y) + (self.z * self.z)
end

function Vector:magnitude()
    return math.sqrt((self.x * self.x) + (self.y * self.y) + (self.z * self.z))
end

function Vector:normalized()

    local magnitude = Vector.magnitude(self)
    if (magnitude > 0.0001) then
        return Vector.__div(self, magnitude)
    else
        return Vector:new()
    end
end

function Vector:distance(other)

    return Vector.magnitude({
        x = self.x - other.x,
        y = self.y - other.y,
        z = self.z - other.z
    })
end

function Vector:getMoveTowardsDelta(other, maxAmount)

    local delta = other - self;
    local magnitude = delta:magnitude()
    if (magnitude > maxAmount) then
        delta = delta / magnitude * maxAmount
    end

    return delta
end

function Vector:moveTowards(other, maxAmount)
    return self + self:getMoveTowardsDelta(other, maxAmount)
end