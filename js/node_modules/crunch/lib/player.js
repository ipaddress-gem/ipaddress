var internals = {};
internals.id = -1;
internals.x = 0;
internals.y = 0;
//TODO: better way to do facing ?
internals.facing = 0;
//0 - right
//1 - up
//2 - left
//3 - down
internals.name = "Player";

internal.setName = function (input) {
	//TODO: validation?
	internals.name = input;
}

module.exports = function() {
	return internals;
};