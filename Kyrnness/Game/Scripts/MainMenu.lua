function OnQuitGameButtonClicked()
	print("[Lua] Quiting Game.");
	Game.QuitGame();
end

function OnNewGameButtonClicked()
	print("[Lua] New Game.");
	Game.PlaySound("music");
	Engine.Log("Playing music!");
end

function OnJoinGameButtonClicked()
	print("[Lua] Join Game.");
    UI.SetVisible("QuitGameButton", false);
end

function OnHostGameButtonClicked()
	print("[Lua] Host Game.");
    UI.SetVisible("QuitGameButton", true);
end