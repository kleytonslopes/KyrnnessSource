function OnQuitGameButtonClicked()
	print("[Lua] Quiting Game.");
	Game.QuitGame();
end

function OnNewGameButtonClicked()
	print("[Lua] New Game.");
	Game.PlaySound("music");
	Engine.Log("Playing music!");
end