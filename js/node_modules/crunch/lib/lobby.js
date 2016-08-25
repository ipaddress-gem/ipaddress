var internals = {};
internals.minimumPlayers = 2; //minimum to start a game from the lobby
internals.maximumPlayers = 8;

internals.players = [];

internals.addPlayer = function(player) {
	//TODO: assert this is a player and that the id does not already exist in the lobby.
	internals.player.push(player);
}


module.exports = function() {
	console.log("Initializing Lobby");
	//TODO: Check for registered players to add to lobby
}