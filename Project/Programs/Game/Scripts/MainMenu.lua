Application = Application or {}

function main()
	 RegisterEvent("Application.OnUpdate", Application.OnUpdate)
end

function Application.OnPostInitialized()
    print("[Lua] App inicializado - Este é um callback de um script!")
end

function Application.OnUpdate()
    -- print("[Lua] OnUpdate!")
end

function OnQuitGameButtonClicked()
	print("[Lua] Quiting Game.");
	Game.QuitGame();
end

function OnNewGameButtonClicked()
	Game.LoadScene("Content/Maps/EntryMap.json")
end

function OnJoinGameButtonClicked()
	print("[Lua] Join Game.");
    -- UI.SetVisible("QuitGameButton", false);
end

function OnHostGameButtonClicked()
	print("[Lua] Host Game.");
    UI.RemoveElement("QuitGameButton");
end