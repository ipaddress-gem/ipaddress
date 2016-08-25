
var internals = {};


internals.initializeConnections = function(cb) {
	console.log("bootstrap.initializeConnections: ENTERING");
	console.log("bootstrap.initializeConnections: Loading redis module");
	exports.redis = internals.redis = require('redis');
	console.log("bootstrap.initializeConnections: Loading redis client");
	exports.client = internals.client = internals.redis.createClient();
	console.log("bootstrap.initializeConnections: RETURN");
	cb();
}

internals.initialize = function(cb) {
	console.log("bootstrap.initialize: ENTERING");
	var namespace = internals.namespace = "crunchdev"; 	//TODO to config
	var tickfrequency = "1000";		//TODO to config
	
	var qb = require("./querybuilder");
	qb.useNamespace(internals.namespace);
	var timestamp = Date.now(); //in ms
	
	internals.client.get(qb.getStatus(), function(err, reply) {
		// reply is null when the key is missing
		console.log(reply);
		if (reply === "null" || reply === "offline" || reply !== "online" || reply !== "maintenance") {
			//Doesn't exist. Probably no other servers running using this namespace. This is a normal boot
			console.log("Doesn't exist");
			var async = require('async');
			
			internals.client.set("namespace", internals.namespace, internals.redis.print)
			internals.client.set(qb.getStatus(), "booting", internals.redis.print)
			internals.client.set(qb.getServerStartTime(), timestamp, internals.redis.print)
			internals.client.set(qb.getTickFrequency(), tickfrequency, internals.redis.print)
			
			internals.client.set(qb.getStatus(), "online", internals.redis.print)
			cb();
		} else {
			//A server already exists using this namespace. We're not doing anything yet with this.
			console.log("exists");
			//TODO: do something meaningful here.
			cb();
			//process.exit();
		}
	});
}

//TODO: move out to some cleanup module
internals.cleanup = function(cb) {
	var qb = require("./querybuilder");
	qb.useNamespace(internals.namespace);
	
	internals.client.set("namespace", "", internals.redis.print);
	internals.client.set(qb.getStatus(), "offline", internals.redis.print);
	internals.client.set(qb.getTickFrequency(), "", internals.redis.print);
	
	internals.client.quit(function() {
		console.log("Exiting via cleanup.");
		cb();
	});
}

module.exports = function() {
	console.log("Starting to initialize connections.");
	var async = require('async');
	
	async.series([
		function(cb) { internals.initializeConnections(cb); },
		function(cb) { internals.initialize(cb); },
		
		function(cb) { internals.cleanup(cb); }
	], function(err, results) {
		console.log("Bootstrap complete.");
	});
}