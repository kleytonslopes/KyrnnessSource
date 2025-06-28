ClientUpdate = {}

function ClientUpdate.OnUpdate(deltaTime)
    local player = Game.getPlayer()
    if player:IsValid() then
        local location = player:GetLocation()
        location.X = location.X + deltaTime * 0.2
        player:SetLocation(location)
        player:SetRotation(FVector(0, 90, 0))
        player:SetScale(FVector(1, 1, 1))
    end
end