function main()
print("[Lua] MyMod.lua - main() iniciado")

    RegisterEvent("Application.OnUpdate", function(deltaTime)
        if ClientUpdate and ClientUpdate.OnUpdate then
            ClientUpdate.OnUpdate(deltaTime)
        else
            print("[Lua] ClientUpdate.OnUpdate não encontrado!")
        end
    end)
end