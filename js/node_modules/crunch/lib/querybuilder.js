
var internals = {};
internals.namespace = "";
internals.build = function (input) {
	return internals.namespace + ":" + input;
}
internals.keys = {
	status: "systemStatus",
	tickFrequency: "tickFrequency",
	serverStartTime: "serverStartTime"

};


exports.useNamespace = function (namespace) {
	internals.namespace = namespace;
}
exports.getStatus = function() {
	return internals.build(internals.keys.status);
}
exports.getTickFrequency = function() {
	return internals.build(internals.keys.tickFrequency);
}
exports.getServerStartTime = function() {
	return internals.build(internals.keys.serverStartTime);
}
//module.exports = internals.initialize;