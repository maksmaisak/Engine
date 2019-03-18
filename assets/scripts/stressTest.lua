--
-- User: maks
-- Date: 2019-01-23
-- Time: 10:51
--

require('math')
require('assets/scripts/vector')

local scene = {}

local boids = {}

local multipliers = {
    separation = 1.5,
    alignment = 1,
    cohesion = 1
}

local maxSpeed = 5
local maxForce = 0.5
local neighborhoodDistance = 12
local desiredSeparationDistance = 8

local position = Vector:new()
local velocity = Vector:new()
local otherPosition = Vector:new()

local function getAcceleration(i, boid, acceleration)

    acceleration = acceleration or Vector:new()

    position:set(boid.transform.position)
    velocity:set(boid.velocity)

    local numNeighbors = 0
    local numSeparationBoids = 0

    local separation = Vector:new()
    local cohesion   = Vector:new()
    local alignment  = Vector:new()
    for j, other in ipairs(boids) do

        if i ~= j then

            otherPosition:set(other.transform.position)
            local away = position - otherPosition
            local sqrDistance = away:sqrMagnitude()
            if (sqrDistance > 0.001) then

                if (sqrDistance < neighborhoodDistance * neighborhoodDistance) then

                    cohesion:add(otherPosition)
                    alignment:add(other.velocity)
                    numNeighbors = numNeighbors + 1
                end

                if (sqrDistance < desiredSeparationDistance * desiredSeparationDistance) then

                    separation:add(away / sqrDistance);
                    numSeparationBoids = numSeparationBoids + 1
                end
            end
        end
    end

    if numNeighbors > 0 then
        cohesion:div(numNeighbors)
        alignment:div(numNeighbors)
    end

    if numSeparationBoids > 0 then
        separation:div(numSeparationBoids)
    end

    separation:setMagnitude(maxSpeed):sub(velocity)
    acceleration:add(separation:mul(multipliers.separation):truncate(maxForce))

    cohesion:sub(position):setMagnitude(maxSpeed):sub(velocity)
    acceleration:add(cohesion:mul(multipliers.cohesion):truncate(maxForce))

    alignment:setMagnitude(maxSpeed):sub(velocity)
    acceleration:add(alignment:mul(multipliers.alignment):truncate(maxForce))

    return acceleration
end

local boidMaterial = Game.makeMaterial { shininess = 100 }

function scene:start()

    local function makeBoid(position)

        local radius = 0.2

        local actor = Game.makeActor {
            Name = "Boid",
            Transform = {
                position = position,
                scale = {radius, radius, radius}
            },
            RenderInfo = {
                mesh = "models/cube_flat.obj",
                material = boidMaterial
            }
        }

        local boid = {
            actor = actor,
            transform = actor:get("Transform"),
            velocity = Vector.from({
                math.random(-maxSpeed, maxSpeed),
                math.random(-maxSpeed, maxSpeed),
                math.random(-maxSpeed, maxSpeed)
            }):truncate(maxSpeed)
        }

        table.insert(boids, boid)

        return boid
    end

    local pi2 = math.pi * 2.0
    for radius = 2,41,6 do
        for theta = 0.0,pi2 - 0.001,pi2/8.0 do

            local position = {
                x = math.cos(theta) * radius,
                y = 0,
                z = math.sin(theta) * radius
            }

            makeBoid(position)
        end
    end

    Game.makeActor {
        Name = "Center",
        Transform = {}
    }

    Game.makeActor {
        Name = "Camera",
        Transform = {
            position = {0, 100, 0},
            rotation = {-90, 0, 0 }
        },
        Camera = {},
        CameraOrbitBehavior = {
            target = "Center",
            distance = 100
        }
    }

    Game.makeActor {
        Name = "DirectionalLight",
        Light = {
            intensity = 0.1,
            color = {0, 0, 1},
            ambientColor = {1, 1, 1},
            kind = "DIRECTIONAL"
        },
        Transform = {
            rotation = {-20, 0, 0},
        }
    }

    Game.makeActor {
        Name = "PointLight",
        Light = {
            intensity = 20
        },
        Transform = {
            position = {0, 0, 0}
        }
    }
end

function scene:update(dt)

    for i, boid in ipairs(boids) do

        boid.velocity:add(getAcceleration(i, boid):mul(dt))

        local originToBoid = Vector.from(boid.transform.position)
        if (originToBoid:magnitude() > 50) then
            if (boid.velocity:dot(originToBoid) > 0) then
                boid.velocity:reflect(originToBoid:normalized():mul(-1), 2)
                --boid.velocity:add(originToBoid:normalize():mul(-dt))
            end
        end

        boid.velocity:truncate(maxSpeed)

        local move = boid.velocity * dt
        boid.transform:move(move.x, move.y, move.z)
    end
end

return scene