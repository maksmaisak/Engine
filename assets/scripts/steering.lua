--
-- User: maks
-- Date: 2019-02-01
-- Time: 18:51
--

require('assets/scripts/object')
require('assets/scripts/vector')

Steering = Object:new {
    maxSpeed = 10,
    maxForce = 10,
    maxLookahead = 1,
    radius = 1
}

function Steering:init()

    self.position = self.position or Vector:new()
    self.velocity = self.velocity or Vector:new()
    self.steer    = self.steer    or Vector:new()
end

function Steering:update(dt)

    self.steer:truncate(self.maxForce)
    self.velocity:add(self.steer    * dt):truncate(self.maxSpeed)
    self.position:add(self.velocity * dt)

    self.steer:reset()
end

function Steering:seek(position)

    self.steer:add(
        Vector.from(position)
            :sub(self.position):setMagnitude(self.maxSpeed)
            :sub(self.velocity):truncate(self.maxForce)
    )
end

function Steering:flee(position)

    self.steer:add(
        Vector.from(self.position)
            :sub(position):setMagnitude(self.maxSpeed)
            :sub(self.velocity):truncate(self.maxForce)
    )
end

function Steering:alignVelocity(velocity)

    self.steer:add(
        Vector.from(velocity):truncate(self.maxSpeed):sub(self.velocity):truncate(self.maxForce)
    )
end

function Steering:brake()
    self:alignVelocity({0, 0, 0})
end

function Steering:arrive(position, radius)

    radius = radius or 1

    local distance = Vector.distance(position, self.position)
    local desiredVelocity = Vector.from(position)
        :sub(self.position):setMagnitude(self.maxSpeed)

    if (distance < radius) then
        desiredVelocity:mul(distance / radius)
    end

    self:alignVelocity(desiredVelocity)
end

function Steering:dodge(position, velocity)

    local toSide = Vector.from(velocity):normalize():doCross({x = 0, y = 1, z = 0}):normalize()
    local ahead = Vector.from(self.velocity):setMagnitude(self.maxLookahead):add(self.position)
    if Vector.from(ahead):sub(position):dot(toSide) < 0 then
        toSide:mul(-1)
    end

    self:alignVelocity(toSide:setMagnitude(self.maxSpeed))
end

local function predictNearestApproachTime(self, mover)

    local relativeVelocity = Vector.from(mover.velocity):sub(self.velocity)
    local relativeSpeed = relativeVelocity:magnitude()

    if (relativeSpeed < 0.001) then
        return 0
    end

    local fromMoverToSelf = Vector.from(self.position):sub(mover.position)
    return relativeVelocity:div(relativeSpeed * relativeSpeed):dot(fromMoverToSelf)
end

local function computeNearestApproachPositions(self, mover, time)

    local ownPosition   = Vector.from(self.velocity):mul(time):add(self.position)
    local otherPosition = Vector.from(mover.velocity):mul(time):add(mover.position)

    return Vector.distance(ownPosition, otherPosition), ownPosition, otherPosition
end

local cosAngleThreshold = math.cos(math.rad(45))
function Steering:avoidMovingObjects(movers, minTimeToCollision)

    minTimeToCollision = minTimeToCollision or 1
    local minTime = minTimeToCollision

    local threat
    local threatPositionAtNearestApproach = Vector:new()
    local selfPositionAtNearestApproach   = Vector:new()

    local collisionDangerThreshold = self.radius * 2;

    for i, mover in ipairs(movers) do

        --predicted time until nearest approach of 'self' and 'mover'
        local approachTime = predictNearestApproachTime(self, mover);
        if approachTime >= 0 and approachTime < minTime then

            local distance, selfPositionAtApproach, moverPositionAtApproach = computeNearestApproachPositions(self, mover, approachTime)
            if distance < collisionDangerThreshold then

                minTime = approachTime

                threat = mover
                selfPositionAtNearestApproach:set(selfPositionAtApproach)
                threatPositionAtNearestApproach:set(moverPositionAtApproach)
            end
        end
    end

    if not threat then
        return
    end

    local forward = self.velocity:normalized()
    local up = Vector:new {x = 0, y = 1, z = 0}
    local side = Vector.cross(forward, up)
    local forceDirection = Vector.from(side)

    local cosAngle = forward:dot(Vector.normalized(threat.velocity))
    if cosAngle < -cosAngleThreshold then

        -- head-on, steer away to the side from future threat position
        if Vector.from(selfPositionAtNearestApproach):sub(threatPositionAtNearestApproach):dot(side) < 0 then
            forceDirection:mul(-1)
        end

    elseif cosAngle > cosAngleThreshold then

        -- parallel, steer away to the side from current threat position
        if Vector.from(selfPositionAtNearestApproach):sub(threat.position):dot(side) < 0 then
            forceDirection:mul(-1)
        end
    else

        -- perpendicular, steer away from its line of motion
        local relativeVelocityDirection = Vector.from(threat.velocity):sub(self.velocity):normalize()
        local fromThreatToSelf = Vector.from(selfPositionAtNearestApproach):sub(threatPositionAtNearestApproach)
        forceDirection:set(Vector.cross(relativeVelocityDirection, up)):normalize()
        if forceDirection:dot(fromThreatToSelf) < 0 then
            forceDirection:mul(-1)
        end
    end

    self.steer:add(forceDirection:mul(self.maxForce))
end